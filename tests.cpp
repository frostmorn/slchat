/* tests.cpp - unit tests for the protocol helpers (no network needed) */
#include "sl.h"
#include <stdio.h>
#include <string.h>

static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); fails++; } } while (0)

int main()
{
    /* md5: RFC 1321 vectors */
    CHECK(md5_hex("", 0) == "d41d8cd98f00b204e9800998ecf8427e");
    CHECK(md5_hex("abc", 3) == "900150983cd24fb0d6963f7d28e17f72");
    const char* m = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
    CHECK(md5_hex(m, strlen(m)) == "57edf4a22be3c955ac49da2e2107b67a");

    /* base64 */
    CHECK(b64_decode("aGVsbG8=") == "hello");
    CHECK(b64_decode("AQIDBA==") == std::string("\x01\x02\x03\x04", 4));

    /* uuid */
    Uuid u;
    CHECK(uuid_parse("11111111-2222-3333-4444-555555555555", &u));
    CHECK(uuid_str(u) == "11111111-2222-3333-4444-555555555555");
    CHECK(!uuid_parse("nope", &u));
    Uuid a, b;
    uuid_parse("ffffffff-0000-0000-0000-000000000001", &a);
    uuid_parse("0000ffff-0000-0000-0000-000000000003", &b);
    CHECK(uuid_str(uuid_xor(a, b)) == "ffff0000-0000-0000-0000-000000000002");
    CHECK(uuid_is_zero(uuid_zero()));

    /* buffers */
    Buf w;
    put_u8(w, 7); put_u16(w, 0x1234); put_u32(w, 0xdeadbeef); put_u64(w, 0x0102030405060708ULL);
    put_f32(w, 1.5f); put_str1(w, "hi"); put_str2(w, "yo");
    CHECK(w[1] == 0x34 && w[2] == 0x12);                 /* little endian */
    Rd r; rd_init(&r, &w[0], w.size());
    CHECK(rd_u8(&r) == 7);
    CHECK(rd_u16(&r) == 0x1234);
    CHECK(rd_u32(&r) == 0xdeadbeefu);
    CHECK(rd_u64(&r) == 0x0102030405060708ULL);
    CHECK(rd_f32(&r) == 1.5f);
    CHECK(rd_str1(&r) == "hi");
    CHECK(rd_str2(&r) == "yo");
    CHECK(!r.err);
    rd_u8(&r);
    CHECK(r.err);                                        /* overrun detected */

    /* zerocode round trip */
    uint8_t raw[] = {1,0,0,0,2,0,3,0,0,0,0,0,0,0,0,0,0,9};
    Buf enc, dec;
    zero_encode(raw, sizeof raw, &enc);
    CHECK(enc.size() < sizeof raw);
    CHECK(zero_decode(&enc[0], enc.size(), &dec));
    CHECK(dec.size() == sizeof raw && memcmp(&dec[0], raw, sizeof raw) == 0);
    uint8_t big[600]; memset(big, 0, sizeof big);
    zero_encode(big, sizeof big, &enc);
    CHECK(zero_decode(&enc[0], enc.size(), &dec) && dec.size() == 600);
    uint8_t bad[] = {5, 0};
    CHECK(!zero_decode(bad, 2, &dec));

    /* LLSD: event queue reply with binary fields */
    const char* eq =
        "<?xml version=\"1.0\"?><llsd><map><key>events</key><array><map>"
        "<key>message</key><string>TeleportFinish</string><key>body</key><map>"
        "<key>Info</key><array><map><key>SimIP</key><binary encoding=\"base64\">CgAAAg==</binary>"
        "<key>SimPort</key><integer>13005</integer><key>SeedCapability</key>"
        "<string>https://sim.example/cap?a=1&amp;b=2</string></map></array></map></map></array>"
        "<key>id</key><integer>42</integer></map></llsd>";
    Val v;
    CHECK(llsd_parse(eq, &v));
    CHECK(val_int(val_get(&v, "id")) == 42);
    const Val* ev = val_at(val_get(&v, "events"), 0);
    CHECK(val_str(val_get(ev, "message")) == "TeleportFinish");
    const Val* info = val_at(val_get(val_get(ev, "body"), "Info"), 0);
    CHECK(val_get(info, "SimIP")->s == std::string("\x0a\x00\x00\x02", 4));
    CHECK(val_int(val_get(info, "SimPort")) == 13005);
    CHECK(val_str(val_get(info, "SeedCapability")) == "https://sim.example/cap?a=1&b=2");
    Val bad2;
    CHECK(!llsd_parse("<llsd><map><key>x</key>", &bad2));

    /* XML-RPC login response */
    const char* lr =
        "<?xml version=\"1.0\"?><methodResponse><params><param><value><struct>"
        "<member><name>login</name><value><string>true</string></value></member>"
        "<member><name>circuit_code</name><value><i4>123456</i4></value></member>"
        "<member><name>sim_ip</name><value>127.0.0.1</value></member>"
        "<member><name>look_at</name><value><array><data><value><double>1.5</double></value></data></array></value></member>"
        "<member><name>flag</name><value><boolean>1</boolean></value></member>"
        "</struct></value></param></params></methodResponse>";
    std::string fault;
    CHECK(xmlrpc_parse_response(lr, &v, &fault));
    CHECK(val_str(val_get(&v, "login")) == "true");
    CHECK(val_int(val_get(&v, "circuit_code")) == 123456);
    CHECK(val_str(val_get(&v, "sim_ip")) == "127.0.0.1");
    CHECK(val_get(val_at(val_get(&v, "look_at"), 0)->t == Val::REAL ? &v : NULL, "x") == NULL);
    CHECK(val_get(&v, "flag")->b);
    const char* fl =
        "<methodResponse><fault><value><struct><member><name>faultString</name>"
        "<value><string>boom</string></value></member></struct></value></fault></methodResponse>";
    CHECK(!xmlrpc_parse_response(fl, &v, &fault) && fault == "boom");

    printf(fails ? "%d FAILED\n" : "all tests passed\n", fails);
    return fails ? 1 : 0;
}
