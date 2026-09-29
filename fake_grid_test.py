#!/usr/bin/env python3
"""End-to-end test: a fake Second Life grid (XML-RPC login, seed cap, event queue,
two UDP simulators) driven against the real ./slchat binary.

It checks the client against *my reading* of the protocol (message_template.msg
and the viewer source), so it proves the client is self-consistent and robust,
not that the real grid agrees - that still needs a run against OpenSim/aditi."""
import base64, hashlib, os, queue, socket, struct, subprocess, sys, threading, time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

AGENT   = '11111111-1111-1111-1111-111111111111'
SESSION = '22222222-2222-2222-2222-222222222222'
OTHER   = '33333333-3333-3333-3333-333333333333'
u = lambda s: bytes.fromhex(s.replace('-', ''))
str1 = lambda s: bytes([len(s.encode()) + 1]) + s.encode() + b'\0'
str2 = lambda s: struct.pack('<H', len(s.encode()) + 1) + s.encode() + b'\0'
vec3 = lambda x, y, z: struct.pack('<fff', x, y, z)

def msgno(f, n):
    if f == 'H': return bytes([n])
    if f == 'M': return b'\xff' + bytes([n])
    if f == 'L': return b'\xff\xff' + struct.pack('>H', n)
    return b'\xff\xff\xff' + bytes([n])

def zenc(b):
    out, i = bytearray(), 0
    while i < len(b):
        if b[i]:
            out.append(b[i]); i += 1
        else:
            r = 0
            while i < len(b) and b[i] == 0 and r < 255: r += 1; i += 1
            out += bytes([0, r])
    return bytes(out)

def zdec(b):
    out, i = bytearray(), 0
    while i < len(b):
        if b[i]: out.append(b[i]); i += 1
        else: out += bytes(b[i + 1]); i += 2
    return bytes(out)

def build(seq, f, n, body, reliable=False, zero=False, acks=()):
    flags = (0x40 if reliable else 0) | (0x80 if zero else 0) | (0x10 if acks else 0)
    payload = msgno(f, n) + body
    if zero: payload = zenc(payload)
    pkt = bytes([flags]) + struct.pack('>I', seq) + b'\0' + payload
    if acks: pkt += b''.join(struct.pack('>I', a) for a in acks) + bytes([len(acks)])
    return pkt

def parse(pkt):
    flags, seq, hdr, end, acks = pkt[0], struct.unpack('>I', pkt[1:5])[0], 6 + pkt[5], len(pkt), []
    if flags & 0x10:
        n = pkt[-1]; base = end - 1 - 4 * n
        acks = [struct.unpack('>I', pkt[base + 4 * i: base + 4 * i + 4])[0] for i in range(n)]
        end = base
    p = pkt[hdr:end]
    if flags & 0x80: p = zdec(p)
    if p[0] != 0xff: key, body = ('H', p[0]), p[1:]
    elif p[1] != 0xff: key, body = ('M', p[1]), p[2:]
    elif p[2] != 0xff: key, body = ('L', (p[2] << 8) | p[3]), p[4:]
    else: key, body = ('F', p[3]), p[4:]
    return dict(flags=flags, seq=seq, key=key, body=body, acks=acks)

