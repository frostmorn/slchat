/* client.cpp - login, LLUDP circuits, message handlers, event queue, teleport */
#include "sl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <algorithm>

#define CHANNEL_NAME "slchat-c11"
#define CLIENT_VERSION "0.1.0"
#define MAX_PACKET 1200

/* ================================================================== */
/* small helpers                                                       */
/* ================================================================== */

double now_s()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

static void say_line(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    putchar('\n');
    fflush(stdout);
}

#define VLOG(cl, ...) do { if ((cl)->verbose) { printf("[v] "); say_line(__VA_ARGS__); } } while (0)

/* remote text may contain terminal escapes: neutralise control bytes */
static std::string clean(const std::string& s)
{
    std::string o = s;
    for (size_t i = 0; i < o.size(); i++) {
        unsigned char c = (unsigned char)o[i];
        if ((c < 0x20 && c != '\n') || c == 0x7f) o[i] = '?';
    }
    return o;
}

static std::string trim(const std::string& s)
{
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) a++;
    while (b > a && isspace((unsigned char)s[b - 1])) b--;
    return s.substr(a, b - a);
}

static std::string unquote(const std::string& s)
{
    if (s.size() >= 2 && s[0] == '"' && s[s.size() - 1] == '"') return s.substr(1, s.size() - 2);
    return s;
}

static std::string lower(std::string s)
{
    for (size_t i = 0; i < s.size(); i++) s[i] = (char)tolower((unsigned char)s[i]);
    return s;
}

/* cut to max bytes without splitting a UTF-8 sequence */
static std::string utf8_trunc(const std::string& s, size_t max)
{
    if (s.size() <= max) return s;
    size_t n = max;
    while (n > 0 && ((unsigned char)s[n] & 0xC0) == 0x80) n--;
    return s.substr(0, n);
}

static void known_add(Client* cl, const Uuid& id, const std::string& name)
{
    if (name.empty() || uuid_is_zero(id)) return;
    for (size_t i = 0; i < cl->known.size(); i++)
        if (uuid_eq(cl->known[i].id, id)) { cl->known[i].name = name; return; }
    if (cl->known.size() >= 500) cl->known.erase(cl->known.begin());
    Known k; k.id = id; k.name = name;
    cl->known.push_back(k);
}

/* ================================================================== */
/* login (XML-RPC over HTTPS, blocking)                                */
/* ================================================================== */

static size_t wr_cb(char* p, size_t sz, size_t n, void* ud)
{
    ((std::string*)ud)->append(p, sz * n);
    return sz * n;
}

static void xr_str(std::string& o, const char* k, const std::string& v)
{
    o += "<member><name>"; o += k; o += "</name><value><string>";
    o += xml_escape(v);
    o += "</string></value></member>";
}
static void xr_int(std::string& o, const char* k, long v)
{
    char t[32]; snprintf(t, sizeof t, "%ld", v);
    o += "<member><name>"; o += k; o += "</name><value><int>"; o += t; o += "</int></value></member>";
}
static void xr_bool(std::string& o, const char* k, bool v)
{
    o += "<member><name>"; o += k; o += "</name><value><boolean>";
    o += v ? "1" : "0";
    o += "</boolean></value></member>";
}

