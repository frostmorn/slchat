/* sl.h - minimal Second Life text client: shared types and prototypes.
 * C++11, C-style: plain structs, free functions, no lambdas/std::function. */
#ifndef SL_H
#define SL_H

#include <stdint.h>
#include <stddef.h>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <curl/curl.h>

/* ------------------------------------------------------------------ */
/* basic data                                                          */
/* ------------------------------------------------------------------ */

struct Uuid { uint8_t b[16]; };

Uuid        uuid_zero();
bool        uuid_parse(const char* s, Uuid* out);   /* 8-4-4-4-12 or 32 hex */
std::string uuid_str(const Uuid& u);
bool        uuid_is_zero(const Uuid& u);
bool        uuid_eq(const Uuid& a, const Uuid& b);
Uuid        uuid_xor(const Uuid& a, const Uuid& b);

std::string md5_hex(const void* data, size_t len);
std::string b64_decode(const std::string& s);

/* ------------------------------------------------------------------ */
/* wire buffers (LLUDP bodies are little endian)                       */
/* ------------------------------------------------------------------ */

typedef std::vector<uint8_t> Buf;

void put_u8  (Buf& b, uint8_t v);
void put_u16 (Buf& b, uint16_t v);
void put_u32 (Buf& b, uint32_t v);
void put_u64 (Buf& b, uint64_t v);
void put_f32 (Buf& b, float v);
void put_uuid(Buf& b, const Uuid& u);
void put_vec3(Buf& b, float x, float y, float z);
void put_str1(Buf& b, const std::string& s);   /* 1-byte length, NUL included */
void put_str2(Buf& b, const std::string& s);   /* 2-byte length, NUL included */

struct Rd { const uint8_t* p; size_t n; size_t pos; bool err; };

void        rd_init (Rd* r, const uint8_t* p, size_t n);
uint8_t     rd_u8   (Rd* r);
uint16_t    rd_u16  (Rd* r);
uint32_t    rd_u32  (Rd* r);
uint64_t    rd_u64  (Rd* r);
float       rd_f32  (Rd* r);
Uuid        rd_uuid (Rd* r);
void        rd_vec3 (Rd* r, float v[3]);
void        rd_skip (Rd* r, size_t n);
std::string rd_str1 (Rd* r);
std::string rd_str2 (Rd* r);

bool zero_decode(const uint8_t* src, size_t n, Buf* out);
void zero_encode(const uint8_t* src, size_t n, Buf* out);   /* used by tests */

/* ------------------------------------------------------------------ */
/* message numbers.  key = (frequency << 16) | number                  */
/* values checked against secondlife/master-message-template           */
/* ------------------------------------------------------------------ */

#define MK_HIGH(n)  (0x00000u | (n))
#define MK_MED(n)   (0x10000u | (n))
#define MK_LOW(n)   (0x20000u | (n))
#define MK_FIXED(n) (0x30000u | (n))

enum {
    MSG_StartPingCheck          = MK_HIGH(1),
    MSG_CompletePingCheck       = MK_HIGH(2),
    MSG_AgentUpdate             = MK_HIGH(4),
    MSG_PacketAck               = MK_FIXED(0xFB),
    MSG_UseCircuitCode          = MK_LOW(3),
    MSG_TeleportLocationRequest = MK_LOW(63),
    MSG_TeleportLocal           = MK_LOW(64),
    MSG_TeleportLandmarkRequest = MK_LOW(65),
    MSG_TeleportProgress        = MK_LOW(66),
    MSG_TeleportLureRequest     = MK_LOW(71),
    MSG_TeleportStart           = MK_LOW(73),
    MSG_TeleportFailed          = MK_LOW(74),
    MSG_ChatFromViewer          = MK_LOW(80),
    MSG_AgentThrottle           = MK_LOW(81),
    MSG_ChatFromSimulator       = MK_LOW(139),
    MSG_RegionHandshake         = MK_LOW(148),
    MSG_RegionHandshakeReply    = MK_LOW(149),
    MSG_KickUser                = MK_LOW(163),
    MSG_CompleteAgentMovement   = MK_LOW(249),
    MSG_AgentMovementComplete   = MK_LOW(250),
    MSG_LogoutRequest           = MK_LOW(252),
    MSG_LogoutReply             = MK_LOW(253),
    MSG_ImprovedInstantMessage  = MK_LOW(254),
    MSG_MapNameRequest          = MK_LOW(408),
    MSG_MapBlockReply           = MK_LOW(409)
};

/* packet header flags */
enum { PF_ZERO = 0x80, PF_RELIABLE = 0x40, PF_RESENT = 0x20, PF_ACK = 0x10 };