class Grid:
    def __init__(self):
        self.events = {'A': queue.Queue(), 'B': queue.Queue()}
        self.eq_acks = {'A': [], 'B': []}
        self.eq_id = {'A': 0, 'B': 0}
        self.seed_bodies = []
        self.login_body = ''
        self.sims = {}
        self.http = ThreadingHTTPServer(('127.0.0.1', 0), self.make_handler())
        self.hport = self.http.server_address[1]
        threading.Thread(target=self.http.serve_forever, daemon=True).start()

    def make_handler(self):
        g = self
        class H(BaseHTTPRequestHandler):
            def log_message(self, *a): pass
            def do_POST(self):
                body = self.rfile.read(int(self.headers['Content-Length'])).decode()
                if self.path == '/login': return self.login(body)
                which = self.path.rsplit('/', 1)[1]
                if self.path.startswith('/seed/'):
                    g.seed_bodies.append(body)
                    return self.send(200, '<llsd><map><key>EventQueueGet</key><string>http://127.0.0.1:%d/eq/%s</string></map></llsd>' % (g.hport, which))
                if self.path.startswith('/eq/'):
                    g.eq_acks[which].append('undef' if '<undef/>' in body else body.split('<integer>')[1].split('<')[0])
                    try: ev = g.events[which].get(timeout=2.0)
                    except queue.Empty: return self.send(502, '')
                    g.eq_id[which] += 1
                    return self.send(200, '<llsd><map><key>events</key><array>%s</array><key>id</key><integer>%d</integer></map></llsd>' % (ev, g.eq_id[which]))
                self.send(404, '')
            def send(self, code, text):
                d = text.encode(); self.send_response(code)
                self.send_header('Content-Type', 'application/llsd+xml'); self.send_header('Content-Length', str(len(d)))
                self.end_headers(); self.wfile.write(d)
            def login(self, body):
                g.login_body = body
                s = lambda k, v: '<member><name>%s</name><value><string>%s</string></value></member>' % (k, v)
                i = lambda k, v: '<member><name>%s</name><value><i4>%d</i4></value></member>' % (k, v)
                self.send(200, '<?xml version="1.0"?><methodResponse><params><param><value><struct>' +
                    s('login', 'true') + s('first_name', '"Tester"') + s('last_name', 'Resident') +
                    s('agent_id', AGENT) + s('session_id', SESSION) + s('secure_session_id', OTHER) +
                    i('circuit_code', 424242) + s('sim_ip', '127.0.0.1') + i('sim_port', g.sims['A'].port) +
                    s('seed_capability', 'http://127.0.0.1:%d/seed/A' % g.hport) +
                    i('region_x', 256000) + i('region_y', 256000) + s('message', 'Welcome to the fake grid') +
                    '</struct></value></param></params></methodResponse>')
        return H

class Sim(threading.Thread):
    def __init__(self, name, grid):
        super().__init__(daemon=True)
        self.name, self.grid = name, grid
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind(('127.0.0.1', 0)); self.sock.settimeout(0.2)
        self.port = self.sock.getsockname()[1]
        self.seq, self.addr, self.got, self.resent, self.stop = 0, None, [], 0, False
        self.dropped_cam = False
        grid.sims[name] = self

    def send(self, f, n, body, reliable=False, zero=False, acks=()):
        self.seq += 1
        self.sock.sendto(build(self.seq, f, n, body, reliable, zero, acks), self.addr)

    def run(self):
        while not self.stop:
            try: pkt, addr = self.sock.recvfrom(4096)
            except socket.timeout: continue
            m = parse(pkt); self.addr = addr
            if m['flags'] & 0x20: self.resent += 1
            k, b = m['key'], m['body']
            # deliberately lose the first CompleteAgentMovement on sim A: client must resend it
            if k == ('L', 249) and self.name == 'A' and not self.dropped_cam:
                self.dropped_cam = True; continue
            self.got.append((k, b))
            if m['flags'] & 0x40:
                self.send('F', 0xFB, bytes([1]) + struct.pack('<I', m['seq']))
            if k == ('H', 1):                                    # never sent by client; ignore
                pass
            elif k == ('L', 249):                                # CompleteAgentMovement
                self.send('L', 250, u(AGENT) + u(SESSION) + vec3(128, 129, 30) + vec3(1, 0, 0) +
                          struct.pack('<QI', (1000 * 256 << 32) | (1000 * 256), 1) + str2('fake 1.0'), reliable=True)
                hs = struct.pack('<IB', 0, 13) + str1('Fake Region ' + self.name) + bytes(200)   # zero-heavy
                self.send('L', 148, hs, reliable=True, zero=True, acks=[m['seq']])
                self.send('H', 1, bytes([7]) + struct.pack('<I', 0))            # StartPingCheck
            elif k == ('L', 80):                                 # ChatFromViewer -> echo
                msg = b[32:]; ln = struct.unpack('<H', msg[:2])[0]; text = msg[2:2 + ln - 1].decode()
                chat = str1('Tester Resident') + u(AGENT) + u(AGENT) + bytes([1, 1, 1]) + vec3(1, 2, 3) + str2(text)
                self.send('L', 139, chat, reliable=True)
                if text == 'dupe-me':                            # resend same seq: client must dedupe
                    self.sock.sendto(build(self.seq, 'L', 139, chat, True), self.addr)
            elif k == ('L', 254):                                # IM from client -> answer as OTHER
                to = b[32 + 1: 32 + 17]; assert b[32] == 0
                sid = b[32 + 1 + 16 + 4 + 16 + 12 + 2: 32 + 1 + 16 + 4 + 16 + 12 + 2 + 16]
                assert sid == bytes(x ^ y for x, y in zip(u(AGENT), to)), 'bad p2p session id'
                off = 32 + 1 + 16 + 4 + 16 + 12 + 2 + 16 + 4
                nl = b[off]; off += 1 + nl; ml = struct.unpack('<H', b[off:off + 2])[0]; text = b[off + 2:off + 2 + ml - 1].decode()
                self.send('L', 254, self.im(0, 'pong: ' + text), reliable=True)
            elif k == ('L', 408):                                # MapNameRequest
                name = b[32 + 4 + 4 + 1 + 1:-1].decode()
                self.grid.map_name = name
                blk = struct.pack('<HH', 1000, 1001) + str1(name) + struct.pack('<BIBB', 13, 0, 20, 0) + bytes(16)
                self.send('L', 409, u(AGENT) + struct.pack('<I', 0) + bytes([1]) + blk, reliable=True)
            elif k == ('L', 63):                                 # TeleportLocationRequest
                self.grid.tp_req = struct.unpack('<Q', b[32:40])[0], struct.unpack('<fff', b[40:52])
                self.send('L', 73, struct.pack('<I', 0))
                self.grid.finish_teleport('B')
            elif k == ('L', 71):                                 # TeleportLureRequest
                self.grid.lure_req = (b[:16], b[32:48], struct.unpack('<I', b[48:52])[0])
                self.grid.finish_teleport('A')
            elif k == ('L', 252):                                # LogoutRequest
                self.send('L', 253, u(AGENT) + u(SESSION) + bytes([0]), reliable=True)

    def im(self, dialog, text, ident=OTHER):
        return (u(OTHER) + u(SESSION) + bytes([0]) + u(AGENT) + struct.pack('<I', 0) + bytes(16) + vec3(0, 0, 0) +
                bytes([0, dialog]) + u(ident) + struct.pack('<I', 0) + str1('Other Person') + str2(text) + str2('') + struct.pack('<I', 0) + bytes([0]))