bool client_login(Client* cl, const std::string& uri, const std::string& user,
                  const std::string& password, const std::string& start,
                  bool agree_tos, const std::string& mfa_token,
                  const std::string& mfa_hash, std::string* reason,
                  std::string* message, std::string* new_mfa_hash)
{
    /* "First Last", "First.Last" or "First" (=> last name Resident) */
    std::string first, last;
    size_t sp = user.find_first_of(" .");
    if (sp == std::string::npos) { first = user; last = "Resident"; }
    else { first = trim(user.substr(0, sp)); last = trim(user.substr(sp + 1)); }

    char host[256] = "host";
    gethostname(host, sizeof host - 1);
    std::string hid = md5_hex(host, strlen(host));

    std::string body =
        "<?xml version=\"1.0\"?><methodCall><methodName>login_to_simulator</methodName>"
        "<params><param><value><struct>";
    xr_str (body, "first", first);
    xr_str (body, "last", last);
    xr_str (body, "passwd", "$1$" + md5_hex(password.data(), password.size()));
    xr_str (body, "start", start);
    xr_str (body, "channel", CHANNEL_NAME);
    xr_str (body, "version", CLIENT_VERSION);
    xr_str (body, "platform", "Lin");
    xr_int (body, "address_size", (long)(sizeof(void*) * 8));
    xr_str (body, "mac", hid);
    xr_str (body, "id0", hid);
    xr_str (body, "host_id", "");
    xr_bool(body, "agree_to_tos", agree_tos);
    xr_bool(body, "read_critical", agree_tos);
    xr_bool(body, "extended_errors", true);
    xr_str (body, "token", mfa_token);
    xr_str (body, "mfa_hash", mfa_hash);
    body += "<member><name>options</name><value><array><data>"
            "<value><string>adult_compliant</string></value>"
            "</data></array></value></member>";
    body += "</struct></value></param></params></methodCall>";

    CURL* e = curl_easy_init();
    if (!e) { *reason = "internal"; *message = "curl init failed"; return false; }
    std::string resp;
    curl_slist* h = curl_slist_append(NULL, "Content-Type: text/xml");
    curl_easy_setopt(e, CURLOPT_URL, uri.c_str());
    curl_easy_setopt(e, CURLOPT_POST, 1L);
    curl_easy_setopt(e, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(e, CURLOPT_POSTFIELDSIZE, (long)body.size());
    curl_easy_setopt(e, CURLOPT_HTTPHEADER, h);
    curl_easy_setopt(e, CURLOPT_WRITEFUNCTION, wr_cb);
    curl_easy_setopt(e, CURLOPT_WRITEDATA, &resp);
    curl_easy_setopt(e, CURLOPT_TIMEOUT, 90L);
    curl_easy_setopt(e, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(e, CURLOPT_USERAGENT, CHANNEL_NAME "/" CLIENT_VERSION);
    CURLcode rc = curl_easy_perform(e);
    long status = 0;
    curl_easy_getinfo(e, CURLINFO_RESPONSE_CODE, &status);
    curl_slist_free_all(h);
    curl_easy_cleanup(e);

    if (rc != CURLE_OK) { *reason = "http"; *message = curl_easy_strerror(rc); return false; }

    Val v;
    std::string fault;
    if (!xmlrpc_parse_response(resp, &v, &fault)) {
        char t[64]; snprintf(t, sizeof t, "unparseable login reply (HTTP %ld)", status);
        *reason = "protocol"; *message = fault.empty() ? std::string(t) : fault;
        return false;
    }

    if (val_get(&v, "mfa_hash")) *new_mfa_hash = val_str(val_get(&v, "mfa_hash"));

    if (val_str(val_get(&v, "login"), "false") != "true") {
        *reason  = val_str(val_get(&v, "reason"), "unknown");
        *message = val_str(val_get(&v, "message"));
        return false;
    }

    cl->first  = unquote(val_str(val_get(&v, "first_name"), first.c_str()));
    cl->last   = unquote(val_str(val_get(&v, "last_name"),  last.c_str()));
    if (!uuid_parse(val_str(val_get(&v, "agent_id")).c_str(), &cl->agent_id) ||
        !uuid_parse(val_str(val_get(&v, "session_id")).c_str(), &cl->session_id)) {
        *reason = "protocol"; *message = "login reply lacks agent/session id";
        return false;
    }
    uuid_parse(val_str(val_get(&v, "secure_session_id")).c_str(), &cl->secure_session_id);
    cl->circuit_code = (uint32_t)val_int(val_get(&v, "circuit_code"));

    std::string ip   = val_str(val_get(&v, "sim_ip"));
    long        port = (long)val_int(val_get(&v, "sim_port"));
    struct sockaddr_in peer;
    memset(&peer, 0, sizeof peer);
    peer.sin_family = AF_INET;
    peer.sin_port   = htons((uint16_t)port);
    if (inet_pton(AF_INET, ip.c_str(), &peer.sin_addr) != 1 || port <= 0) {
        *reason = "protocol"; *message = "login reply has bad sim_ip/sim_port: " + ip;
        return false;
    }
    cl->cur.peer     = peer;
    cl->cur.seed_cap = val_str(val_get(&v, "seed_capability"));
    cl->login_handle = ((uint64_t)val_int(val_get(&v, "region_x")) << 32) |
                        (uint64_t)val_int(val_get(&v, "region_y"));
    cl->cur.handle   = cl->login_handle;
    *message = val_str(val_get(&v, "message"));
    return true;
}

/* ================================================================== */
/* circuits: sockets, reliable send/ack                                */
/* ================================================================== */

static bool circuit_init(Circuit* c, const struct sockaddr_in* peer)
{
    *c = Circuit();
    c->fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (c->fd < 0) return false;
    fcntl(c->fd, F_SETFL, fcntl(c->fd, F_GETFL, 0) | O_NONBLOCK);
    c->peer    = *peer;
    c->open    = true;
    c->last_rx = now_s();
    return true;
}

static void circuit_close(Circuit* c)
{
    if (c->open && c->fd >= 0) close(c->fd);
    *c = Circuit();
    c->fd = -1;
}

static void msg_number(Buf& p, uint32_t key)
{
    uint32_t f = key >> 16, n = key & 0xFFFF;
    switch (f) {
    case 0: put_u8(p, (uint8_t)n); break;
    case 1: put_u8(p, 0xFF); put_u8(p, (uint8_t)n); break;
    case 2: put_u8(p, 0xFF); put_u8(p, 0xFF); put_u8(p, (uint8_t)(n >> 8)); put_u8(p, (uint8_t)n); break;
    default: put_u8(p, 0xFF); put_u8(p, 0xFF); put_u8(p, 0xFF); put_u8(p, (uint8_t)n); break;
    }
}

static void raw_send(Circuit* c, const uint8_t* d, size_t n)
{
    sendto(c->fd, d, n, 0, (const struct sockaddr*)&c->peer, sizeof c->peer);
}

static void send_msg(Circuit* c, uint32_t key, const Buf& body, bool reliable)
{
    if (!c->open) return;
    Buf p;
    put_u8(p, reliable ? PF_RELIABLE : 0);
    c->seq++;
    if (c->seq >= 0x01000000u) c->seq = 1;
    p.push_back((uint8_t)(c->seq >> 24)); p.push_back((uint8_t)(c->seq >> 16));   /* big endian */
    p.push_back((uint8_t)(c->seq >> 8));  p.push_back((uint8_t)c->seq);
    put_u8(p, 0);                                                                /* no extra header */
    msg_number(p, key);
    p.insert(p.end(), body.begin(), body.end());
    if (p.size() > MAX_PACKET) return;
    if (reliable) {
        Pending pd;
        pd.seq = c->seq; pd.pkt = p; pd.sent = now_s(); pd.tries = 0;
        c->unacked.push_back(pd);
    }
    raw_send(c, &p[0], p.size());
}

static void ack_remove(Circuit* c, uint32_t seq)
{
    for (size_t i = 0; i < c->unacked.size(); i++)
        if (c->unacked[i].seq == seq) { c->unacked.erase(c->unacked.begin() + (long)i); return; }
}

static void flush_acks(Circuit* c)
{
    while (c->open && !c->to_ack.empty()) {
        size_t n = std::min<size_t>(c->to_ack.size(), 255);
        Buf b;
        put_u8(b, (uint8_t)n);
        for (size_t i = 0; i < n; i++) put_u32(b, c->to_ack[i]);       /* body: little endian */
        c->to_ack.erase(c->to_ack.begin(), c->to_ack.begin() + (long)n);
        send_msg(c, MSG_PacketAck, b, false);
    }
}

static void resend_due(Client* cl, Circuit* c, double t)
{
    for (size_t i = 0; i < c->unacked.size(); ) {
        Pending& pd = c->unacked[i];
        if (t - pd.sent < 3.0) { i++; continue; }
        if (pd.tries >= 4) {
            VLOG(cl, "dropping unacked packet seq %u", pd.seq);
            c->unacked.erase(c->unacked.begin() + (long)i);
            continue;
        }
        pd.pkt[0] |= PF_RESENT;
        raw_send(c, &pd.pkt[0], pd.pkt.size());
        pd.sent = t;
        pd.tries++;
        i++;
    }
}

/* ================================================================== */
/* outgoing messages                                                   */
/* ================================================================== */

static void agent_block(Client* cl, Buf& b)
{
    put_uuid(b, cl->agent_id);
    put_uuid(b, cl->session_id);
}

static void send_agent_update(Client* cl, Circuit* c)
{
    Buf b;
    agent_block(cl, b);
    put_f32(b, 0); put_f32(b, 0); put_f32(b, 0);        /* body rotation (identity) */
    put_f32(b, 0); put_f32(b, 0); put_f32(b, 0);        /* head rotation */
    put_u8(b, 0);                                       /* state */
    put_vec3(b, cl->pos[0], cl->pos[1], cl->pos[2]);    /* camera center */
    put_vec3(b, 1, 0, 0); put_vec3(b, 0, 1, 0); put_vec3(b, 0, 0, 1);
    put_f32(b, 8.0f);                                   /* draw distance: keep it small */
    put_u32(b, 0);                                      /* control flags */
    put_u8(b, 0);                                       /* flags */
    send_msg(c, MSG_AgentUpdate, b, false);
}

static void send_throttle(Client* cl, Circuit* c)
{
    /* resend, land, wind, cloud, task, texture, asset  (bits/second) */
    static const float th[7] = { 15000, 1000, 1000, 1000, 15000, 1000, 5000 };
    Buf b;
    agent_block(cl, b);
    put_u32(b, cl->circuit_code);
    put_u32(b, 0);                                      /* gen counter */
    put_u8(b, 28);
    for (int i = 0; i < 7; i++) put_f32(b, th[i]);
    send_msg(c, MSG_AgentThrottle, b, true);
}

static void send_handshake(Client* cl, Circuit* c)
{
    Buf b;
    put_u32(b, cl->circuit_code);
    put_uuid(b, cl->session_id);
    put_uuid(b, cl->agent_id);
    send_msg(c, MSG_UseCircuitCode, b, true);

    Buf m;
    agent_block(cl, m);
    put_u32(m, cl->circuit_code);
    send_msg(c, MSG_CompleteAgentMovement, m, true);

    send_agent_update(cl, c);
}

static void send_chat(Client* cl, const std::string& text, int type)
{
    Buf b;
    agent_block(cl, b);
    put_str2(b, utf8_trunc(text, 1023));
    put_u8(b, (uint8_t)type);
    put_u32(b, 0);                                      /* channel 0 = local chat */
    send_msg(&cl->cur, MSG_ChatFromViewer, b, true);
}

static void send_im(Client* cl, const Uuid& to, const std::string& text)
{
    Buf b;
    agent_block(cl, b);
    put_u8(b, 0);                                       /* FromGroup */
    put_uuid(b, to);
    put_u32(b, 0);                                      /* ParentEstateID */
    put_uuid(b, uuid_zero());                           /* RegionID */
    put_vec3(b, cl->pos[0], cl->pos[1], cl->pos[2]);
    put_u8(b, 0);                                       /* Offline */
    put_u8(b, IM_NOTHING_SPECIAL);
    put_uuid(b, uuid_xor(cl->agent_id, to));            /* p2p session id */
    put_u32(b, 0);                                      /* Timestamp */
    put_str1(b, cl->first + " " + cl->last);
    put_str2(b, utf8_trunc(text, 1023));
    put_u16(b, 1); put_u8(b, 0);                        /* empty binary bucket */
    put_u32(b, 0);                                      /* EstateBlock.EstateID */
    put_u8(b, 0);                                       /* MetaData: 0 blocks */
    send_msg(&cl->cur, MSG_ImprovedInstantMessage, b, true);
}

/* ================================================================== */
/* http (seed capability and event queue) on curl_multi                */
/* ================================================================== */

static void http_cancel(Client* cl, HttpReq* r)
{
    if (!r->active) return;
    curl_multi_remove_handle(cl->multi, r->easy);
    curl_easy_cleanup(r->easy);
    if (r->hdrs) curl_slist_free_all(r->hdrs);
    r->easy = NULL; r->hdrs = NULL; r->active = false;
}

static void http_start(Client* cl, HttpReq* r, int kind, const std::string& url,
                       const std::string& body, long timeout_s)
{
    http_cancel(cl, r);
    r->easy = curl_easy_init();
    if (!r->easy) return;
    r->body = body;
    r->resp.clear();
    r->kind = kind;
    r->hdrs = curl_slist_append(NULL, "Content-Type: application/llsd+xml");
    r->hdrs = curl_slist_append(r->hdrs, "Accept: application/llsd+xml");
    curl_easy_setopt(r->easy, CURLOPT_URL, url.c_str());
    curl_easy_setopt(r->easy, CURLOPT_POST, 1L);
    curl_easy_setopt(r->easy, CURLOPT_POSTFIELDS, r->body.c_str());
    curl_easy_setopt(r->easy, CURLOPT_POSTFIELDSIZE, (long)r->body.size());
    curl_easy_setopt(r->easy, CURLOPT_HTTPHEADER, r->hdrs);
    curl_easy_setopt(r->easy, CURLOPT_WRITEFUNCTION, wr_cb);
    curl_easy_setopt(r->easy, CURLOPT_WRITEDATA, &r->resp);
    curl_easy_setopt(r->easy, CURLOPT_TIMEOUT, timeout_s);
    curl_easy_setopt(r->easy, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(r->easy, CURLOPT_ACCEPT_ENCODING, "");
    curl_easy_setopt(r->easy, CURLOPT_USERAGENT, CHANNEL_NAME "/" CLIENT_VERSION);
    curl_multi_add_handle(cl->multi, r->easy);
    r->active = true;
}

static void seed_fetch(Client* cl, const std::string& url)
{
    if (url.empty()) return;
    http_start(cl, &cl->seed, HK_SEED, url,
               "<llsd><array><string>EventQueueGet</string></array></llsd>", 30);
}

static void eq_poll(Client* cl)
{
    if (cl->eq_url.empty() || cl->eq.active) return;
    std::string body = "<llsd><map><key>ack</key>";
    if (cl->eq_have_ack) { char t[40]; snprintf(t, sizeof t, "<integer>%lld</integer>", (long long)cl->eq_ack); body += t; }
    else body += "<undef/>";
    body += "<key>done</key><boolean>false</boolean></map></llsd>";
    http_start(cl, &cl->eq, HK_EQ, cl->eq_url, body, 90);
}

/* ================================================================== */
/* teleport                                                            */
/* ================================================================== */

static void tp_reset(Client* cl) { cl->tp = TP_IDLE; cl->tp_deadline = 0; }

static void send_tp_request(Client* cl, uint64_t handle)
{
    Buf b;
    agent_block(cl, b);
    put_u64(b, handle);
    put_vec3(b, cl->tp_pos[0], cl->tp_pos[1], cl->tp_pos[2]);
    put_vec3(b, 0, 1, 0);
    send_msg(&cl->cur, MSG_TeleportLocationRequest, b, true);
    cl->tp = TP_REQUESTED;
    cl->tp_deadline = now_s() + 60;
}

static void start_teleport(Client* cl, const std::string& region, const float pos[3])
{
    if (cl->tp != TP_IDLE) { say_line("* a teleport is already in progress"); return; }
    cl->tp_name = region;
    memcpy(cl->tp_pos, pos, sizeof cl->tp_pos);
    cl->tp = TP_MAPLOOKUP;
    cl->tp_deadline = now_s() + 20;

    Buf b;
    agent_block(cl, b);
    put_u32(b, 2);                                      /* flags: layer */
    put_u32(b, 0);                                      /* estate id (sim fills) */
    put_u8(b, 0);                                       /* godlike */
    put_str1(b, region);
    send_msg(&cl->cur, MSG_MapNameRequest, b, true);
    say_line("* looking up region '%s'...", clean(region).c_str());
}

/* TeleportFinish arrives on the event queue: open a circuit to the new sim */
static void tp_finish_event(Client* cl, const Val* info)
{
    const Val* ipv = val_get(info, "SimIP");
    std::string seed = val_str(val_get(info, "SeedCapability"));
    long port = (long)val_int(val_get(info, "SimPort"));
    if (!ipv || ipv->s.size() != 4 || port <= 0 || port > 65535) {
        say_line("* malformed TeleportFinish event");
        tp_reset(cl);
        return;
    }
    struct sockaddr_in peer;
    memset(&peer, 0, sizeof peer);
    peer.sin_family = AF_INET;
    peer.sin_port   = htons((uint16_t)port);
    memcpy(&peer.sin_addr, ipv->s.data(), 4);           /* already in network order */

    circuit_close(&cl->nxt);
    if (!circuit_init(&cl->nxt, &peer)) { say_line("* socket() failed"); tp_reset(cl); return; }
    cl->nxt.seed_cap = seed;

    http_cancel(cl, &cl->seed);
    cl->eq_url.clear();                                 /* old sim's queue is dead */
    http_cancel(cl, &cl->eq);

    cl->tp = TP_ARRIVING;
    cl->tp_deadline = now_s() + 30;
    send_handshake(cl, &cl->nxt);
    say_line("* teleport: connecting to new region...");
}

static void tp_arrived(Client* cl)
{
    circuit_close(&cl->cur);
    cl->cur = cl->nxt;
    cl->nxt = Circuit();
    cl->nxt.fd = -1;
    tp_reset(cl);
    send_throttle(cl, &cl->cur);
    cl->eq_have_ack = false;
    seed_fetch(cl, cl->cur.seed_cap);
    say_line("* teleport complete");
}

/* ================================================================== */
/* event queue events                                                  */
/* ================================================================== */

static void handle_event(Client* cl, const std::string& name, const Val* body)
{
    if (name == "TeleportFinish") {
        tp_finish_event(cl, val_at(val_get(body, "Info"), 0));
    } else if (name == "CrossedRegion") {
        say_line("* crossed a region border (not supported, expect trouble)");
    } else if (name == "ChatterBoxInvitation") {
        say_line("* incoming group/conference IM invitation (not supported)");
    } else {
        VLOG(cl, "event %s", name.c_str());
    }
}

static void on_seed(Client* cl, long status, const std::string& resp)
{
    Val v;
    if (status == 200 && llsd_parse(resp, &v)) {
        std::string url = val_str(val_get(&v, "EventQueueGet"));
        if (!url.empty()) {
            cl->eq_url = url;
            cl->eq_have_ack = false;
            cl->eq_fails = 0;
            VLOG(cl, "event queue: %s", url.c_str());
            eq_poll(cl);
            return;
        }
    }
    say_line("* no event queue capability (HTTP %ld): teleport will not work", status);
}

static void on_eq(Client* cl, CURLcode rc, long status, const std::string& resp)
{
    if (cl->eq_url.empty()) return;                     /* cancelled meanwhile */
    if (rc == CURLE_OK && status == 200) {
        Val v;
        if (llsd_parse(resp, &v)) {
            const Val* id = val_get(&v, "id");
            if (id) { cl->eq_have_ack = true; cl->eq_ack = val_int(id); }
            const Val* evs = val_get(&v, "events");
            size_t n = (evs && evs->t == Val::ARR) ? evs->vals.size() : 0;
            for (size_t i = 0; i < n; i++) {
                const Val* e = &evs->vals[i];
                handle_event(cl, val_str(val_get(e, "message")), val_get(e, "body"));
            }
            cl->eq_fails = 0;
            if (!cl->eq_url.empty()) eq_poll(cl);
            return;
        }
    }
    if (status == 404 || status == 410) {
        say_line("* event queue closed by server (HTTP %ld)", status);
        cl->eq_url.clear();
        return;
    }
    if (status == 502 || status == 499 || rc == CURLE_OPERATION_TIMEDOUT) {   /* just idle */
        cl->eq_fails = 0;
        eq_poll(cl);
        return;
    }
    if (++cl->eq_fails > 8) {
        say_line("* event queue keeps failing, giving up on it");
        cl->eq_url.clear();
        return;
    }
    cl->eq_retry_at = now_s() + 2.0;
}

static void http_pump(Client* cl)
{
    if (!cl->multi) return;
    int running = 0;
    curl_multi_perform(cl->multi, &running);
    int left = 0;
    CURLMsg* m;
    while ((m = curl_multi_info_read(cl->multi, &left)) != NULL) {
        if (m->msg != CURLMSG_DONE) continue;
        CURL* easy = m->easy_handle;
        CURLcode rc = m->data.result;
        HttpReq* r = NULL;
        if (cl->seed.active && cl->seed.easy == easy) r = &cl->seed;
        else if (cl->eq.active && cl->eq.easy == easy) r = &cl->eq;
        if (!r) continue;
        long status = 0;
        curl_easy_getinfo(easy, CURLINFO_RESPONSE_CODE, &status);
        std::string resp;
        resp.swap(r->resp);
        int kind = r->kind;
        http_cancel(cl, r);                             /* removes + frees the handle */
        if (kind == HK_SEED) on_seed(cl, rc == CURLE_OK ? status : 0, resp);
        else                 on_eq(cl, rc, status, resp);
    }
}

/* ================================================================== */
/* incoming UDP messages                                               */
/* ================================================================== */

static const char* chat_verb(int type)
{
    if (type == CHAT_WHISPER) return " whispers";
    if (type == CHAT_SHOUT)   return " shouts";
    return "";
}

static void on_chat(Client* cl, Rd* r)
{
    std::string from = rd_str1(r);
    Uuid src = rd_uuid(r);
    rd_uuid(r);                                         /* owner */
    int stype = rd_u8(r), ctype = rd_u8(r);
    rd_u8(r);                                           /* audible */
    float p[3]; rd_vec3(r, p);
    std::string msg = rd_str2(r);
    if (r->err || msg.empty() || ctype == CHAT_START || ctype == CHAT_STOP) return;
    if (stype == 1) known_add(cl, src, from);
    if (msg.compare(0, 4, "/me ") == 0 && stype != 2)
        say_line("* %s %s", clean(from).c_str(), clean(msg.substr(4)).c_str());
    else
        say_line("[chat] %s%s%s: %s", stype == 2 ? "(object) " : "", clean(from).c_str(),
                 chat_verb(ctype), clean(msg).c_str());
}

static void on_im(Client* cl, Rd* r)
{
    Uuid from = rd_uuid(r);
    rd_uuid(r);                                         /* session id */
    rd_u8(r);                                           /* from group */
    rd_uuid(r);                                         /* to agent */
    rd_u32(r);                                          /* parent estate */
    rd_uuid(r);                                         /* region id */
    float p[3]; rd_vec3(r, p);
    rd_u8(r);                                           /* offline */
    int dialog = rd_u8(r);
    Uuid id = rd_uuid(r);
    rd_u32(r);                                          /* timestamp */
    std::string name = rd_str1(r);
    std::string msg  = rd_str2(r);
    if (r->err) return;

    switch (dialog) {
    case IM_NOTHING_SPECIAL:
        known_add(cl, from, name);
        cl->last_im_from = from; cl->have_last_im = true;
        say_line("[IM] %s: %s", clean(name).c_str(), clean(msg).c_str());
        break;
    case IM_TYPING_START: case IM_TYPING_STOP:
        break;
    case IM_LURE_USER:
        known_add(cl, from, name);
        cl->last_lure = id; cl->have_lure = true;
        say_line("[teleport offer] %s: %s   (type /accept to go)", clean(name).c_str(), clean(msg).c_str());
        break;
    case IM_FROM_TASK: case IM_FROM_TASK_AS_ALERT:
        say_line("[object IM] %s: %s", clean(name).c_str(), clean(msg).c_str());
        break;
    case IM_MESSAGEBOX:
        say_line("[notice] %s", clean(msg).c_str());
        break;
    case IM_FRIENDSHIP_OFFERED:
        known_add(cl, from, name);
        say_line("[friend request] %s: %s (accepting is not supported)", clean(name).c_str(), clean(msg).c_str());
        break;
    case 17:                                            /* IM_SESSION_SEND: group / conference */
        say_line("[group/conf] %s: %s", clean(name).c_str(), clean(msg).c_str());
        break;
    default:
        VLOG(cl, "IM dialog %d from %s: %s", dialog, clean(name).c_str(), clean(msg).c_str());
    }
}

static void on_map_block(Client* cl, Rd* r)
{
    rd_uuid(r); rd_u32(r);
    int n = rd_u8(r);
    bool found = false;
    uint32_t fx = 0, fy = 0;
    std::string fname;
    for (int i = 0; i < n && !r->err; i++) {
        uint32_t x = rd_u16(r), y = rd_u16(r);
        std::string name = rd_str1(r);
        rd_u8(r); rd_u32(r); rd_u8(r); rd_u8(r); rd_uuid(r);
        if (r->err || name.empty()) continue;
        bool exact = lower(name) == lower(cl->tp_name);
        if (exact || !found) { found = true; fx = x; fy = y; fname = name; }
        if (exact) break;
    }
    if (cl->tp != TP_MAPLOOKUP) return;
    if (!found) { say_line("* region '%s' not found", clean(cl->tp_name).c_str()); tp_reset(cl); return; }
    uint64_t handle = ((uint64_t)(fx * 256u) << 32) | (uint64_t)(fy * 256u);
    say_line("* teleporting to %s (%u,%u)...", clean(fname).c_str(), fx, fy);
    send_tp_request(cl, handle);
}

static void handle_message(Client* cl, Circuit* c, uint32_t key, Rd* r)
{
    switch (key) {
    case MSG_PacketAck: {
        int n = rd_u8(r);
        for (int i = 0; i < n; i++) { uint32_t id = rd_u32(r); if (r->err) break; ack_remove(c, id); }
        break; }
    case MSG_StartPingCheck: {
        Buf b; put_u8(b, rd_u8(r));
        send_msg(c, MSG_CompletePingCheck, b, false);
        break; }
    case MSG_RegionHandshake: {
        rd_u32(r); rd_u8(r);
        std::string name = rd_str1(r);
        if (!r->err) { c->sim_name = name; say_line("* region: %s", clean(name).c_str()); }
        Buf b; agent_block(cl, b); put_u32(b, 0);       /* RegionInfo.Flags */
        send_msg(c, MSG_RegionHandshakeReply, b, true);
        break; }
    case MSG_AgentMovementComplete: {
        rd_uuid(r); rd_uuid(r);
        float pos[3], look[3];
        rd_vec3(r, pos); rd_vec3(r, look);
        uint64_t handle = rd_u64(r);
        if (r->err) break;
        c->handle = handle;
        memcpy(cl->pos, pos, sizeof pos);
        say_line("* in world at <%.0f, %.0f, %.0f>", pos[0], pos[1], pos[2]);
        if (c == &cl->nxt) tp_arrived(cl);
        else               send_throttle(cl, c);
        break; }
    case MSG_ChatFromSimulator:      on_chat(cl, r); break;
    case MSG_ImprovedInstantMessage: on_im(cl, r);   break;
    case MSG_MapBlockReply:          on_map_block(cl, r); break;
    case MSG_TeleportStart:
        say_line("* teleport started");
        break;
    case MSG_TeleportProgress: {
        rd_uuid(r); rd_u32(r);
        std::string m = rd_str1(r);
        if (!r->err) say_line("* teleport: %s", clean(m).c_str());
        break; }
    case MSG_TeleportFailed: {
        rd_uuid(r);
        std::string m = rd_str1(r);
        say_line("* teleport failed: %s", clean(m).c_str());
        tp_reset(cl);
        break; }
    case MSG_TeleportLocal: {
        rd_uuid(r); rd_u32(r);
        float pos[3]; rd_vec3(r, pos);
        if (!r->err) memcpy(cl->pos, pos, sizeof pos);
        say_line("* teleported within the region to <%.0f, %.0f, %.0f>", cl->pos[0], cl->pos[1], cl->pos[2]);
        tp_reset(cl);
        break; }
    case MSG_KickUser: {
        rd_u32(r); rd_u16(r); rd_uuid(r); rd_uuid(r);
        std::string m = rd_str2(r);
        say_line("* kicked by the simulator: %s", clean(m).c_str());
        cl->done = true;
        break; }
    case MSG_LogoutReply:
        cl->done = true;
        break;
    default:
        VLOG(cl, "unhandled message %08x", key);
    }
}

static void process_packet(Client* cl, Circuit* c, const uint8_t* p, size_t n)
{
    if (n < 7) return;
    uint8_t  flags = p[0];
    uint32_t seq   = ((uint32_t)p[1] << 24) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 8) | p[4];
    size_t   hdr   = 6 + p[5];
    if (hdr >= n) return;
    size_t end = n;

    if (flags & PF_ACK) {                               /* appended acks: big endian, count last */
        size_t na = p[end - 1];
        if (end < hdr + 1 + 4 * na) return;
        size_t base = end - 1 - 4 * na;
        for (size_t i = 0; i < na; i++) {
            const uint8_t* a = p + base + 4 * i;
            ack_remove(c, ((uint32_t)a[0] << 24) | ((uint32_t)a[1] << 16) | ((uint32_t)a[2] << 8) | a[3]);
        }
        end = base;
    }
    if (flags & PF_RELIABLE) {
        c->to_ack.push_back(seq);
        bool dup = false;
        for (int i = 0; i < 256; i++) if (c->seen[i] == seq) { dup = true; break; }
        if (dup) return;
        c->seen[c->seen_n++ & 255] = seq;
    }

    Buf tmp;
    const uint8_t* body;
    size_t blen;
    if (flags & PF_ZERO) {
        if (!zero_decode(p + hdr, end - hdr, &tmp) || tmp.empty()) return;
        body = &tmp[0]; blen = tmp.size();
    } else {
        body = p + hdr; blen = end - hdr;
    }
    if (blen < 1) return;

    uint32_t key;
    size_t off;
    if (body[0] != 0xFF)      { key = MK_HIGH(body[0]); off = 1; }
    else if (blen < 2)        return;
    else if (body[1] != 0xFF) { key = MK_MED(body[1]);  off = 2; }
    else if (blen < 4)        return;
    else if (body[2] != 0xFF) { key = MK_LOW(((uint32_t)body[2] << 8) | body[3]); off = 4; }
    else                      { key = MK_FIXED(body[3]); off = 4; }

    Rd r;
    rd_init(&r, body + off, blen - off);
    handle_message(cl, c, key, &r);
}

void client_recv(Client* cl, Circuit* c)
{
    if (!c->open) return;
    uint8_t buf[4096];
    for (int i = 0; i < 500; i++) {
        struct sockaddr_in from;
        socklen_t fl = sizeof from;
        ssize_t n = recvfrom(c->fd, buf, sizeof buf, 0, (struct sockaddr*)&from, &fl);
        if (n < 0) break;
        if (from.sin_addr.s_addr != c->peer.sin_addr.s_addr || from.sin_port != c->peer.sin_port) continue;
        c->last_rx = now_s();
        process_packet(cl, c, buf, (size_t)n);
        if (!c->open) break;                            /* handler may have swapped circuits */
    }
}

/* ================================================================== */
/* connect / tick / logout                                             */
/* ================================================================== */

void client_connect(Client* cl)
{
    struct sockaddr_in peer = cl->cur.peer;
    std::string seed = cl->cur.seed_cap;
    uint64_t handle = cl->cur.handle;
    if (!circuit_init(&cl->cur, &peer)) { say_line("* socket() failed"); cl->done = true; return; }
    cl->cur.seed_cap = seed;
    cl->cur.handle = handle;
    cl->nxt.fd = -1;
    cl->pos[0] = 128; cl->pos[1] = 128; cl->pos[2] = 30;
    cl->multi = curl_multi_init();
    cl->next_agent_update = now_s() + 0.5;
    send_handshake(cl, &cl->cur);
    seed_fetch(cl, seed);
}

void client_tick(Client* cl)
{
    double t = now_s();
    http_pump(cl);

    if (cl->eq_retry_at > 0 && t >= cl->eq_retry_at) {
        cl->eq_retry_at = 0;
        eq_poll(cl);
    }

    flush_acks(&cl->cur);
    flush_acks(&cl->nxt);

    if (t >= cl->next_resend) {
        cl->next_resend = t + 0.5;
        resend_due(cl, &cl->cur, t);
        resend_due(cl, &cl->nxt, t);
    }

    if (t >= cl->next_agent_update && !cl->logging_out) {
        cl->next_agent_update = t + 0.5;
        if (cl->tp == TP_ARRIVING) send_agent_update(cl, &cl->nxt);
        else                       send_agent_update(cl, &cl->cur);
    }

    if (cl->tp != TP_IDLE && cl->tp_deadline > 0 && t > cl->tp_deadline) {
        say_line("* teleport timed out");
        if (cl->tp == TP_ARRIVING) circuit_close(&cl->nxt);
        tp_reset(cl);
    }

    if (cl->logging_out && t >= cl->logout_deadline) cl->done = true;

    if (!cl->logging_out && cl->cur.open && t - cl->cur.last_rx > 60.0 && cl->tp != TP_ARRIVING) {
        say_line("* no packets from the simulator for 60s, giving up");
        cl->done = true;
    }
}

void client_logout(Client* cl)
{
    if (cl->logging_out) return;
    cl->logging_out = true;
    cl->logout_deadline = now_s() + 5.0;
    if (!cl->cur.open) { cl->done = true; return; }
    Buf b;
    agent_block(cl, b);
    send_msg(&cl->cur, MSG_LogoutRequest, b, true);
    say_line("* logging out...");
}

void client_shutdown(Client* cl)
{
    if (cl->multi) {
        http_cancel(cl, &cl->seed);
        http_cancel(cl, &cl->eq);
        curl_multi_cleanup(cl->multi);
        cl->multi = NULL;
    }
    circuit_close(&cl->cur);
    circuit_close(&cl->nxt);
}

/* ================================================================== */
/* console commands                                                    */
/* ================================================================== */

static void split_word(const std::string& s, std::string* word, std::string* rest)
{
    size_t a = 0;
    while (a < s.size() && isspace((unsigned char)s[a])) a++;
    size_t b = a;
    while (b < s.size() && !isspace((unsigned char)s[b])) b++;
    *word = s.substr(a, b - a);
    *rest = trim(s.substr(b));
}

/* target of /im: uuid, "quoted name", or a word matched against names we have seen */
static bool resolve_target(Client* cl, std::string* args, Uuid* out)
{
    std::string a = trim(*args), who, rest;
    if (a.empty()) return false;
    if (a[0] == '"') {
        size_t q = a.find('"', 1);
        if (q == std::string::npos) return false;
        who = a.substr(1, q - 1);
        rest = trim(a.substr(q + 1));
    } else split_word(a, &who, &rest);
    *args = rest;

    if (uuid_parse(who.c_str(), out)) return true;

    std::string w = lower(who);
    int hits = 0;
    for (int pass = 0; pass < 3 && hits == 0; pass++)   /* exact, prefix, substring */
        for (size_t i = 0; i < cl->known.size(); i++) {
            std::string n = lower(cl->known[i].name);
            bool ok = pass == 0 ? n == w : pass == 1 ? n.compare(0, w.size(), w) == 0 : n.find(w) != std::string::npos;
            if (ok) { *out = cl->known[i].id; hits++; }
        }
    if (hits == 0) { say_line("* don't know anyone matching '%s' (use their UUID)", clean(who).c_str()); return false; }
    if (hits > 1)  { say_line("* '%s' is ambiguous, be more specific or use /who", clean(who).c_str()); return false; }
    return true;
}

static bool is_number(const std::string& s)
{
    if (s.empty()) return false;
    char* e;
    strtod(s.c_str(), &e);
    return *e == 0;
}

/* "Region", "Region x y z", "Region/x/y/z" (spaces allowed in the region name) */
static std::string parse_tp_args(std::string a, float pos[3])
{
    pos[0] = 128; pos[1] = 128; pos[2] = 30;
    for (size_t i; (i = a.find("%20")) != std::string::npos; ) a.replace(i, 3, " ");
    if (a.find('/') != std::string::npos) {
        std::vector<std::string> parts;
        size_t s = 0;
        for (;;) {
            size_t i = a.find('/', s);
            parts.push_back(trim(a.substr(s, i == std::string::npos ? i : i - s)));
            if (i == std::string::npos) break;
            s = i + 1;
        }
        for (size_t k = 1; k < parts.size() && k <= 3; k++) pos[k - 1] = (float)atof(parts[k].c_str());
        return parts[0];
    }
    std::vector<std::string> tok;
    size_t s = 0;
    while (s < a.size()) {
        while (s < a.size() && isspace((unsigned char)a[s])) s++;
        size_t e = s;
        while (e < a.size() && !isspace((unsigned char)a[e])) e++;
        if (e > s) tok.push_back(a.substr(s, e - s));
        s = e;
    }
    size_t nnum = 0;
    while (nnum < 3 && nnum + 1 < tok.size() && is_number(tok[tok.size() - 1 - nnum])) nnum++;
    if (nnum == 1) nnum = 0;                            /* a lone number is part of the name */
    for (size_t k = 0; k < nnum; k++) pos[k] = (float)atof(tok[tok.size() - nnum + k].c_str());
    std::string name;
    for (size_t k = 0; k + nnum < tok.size(); k++) { if (k) name += ' '; name += tok[k]; }
    return name;
}

static void print_help()
{
    say_line("plain text        say in local chat");
    say_line("/shout /whisper   local chat, louder or quieter");
    say_line("/im <who> <text>  who = uuid, \"quoted name\" or a name seen in chat/IM");
    say_line("/r <text>         reply to the last IM sender");
    say_line("/tp Region [x y z]   also Region/x/y/z");
    say_line("/accept           accept the last teleport offer");
    say_line("/home             teleport home");
    say_line("/who              people seen so far");
    say_line("/where            current region and position");
    say_line("/quit             log out");
}

void client_command(Client* cl, const std::string& raw)
{
    std::string line = trim(raw);
    if (line.empty()) return;
    if (line[0] != '/') { send_chat(cl, line, CHAT_NORMAL); return; }

    std::string cmd, arg;
    split_word(line, &cmd, &arg);
    cmd = lower(cmd);

    if (cmd == "/say" || cmd == "/s")  { if (!arg.empty()) send_chat(cl, arg, CHAT_NORMAL); }
    else if (cmd == "/shout")          { if (!arg.empty()) send_chat(cl, arg, CHAT_SHOUT); }
    else if (cmd == "/whisper")        { if (!arg.empty()) send_chat(cl, arg, CHAT_WHISPER); }
    else if (cmd == "/im") {
        Uuid to;
        if (!resolve_target(cl, &arg, &to)) return;
        if (arg.empty()) { say_line("* usage: /im <who> <text>"); return; }
        send_im(cl, to, arg);
        say_line("[IM to %s] %s", uuid_str(to).c_str(), clean(arg).c_str());
    }
    else if (cmd == "/r") {
        if (!cl->have_last_im) { say_line("* nobody to reply to yet"); return; }
        if (arg.empty()) { say_line("* usage: /r <text>"); return; }
        send_im(cl, cl->last_im_from, arg);
        say_line("[IM to %s] %s", uuid_str(cl->last_im_from).c_str(), clean(arg).c_str());
    }
    else if (cmd == "/tp") {
        float pos[3];
        std::string name = parse_tp_args(arg, pos);
        if (name.empty()) { say_line("* usage: /tp Region [x y z]"); return; }
        start_teleport(cl, name, pos);
    }
    else if (cmd == "/accept") {
        if (!cl->have_lure) { say_line("* no pending teleport offer"); return; }
        if (cl->tp != TP_IDLE) { say_line("* a teleport is already in progress"); return; }
        Buf b;
        put_uuid(b, cl->agent_id); put_uuid(b, cl->session_id);
        put_uuid(b, cl->last_lure);
        put_u32(b, TELEPORT_VIA_LURE);
        send_msg(&cl->cur, MSG_TeleportLureRequest, b, true);
        cl->have_lure = false;
        cl->tp = TP_REQUESTED;
        cl->tp_deadline = now_s() + 60;
        say_line("* accepting teleport offer...");
    }
    else if (cmd == "/home") {
        if (cl->tp != TP_IDLE) { say_line("* a teleport is already in progress"); return; }
        Buf b;
        agent_block(cl, b);
        put_uuid(b, uuid_zero());                       /* null landmark = home */
        send_msg(&cl->cur, MSG_TeleportLandmarkRequest, b, true);
        cl->tp = TP_REQUESTED;
        cl->tp_deadline = now_s() + 60;
        say_line("* teleporting home...");
    }
    else if (cmd == "/who") {
        if (cl->known.empty()) say_line("* nobody seen yet");
        for (size_t i = 0; i < cl->known.size(); i++)
            say_line("  %s  %s", uuid_str(cl->known[i].id).c_str(), clean(cl->known[i].name).c_str());
    }
    else if (cmd == "/where") {
        say_line("* %s <%.0f, %.0f, %.0f>", clean(cl->cur.sim_name).c_str(), cl->pos[0], cl->pos[1], cl->pos[2]);
    }
    else if (cmd == "/quit" || cmd == "/exit") client_logout(cl);
    else if (cmd == "/help" || cmd == "/?") print_help();
    else say_line("* unknown command %s (try /help)", cmd.c_str());
}