/* chat types */
enum { CHAT_WHISPER = 0, CHAT_NORMAL = 1, CHAT_SHOUT = 2,
       CHAT_START = 4, CHAT_STOP = 5, CHAT_DEBUG = 6 };

/* instant message dialogs (subset) */
enum { IM_NOTHING_SPECIAL = 0, IM_MESSAGEBOX = 1, IM_GROUP_INVITATION = 3,
       IM_INVENTORY_OFFERED = 4, IM_FROM_TASK = 19, IM_LURE_USER = 22,
       IM_TELEPORT_REQUEST = 26, IM_FROM_TASK_AS_ALERT = 31,
       IM_GROUP_NOTICE = 32, IM_FRIENDSHIP_OFFERED = 38,
       IM_TYPING_START = 41, IM_TYPING_STOP = 42 };

#define TELEPORT_VIA_LURE 0x04u

/* ------------------------------------------------------------------ */
/* XML / LLSD / XML-RPC                                                */
/* ------------------------------------------------------------------ */

struct XNode {
    std::string        name;
    std::string        text;
    std::vector<XNode> kids;
};

bool        xml_parse(const std::string& s, XNode* root);
std::string xml_escape(const std::string& s);

/* generic value tree, used for both LLSD and XML-RPC */
struct Val {
    enum Type { UNDEF, BOOL, INT, REAL, STR, BIN, ARR, MAP };
    Type                     t;
    bool                     b;
    int64_t                  i;
    double                   r;
    std::string              s;      /* STR (also uuid/date/uri) and BIN */
    std::vector<Val>         vals;   /* ARR items, MAP values */
    std::vector<std::string> keys;   /* MAP keys, parallel to vals */
    Val() : t(UNDEF), b(false), i(0), r(0) {}
};

const Val*  val_get(const Val* m, const char* key);       /* NULL if absent */
const Val*  val_at (const Val* a, size_t idx);
std::string val_str(const Val* v, const char* def = "");
int64_t     val_int(const Val* v, int64_t def = 0);

bool llsd_parse(const std::string& xml, Val* out);
bool xmlrpc_parse_response(const std::string& xml, Val* out, std::string* fault);

/* ------------------------------------------------------------------ */
/* client state                                                        */
/* ------------------------------------------------------------------ */

struct Pending {
    uint32_t             seq;
    std::vector<uint8_t> pkt;
    double               sent;
    int                  tries;
};

struct Circuit {
    bool                 open;
    int                  fd;
    struct sockaddr_in   peer;
    uint32_t             seq;          /* last outbound sequence used */
    std::vector<Pending> unacked;
    std::vector<uint32_t> to_ack;
    uint32_t             seen[256];    /* ring of recent reliable seqs */
    int                  seen_n;
    double               last_rx;
    std::string          seed_cap;
    std::string          sim_name;
    uint64_t             handle;
};

struct Known { Uuid id; std::string name; };

enum TpState { TP_IDLE, TP_MAPLOOKUP, TP_REQUESTED, TP_ARRIVING };

enum { HK_SEED = 1, HK_EQ = 2 };

struct HttpReq {
    CURL*        easy;
    curl_slist*  hdrs;
    std::string  body;
    std::string  resp;
    int          kind;
    bool         active;
};

struct Client {
    /* identity, from the login response */
    std::string first, last;
    Uuid        agent_id, session_id, secure_session_id;
    uint32_t    circuit_code;
    uint64_t    login_handle;
    float       pos[3];

    /* circuits: cur is the live one, nxt exists only while teleporting */
    Circuit     cur, nxt;

    /* http side */
    CURLM*      multi;
    HttpReq     seed, eq;
    std::string eq_url;
    bool        eq_have_ack;
    int64_t     eq_ack;
    int         eq_fails;
    double      eq_retry_at;

    /* timers */
    double      next_agent_update;
    double      next_resend;

    /* teleport */
    TpState     tp;
    double      tp_deadline;
    std::string tp_name;
    float       tp_pos[3];

    /* chat/IM conveniences */
    Uuid        last_im_from;   bool have_last_im;
    Uuid        last_lure;      bool have_lure;
    std::vector<Known> known;

    int         verbose;
    bool        logging_out;
    double      logout_deadline;
    bool        done;
};

/* client.cpp */
double now_s();
bool   client_login(Client* cl, const std::string& uri, const std::string& user,
                    const std::string& password, const std::string& start,
                    bool agree_tos, const std::string& mfa_token,
                    const std::string& mfa_hash, std::string* reason,
                    std::string* message, std::string* new_mfa_hash);
void   client_connect(Client* cl);
void   client_recv(Client* cl, Circuit* c);
void   client_tick(Client* cl);
void   client_command(Client* cl, const std::string& line);
void   client_logout(Client* cl);
void   client_shutdown(Client* cl);

#endif