def finish_teleport(self, target):
    sim = self.sims[target]
    sim.addr = None
    ev = ('<map><key>message</key><string>TeleportFinish</string><key>body</key><map><key>Info</key><array><map>'
          '<key>AgentID</key><uuid>%s</uuid><key>LocationID</key><binary>AAAAAQ==</binary>'
          '<key>SimIP</key><binary>%s</binary><key>SimPort</key><integer>%d</integer>'
          '<key>RegionHandle</key><binary>%s</binary><key>SeedCapability</key><string>http://127.0.0.1:%d/seed/%s</string>'
          '<key>SimAccess</key><integer>13</integer><key>TeleportFlags</key><binary>AAAAAA==</binary></map></array></map></map>' %
          (AGENT, base64.b64encode(bytes([127, 0, 0, 1])).decode(), sim.port,
           base64.b64encode(struct.pack('>Q', 5)).decode(), self.hport, target))
    self.events['A' if target == 'B' else 'B'].put(ev)
    # events are delivered on the queue of the sim the client is leaving
Grid.finish_teleport = finish_teleport

# ---------------------------------------------------------------- driver
def main():
    g = Grid(); A = Sim('A', g); B = Sim('B', g); A.start(); B.start()
    env = dict(os.environ, SL_PASSWORD='secret')
    p = subprocess.Popen([os.environ.get('SLCHAT', './slchat'), 'Tester Resident', '--login-uri', 'http://127.0.0.1:%d/login' % g.hport, '-v'],
                         stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, env=env, text=True, bufsize=1)
    out = []
    threading.Thread(target=lambda: [out.append(l.rstrip('\n')) for l in p.stdout], daemon=True).start()
    fails = []
    def wait(text, t=12, n=1):
        end = time.time() + t
        while time.time() < end:
            if sum(text in l for l in out) >= n: return True
            time.sleep(0.05)
        fails.append('timeout waiting for: %r' % text); return False
    def cmd(s): p.stdin.write(s + '\n'); p.stdin.flush()
    def check(c, what):
        if not c: fails.append('check failed: ' + what)

    wait('logged in as Tester Resident'); wait('Welcome to the fake grid')
    check('$1$' + hashlib.md5(b'secret').hexdigest() in g.login_body, 'passwd is $1$+md5')
    check('<name>first</name><value><string>Tester' in g.login_body, 'first name sent')
    wait('in world at <128, 129, 30>', 15)                       # needs the 3s resend of CompleteAgentMovement
    check(A.resent >= 1, 'client resent the un-acked reliable packet')
    wait('region: Fake Region A')                                # zerocoded RegionHandshake decoded
    check(any(k == ('L', 149) for k, _ in A.got), 'RegionHandshakeReply sent')
    check(any(k == ('H', 2) for k, _ in A.got), 'CompletePingCheck sent')
    check(any(k == ('L', 81) for k, _ in A.got), 'AgentThrottle sent')
    check(any(k == ('H', 4) for k, _ in A.got), 'AgentUpdate sent')
    check(any(k == ('F', 0xFB) for k, _ in A.got), 'PacketAck sent')
    wait('event queue: http://127.0.0.1')
    cmd('hello world'); wait('[chat] Tester Resident: hello world')
    cmd('dupe-me'); time.sleep(0.6)
    check(sum('[chat] Tester Resident: dupe-me' in l for l in out) == 1, 'duplicate reliable packet delivered once')
    A.addr and A.send('L', 139, str1('Evil\x1b[31mObj') + u(OTHER) + u(OTHER) + bytes([2, 1, 1]) + vec3(0, 0, 0) + str2('hi\x1b[2Jthere'), reliable=True)
    wait('(object) Evil?[31mObj: hi?[2Jthere')
    check(not any('\x1b' in l for l in out), 'escape bytes stripped from remote text')
    cmd('/im %s ping' % OTHER); wait('[IM] Other Person: pong: ping')
    cmd('/r again'); wait('[IM] Other Person: pong: again')
    cmd('/im other third'); wait('[IM] Other Person: pong: third')
    A.send('L', 254, A.im(22, 'Come join me', ident='44444444-4444-4444-4444-444444444444'), reliable=True)
    wait('[teleport offer] Other Person: Come join me')
    cmd('/tp Test Region 10 20 30'); wait('teleport complete', 15)
    check(getattr(g, 'map_name', '') == 'Test Region', 'MapNameRequest carried the region name')
    check(g.tp_req == ((1000 * 256 << 32) | (1001 * 256), (10.0, 20.0, 30.0)), 'TeleportLocationRequest handle/pos')
    wait('region: Fake Region B')
    check(any(k == ('L', 3) for k, _ in B.got), 'UseCircuitCode sent to new sim')
    check(any(k == ('L', 249) for k, _ in B.got), 'CompleteAgentMovement sent to new sim')
    check(len(g.seed_bodies) >= 2 and all('EventQueueGet' in b for b in g.seed_bodies), 'seed cap requested again on the new sim')
    wait('event queue: http://127.0.0.1:%d/eq/B' % g.hport)
    cmd('after tp'); wait('[chat] Tester Resident: after tp')
    check(any(k == ('L', 80) for k, _ in B.got), 'chat now goes to the new sim')
    B.send('L', 254, B.im(22, 'Back here', ident='55555555-5555-5555-5555-555555555555'), reliable=True)
    wait('[teleport offer] Other Person: Back here')
    cmd('/accept'); wait('accepting teleport offer'); time.sleep(0.5)
    wait('* teleport complete', 15, n=2)
    check(g.lure_req[0] == u(AGENT) and g.lure_req[1] == u('55555555-5555-5555-5555-555555555555') and g.lure_req[2] == 4, 'TeleportLureRequest fields')
    cmd('/where'); time.sleep(0.3)
    cmd('/quit'); wait('logging out')
    try: rc = p.wait(timeout=8)
    except subprocess.TimeoutExpired: p.kill(); rc = -1; fails.append('client did not exit after logout')
    check(rc == 0, 'exit code 0 (got %s)' % rc)
    A.stop = B.stop = True
    if '-q' not in sys.argv:
        print('\n'.join('  | ' + l for l in out))
    print('FAILED:\n  ' + '\n  '.join(fails) if fails else 'END-TO-END OK')
    return 1 if fails else 0

if __name__ == '__main__':
    sys.exit(main())
