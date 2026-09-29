/* util.cpp - md5, base64, uuid, wire buffers, zerocoding, XML/LLSD/XML-RPC */
#include "sl.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <algorithm>

/* ================================================================== */
/* MD5 (RFC 1321)                                                      */
/* ================================================================== */

static uint32_t rol32(uint32_t x, int c) { return (x << c) | (x >> (32 - c)); }

std::string md5_hex(const void* data, size_t len)
{
    static const int S[64] = {
        7,12,17,22, 7,12,17,22, 7,12,17,22, 7,12,17,22,
        5, 9,14,20, 5, 9,14,20, 5, 9,14,20, 5, 9,14,20,
        4,11,16,23, 4,11,16,23, 4,11,16,23, 4,11,16,23,
        6,10,15,21, 6,10,15,21, 6,10,15,21, 6,10,15,21 };
    uint32_t K[64];
    for (int i = 0; i < 64; i++)
        K[i] = (uint32_t)(4294967296.0 * fabs(sin((double)(i + 1))));

    uint32_t a0 = 0x67452301u, b0 = 0xefcdab89u, c0 = 0x98badcfeu, d0 = 0x10325476u;

    Buf m((const uint8_t*)data, (const uint8_t*)data + len);
    m.push_back(0x80);
    while (m.size() % 64 != 56) m.push_back(0);
    uint64_t bits = (uint64_t)len * 8;
    for (int i = 0; i < 8; i++) m.push_back((uint8_t)(bits >> (8 * i)));

    for (size_t off = 0; off < m.size(); off += 64) {
        uint32_t M[16];
        for (int j = 0; j < 16; j++)
            M[j] = (uint32_t)m[off + 4*j]         | ((uint32_t)m[off + 4*j + 1] << 8) |
                   ((uint32_t)m[off + 4*j + 2] << 16) | ((uint32_t)m[off + 4*j + 3] << 24);
        uint32_t A = a0, B = b0, C = c0, D = d0;
        for (int i = 0; i < 64; i++) {
            uint32_t F; int g;
            if (i < 16)      { F = (B & C) | (~B & D); g = i; }
            else if (i < 32) { F = (D & B) | (~D & C); g = (5 * i + 1) % 16; }
            else if (i < 48) { F = B ^ C ^ D;          g = (3 * i + 5) % 16; }
            else             { F = C ^ (B | ~D);       g = (7 * i) % 16; }
            F = F + A + K[i] + M[g];
            A = D; D = C; C = B;
            B = B + rol32(F, S[i]);
        }
        a0 += A; b0 += B; c0 += C; d0 += D;
    }

    uint32_t w[4] = { a0, b0, c0, d0 };
    char out[33];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            sprintf(out + i * 8 + j * 2, "%02x", (unsigned)((w[i] >> (8 * j)) & 0xff));
    return std::string(out, 32);
}

/* ================================================================== */
/* base64                                                              */
/* ================================================================== */

std::string b64_decode(const std::string& s)
{
    std::string out;
    uint32_t acc = 0;
    int bits = 0;
    for (size_t i = 0; i < s.size(); i++) {
        int c = (unsigned char)s[i], v;
        if      (c >= 'A' && c <= 'Z') v = c - 'A';
        else if (c >= 'a' && c <= 'z') v = c - 'a' + 26;
        else if (c >= '0' && c <= '9') v = c - '0' + 52;
        else if (c == '+') v = 62;
        else if (c == '/') v = 63;
        else continue;                       /* '=' and whitespace */
        acc = (acc << 6) | (uint32_t)v;
        bits += 6;
        if (bits >= 8) {
            bits -= 8;
            out.push_back((char)((acc >> bits) & 0xff));
        }
    }
    return out;
}

/* ================================================================== */
/* uuid                                                                */
/* ================================================================== */

Uuid uuid_zero() { Uuid u; memset(u.b, 0, 16); return u; }

bool uuid_is_zero(const Uuid& u)
{
    for (int i = 0; i < 16; i++) if (u.b[i]) return false;
    return true;
}

bool uuid_eq(const Uuid& a, const Uuid& b) { return memcmp(a.b, b.b, 16) == 0; }

Uuid uuid_xor(const Uuid& a, const Uuid& b)
{
    Uuid r;
    for (int i = 0; i < 16; i++) r.b[i] = a.b[i] ^ b.b[i];
    return r;
}

static int hexval(int c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

bool uuid_parse(const char* s, Uuid* out)
{
    int n = 0;
    Uuid u = uuid_zero();
    for (; *s; s++) {
        if (*s == '-') continue;
        int v = hexval((unsigned char)*s);
        if (v < 0 || n >= 32) return false;
        if (n % 2 == 0) u.b[n / 2] = (uint8_t)(v << 4);
        else            u.b[n / 2] |= (uint8_t)v;
        n++;
    }
    if (n != 32) return false;
    *out = u;
    return true;
}

std::string uuid_str(const Uuid& u)
{
    char t[40];
    snprintf(t, sizeof t,
             "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
             u.b[0], u.b[1], u.b[2], u.b[3], u.b[4], u.b[5], u.b[6], u.b[7],
             u.b[8], u.b[9], u.b[10], u.b[11], u.b[12], u.b[13], u.b[14], u.b[15]);
    return t;
}

/* ================================================================== */
/* buffer writers / readers                                            */
/* ================================================================== */

void put_u8 (Buf& b, uint8_t v)  { b.push_back(v); }
void put_u16(Buf& b, uint16_t v) { b.push_back(v & 0xff); b.push_back(v >> 8); }
void put_u32(Buf& b, uint32_t v) { for (int i = 0; i < 4; i++) b.push_back((v >> (8 * i)) & 0xff); }
void put_u64(Buf& b, uint64_t v) { for (int i = 0; i < 8; i++) b.push_back((uint8_t)(v >> (8 * i))); }

void put_f32(Buf& b, float v)
{
    uint32_t u;
    memcpy(&u, &v, 4);
    put_u32(b, u);
}

void put_uuid(Buf& b, const Uuid& u) { b.insert(b.end(), u.b, u.b + 16); }

void put_vec3(Buf& b, float x, float y, float z)
{
    put_f32(b, x); put_f32(b, y); put_f32(b, z);
}

void put_str1(Buf& b, const std::string& s)
{
    size_t n = s.size();
    if (n > 254) n = 254;
    put_u8(b, (uint8_t)(n + 1));
    b.insert(b.end(), s.begin(), s.begin() + n);
    b.push_back(0);
}

void put_str2(Buf& b, const std::string& s)
{
    size_t n = s.size();
    if (n > 65534) n = 65534;
    put_u16(b, (uint16_t)(n + 1));
    b.insert(b.end(), s.begin(), s.begin() + n);
    b.push_back(0);
}

void rd_init(Rd* r, const uint8_t* p, size_t n) { r->p = p; r->n = n; r->pos = 0; r->err = false; }

static bool rd_need(Rd* r, size_t k)
{
    if (r->err || r->n - r->pos < k) { r->err = true; return false; }
    return true;
}

void rd_skip(Rd* r, size_t k) { if (rd_need(r, k)) r->pos += k; }

uint8_t rd_u8(Rd* r)
{
    if (!rd_need(r, 1)) return 0;
    return r->p[r->pos++];
}

uint16_t rd_u16(Rd* r)
{
    if (!rd_need(r, 2)) return 0;
    uint16_t v = (uint16_t)(r->p[r->pos] | (r->p[r->pos + 1] << 8));
    r->pos += 2;
    return v;
}

uint32_t rd_u32(Rd* r)
{
    if (!rd_need(r, 4)) return 0;
    uint32_t v = 0;
    for (int i = 0; i < 4; i++) v |= (uint32_t)r->p[r->pos + i] << (8 * i);
    r->pos += 4;
    return v;
}

uint64_t rd_u64(Rd* r)
{
    if (!rd_need(r, 8)) return 0;
    uint64_t v = 0;
    for (int i = 0; i < 8; i++) v |= (uint64_t)r->p[r->pos + i] << (8 * i);
    r->pos += 8;
    return v;
}

float rd_f32(Rd* r)
{
    uint32_t u = rd_u32(r);
    float f;
    memcpy(&f, &u, 4);
    return f;
}

Uuid rd_uuid(Rd* r)
{
    Uuid u = uuid_zero();
    if (rd_need(r, 16)) { memcpy(u.b, r->p + r->pos, 16); r->pos += 16; }
    return u;
}

void rd_vec3(Rd* r, float v[3]) { v[0] = rd_f32(r); v[1] = rd_f32(r); v[2] = rd_f32(r); }

static std::string rd_bytes_str(Rd* r, size_t n)
{
    std::string s;
    if (rd_need(r, n)) {
        s.assign((const char*)r->p + r->pos, n);
        r->pos += n;
    }
    while (!s.empty() && s[s.size() - 1] == 0) s.erase(s.size() - 1);
    return s;
}

std::string rd_str1(Rd* r) { size_t n = rd_u8(r);  return rd_bytes_str(r, n); }
std::string rd_str2(Rd* r) { size_t n = rd_u16(r); return rd_bytes_str(r, n); }

/* ================================================================== */
/* zerocoding: a run of N zero bytes (1..255) becomes 00 N             */
/* ================================================================== */

bool zero_decode(const uint8_t* src, size_t n, Buf* out)
{
    out->clear();
    for (size_t i = 0; i < n; i++) {
        if (src[i] != 0) { out->push_back(src[i]); continue; }
        if (i + 1 >= n) return false;
        out->insert(out->end(), src[i + 1], 0);
        i++;
        if (out->size() > 65536) return false;
    }
    return true;
}

void zero_encode(const uint8_t* src, size_t n, Buf* out)
{
    out->clear();
    for (size_t i = 0; i < n; ) {
        if (src[i] != 0) { out->push_back(src[i++]); continue; }
        size_t run = 0;
        while (i < n && src[i] == 0 && run < 255) { run++; i++; }
        out->push_back(0);
        out->push_back((uint8_t)run);
    }
}

/* ================================================================== */
/* tiny XML parser -> XNode tree                                       */
/* ================================================================== */

std::string xml_escape(const std::string& s)
{
    std::string o;
    for (size_t i = 0; i < s.size(); i++) {
        switch (s[i]) {
        case '&': o += "&amp;";  break;
        case '<': o += "&lt;";   break;
        case '>': o += "&gt;";   break;
        case '"': o += "&quot;"; break;
        default:  o += s[i];
        }
    }
    return o;
}

struct XP { const char* p; const char* e; };

static void utf8_append(std::string& o, unsigned long cp)
{
    if (cp < 0x80) o += (char)cp;
    else if (cp < 0x800) { o += (char)(0xC0 | (cp >> 6)); o += (char)(0x80 | (cp & 0x3F)); }
    else if (cp < 0x10000) {
        o += (char)(0xE0 | (cp >> 12)); o += (char)(0x80 | ((cp >> 6) & 0x3F));
        o += (char)(0x80 | (cp & 0x3F));
    } else {
        o += (char)(0xF0 | (cp >> 18)); o += (char)(0x80 | ((cp >> 12) & 0x3F));
        o += (char)(0x80 | ((cp >> 6) & 0x3F)); o += (char)(0x80 | (cp & 0x3F));
    }
}

static void xml_append_text(std::string& out, const char* s, const char* e)
{
    while (s < e) {
        if (*s != '&') { out += *s++; continue; }
        const char* semi = (const char*)memchr(s, ';', (size_t)(e - s));
        if (!semi || semi - s > 10) { out += *s++; continue; }
        std::string ent(s + 1, semi);
        if      (ent == "amp")  out += '&';
        else if (ent == "lt")   out += '<';
        else if (ent == "gt")   out += '>';
        else if (ent == "quot") out += '"';
        else if (ent == "apos") out += '\'';
        else if (!ent.empty() && ent[0] == '#') {
            unsigned long cp = (ent.size() > 1 && (ent[1] == 'x' || ent[1] == 'X'))
                             ? strtoul(ent.c_str() + 2, NULL, 16)
                             : strtoul(ent.c_str() + 1, NULL, 10);
            utf8_append(out, cp);
        } else { out.append(s, semi + 1); }
        s = semi + 1;
    }
}

static const char* xfind(const char* p, const char* e, const char* pat)
{
    const char* pe = pat + strlen(pat);
    const char* q  = std::search(p, e, pat, pe);
    return q == e ? NULL : q;
}

static bool xml_content(XP& x, XNode& node, int depth)
{
    if (depth > 64) return false;
    while (x.p < x.e) {
        if (*x.p != '<') {
            const char* s = x.p;
            while (x.p < x.e && *x.p != '<') x.p++;
            xml_append_text(node.text, s, x.p);
            continue;
        }
        if (x.e - x.p >= 4 && !memcmp(x.p, "<!--", 4)) {
            const char* q = xfind(x.p + 4, x.e, "-->");
            if (!q) return false;
            x.p = q + 3;
            continue;
        }
        if (x.e - x.p >= 9 && !memcmp(x.p, "<![CDATA[", 9)) {
            const char* q = xfind(x.p + 9, x.e, "]]>");
            if (!q) return false;
            node.text.append(x.p + 9, q);
            x.p = q + 3;
            continue;
        }
        if (x.e - x.p >= 2 && x.p[1] == '?') {
            const char* q = xfind(x.p + 2, x.e, "?>");
            if (!q) return false;
            x.p = q + 2;
            continue;
        }
        if (x.e - x.p >= 2 && x.p[1] == '!') {           /* doctype etc. */
            const char* q = (const char*)memchr(x.p, '>', (size_t)(x.e - x.p));
            if (!q) return false;
            x.p = q + 1;
            continue;
        }
        if (x.e - x.p >= 2 && x.p[1] == '/') {           /* closing tag */
            const char* q = (const char*)memchr(x.p, '>', (size_t)(x.e - x.p));
            if (!q) return false;
            x.p = q + 1;
            return true;
        }
        /* opening tag */
        x.p++;
        const char* s = x.p;
        while (x.p < x.e && !isspace((unsigned char)*x.p) && *x.p != '>' && *x.p != '/') x.p++;
        XNode child;
        child.name.assign(s, x.p);
        bool selfclose = false;
        while (x.p < x.e && *x.p != '>') {
            if (*x.p == '"' || *x.p == '\'') {
                char q = *x.p++;
                while (x.p < x.e && *x.p != q) x.p++;
                if (x.p < x.e) x.p++;
                continue;
            }
            if (*x.p == '/' && x.p + 1 < x.e && x.p[1] == '>') selfclose = true;
            x.p++;
        }
        if (x.p >= x.e) return false;
        x.p++;                                           /* consume '>' */
        if (!selfclose && !xml_content(x, child, depth + 1)) return false;
        node.kids.push_back(child);
    }
    return depth == 0;
}

bool xml_parse(const std::string& s, XNode* root)
{
    XP x;
    x.p = s.data();
    x.e = s.data() + s.size();
    root->name.clear(); root->text.clear(); root->kids.clear();
    return xml_content(x, *root, 0);
}

/* ================================================================== */
/* Val helpers, LLSD and XML-RPC decoding                              */
/* ================================================================== */

const Val* val_get(const Val* m, const char* key)
{
    if (!m || m->t != Val::MAP) return NULL;
    for (size_t i = 0; i < m->keys.size(); i++)
        if (m->keys[i] == key) return &m->vals[i];
    return NULL;
}

const Val* val_at(const Val* a, size_t idx)
{
    if (!a || a->t != Val::ARR || idx >= a->vals.size()) return NULL;
    return &a->vals[idx];
}

std::string val_str(const Val* v, const char* def)
{
    if (!v) return def;
    if (v->t == Val::STR || v->t == Val::BIN) return v->s;
    if (v->t == Val::INT) { char t[32]; snprintf(t, sizeof t, "%lld", (long long)v->i); return t; }
    return def;
}

int64_t val_int(const Val* v, int64_t def)
{
    if (!v) return def;
    if (v->t == Val::INT)  return v->i;
    if (v->t == Val::BOOL) return v->b ? 1 : 0;
    if (v->t == Val::REAL) return (int64_t)v->r;
    if (v->t == Val::STR)  return strtoll(v->s.c_str(), NULL, 10);
    return def;
}

static bool llsd_node(const XNode& n, Val* v)
{
    const std::string& t = n.name;
    if      (t == "undef")   v->t = Val::UNDEF;
    else if (t == "boolean") { v->t = Val::BOOL; v->b = (n.text == "true" || n.text == "1"); }
    else if (t == "integer") { v->t = Val::INT;  v->i = strtoll(n.text.c_str(), NULL, 10); }
    else if (t == "real")    { v->t = Val::REAL; v->r = strtod(n.text.c_str(), NULL); }
    else if (t == "string" || t == "uuid" || t == "uri" || t == "date") { v->t = Val::STR; v->s = n.text; }
    else if (t == "binary")  { v->t = Val::BIN;  v->s = b64_decode(n.text); }
    else if (t == "array") {
        v->t = Val::ARR;
        for (size_t i = 0; i < n.kids.size(); i++) {
            Val c;
            if (!llsd_node(n.kids[i], &c)) return false;
            v->vals.push_back(c);
        }
    } else if (t == "map") {
        v->t = Val::MAP;
        for (size_t i = 0; i + 1 < n.kids.size(); i += 2) {
            if (n.kids[i].name != "key") return false;
            Val c;
            if (!llsd_node(n.kids[i + 1], &c)) return false;
            v->keys.push_back(n.kids[i].text);
            v->vals.push_back(c);
        }
    } else return false;
    return true;
}

bool llsd_parse(const std::string& xml, Val* out)
{
    XNode root;
    if (!xml_parse(xml, &root)) return false;
    for (size_t i = 0; i < root.kids.size(); i++)
        if (root.kids[i].name == "llsd" && !root.kids[i].kids.empty())
            return llsd_node(root.kids[i].kids[0], out);
    return false;
}

static const XNode* xn_child(const XNode& n, const char* name)
{
    for (size_t i = 0; i < n.kids.size(); i++)
        if (n.kids[i].name == name) return &n.kids[i];
    return NULL;
}

static bool xr_value(const XNode& value, Val* v, int depth)
{
    if (depth > 32) return false;
    if (value.kids.empty()) { v->t = Val::STR; v->s = value.text; return true; }
    const XNode& k = value.kids[0];
    const std::string& t = k.name;
    if      (t == "string" || t == "dateTime.iso8601") { v->t = Val::STR; v->s = k.text; }
    else if (t == "int" || t == "i4" || t == "i8") { v->t = Val::INT; v->i = strtoll(k.text.c_str(), NULL, 10); }
    else if (t == "boolean") { v->t = Val::BOOL; v->b = (k.text == "1" || k.text == "true"); }
    else if (t == "double")  { v->t = Val::REAL; v->r = strtod(k.text.c_str(), NULL); }
    else if (t == "base64")  { v->t = Val::BIN;  v->s = b64_decode(k.text); }
    else if (t == "nil")     { v->t = Val::UNDEF; }
    else if (t == "array") {
        v->t = Val::ARR;
        const XNode* data = xn_child(k, "data");
        if (data)
            for (size_t i = 0; i < data->kids.size(); i++) {
                if (data->kids[i].name != "value") continue;
                Val c;
                if (!xr_value(data->kids[i], &c, depth + 1)) return false;
                v->vals.push_back(c);
            }
    } else if (t == "struct") {
        v->t = Val::MAP;
        for (size_t i = 0; i < k.kids.size(); i++) {
            if (k.kids[i].name != "member") continue;
            const XNode* nm = xn_child(k.kids[i], "name");
            const XNode* vl = xn_child(k.kids[i], "value");
            if (!nm || !vl) continue;
            Val c;
            if (!xr_value(*vl, &c, depth + 1)) return false;
            v->keys.push_back(nm->text);
            v->vals.push_back(c);
        }
    } else { v->t = Val::STR; v->s = value.text; }
    return true;
}

bool xmlrpc_parse_response(const std::string& xml, Val* out, std::string* fault)
{
    XNode root;
    if (!xml_parse(xml, &root)) return false;
    const XNode* mr = xn_child(root, "methodResponse");
    if (!mr) return false;
    const XNode* f = xn_child(*mr, "fault");
    if (f) {
        const XNode* v = xn_child(*f, "value");
        Val fv;
        if (v && xr_value(*v, &fv, 0)) *fault = val_str(val_get(&fv, "faultString"), "fault");
        else *fault = "fault";
        return false;
    }
    const XNode* params = xn_child(*mr, "params");
    const XNode* param  = params ? xn_child(*params, "param") : NULL;
    const XNode* value  = param ? xn_child(*param, "value") : NULL;
    if (!value) return false;
    return xr_value(*value, out, 0);
}
