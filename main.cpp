/* main.cpp - slchat: text-only Second Life client (login, local chat, IM, teleport) */
#include "sl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <poll.h>
#include <termios.h>

static volatile sig_atomic_t g_sigint = 0;

static void on_sigint(int)
{
    if (g_sigint) _exit(130);                           /* second ^C: leave at once */
    g_sigint = 1;
}

static bool read_secret(const char* prompt, std::string* out)
{
    fputs(prompt, stderr);
    fflush(stderr);
    struct termios old, quiet;
    bool tty = isatty(0) && tcgetattr(0, &old) == 0;
    if (tty) { quiet = old; quiet.c_lflag &= ~(tcflag_t)ECHO; tcsetattr(0, TCSANOW, &quiet); }
    char buf[512];
    char* r = fgets(buf, sizeof buf, stdin);
    if (tty) { tcsetattr(0, TCSANOW, &old); fputc('\n', stderr); }
    if (!r) return false;
    out->assign(buf);
    while (!out->empty() && ((*out)[out->size() - 1] == '\n' || (*out)[out->size() - 1] == '\r'))
        out->erase(out->size() - 1);
    return true;
}

static void usage()
{
    fputs(
"usage: slchat \"First Last\" [options]\n"
"  --login-uri URL   login endpoint (default: main grid)\n"
"  --aditi           use the beta grid (aditi) login endpoint\n"
"  --start S         last | home | uri:Region&x&y&z      (default: last)\n"
"  --agree-tos       tell the login server you have read/accepted the current\n"
"                    Terms of Service / critical message (needed once after updates)\n"
"  --mfa-hash H      MFA hash from an earlier login, to skip the token prompt\n"
"  -v                verbose protocol log\n"
"password: taken from $SL_PASSWORD, otherwise prompted (never put it on the command line)\n",
        stderr);
}

int main(int argc, char** argv)
{
    std::string user, uri = "https://login.agni.lindenlab.com/cgi-bin/login.cgi";
    std::string start = "last", mfa_hash;
    bool agree = false;
    int verbose = 0;

    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        if      (a == "--login-uri" && i + 1 < argc) uri = argv[++i];
        else if (a == "--aditi")     uri = "https://login.aditi.lindenlab.com/cgi-bin/login.cgi";
        else if (a == "--start" && i + 1 < argc)     start = argv[++i];
        else if (a == "--mfa-hash" && i + 1 < argc)  mfa_hash = argv[++i];
        else if (a == "--agree-tos") agree = true;
        else if (a == "-v")          verbose = 1;
        else if (a == "-h" || a == "--help") { usage(); return 0; }
        else if (a[0] != '-' && user.empty()) user = a;
        else { usage(); return 2; }
    }
    if (user.empty()) { usage(); return 2; }

    std::string password;
    const char* envpw = getenv("SL_PASSWORD");
    if (envpw) password = envpw;
    else if (!read_secret("password: ", &password)) return 2;

    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, on_sigint);
    curl_global_init(CURL_GLOBAL_DEFAULT);

    Client* cl = new Client();
    cl->verbose = verbose;

    /* ---- login, with MFA retry ---- */
    std::string token;
    for (int attempt = 0; ; attempt++) {
        std::string reason, message, new_hash;
        if (client_login(cl, uri, user, password, start, agree, token, mfa_hash,
                         &reason, &message, &new_hash)) {
            if (!new_hash.empty() && new_hash != mfa_hash)
                fprintf(stderr, "note: server issued an MFA hash; pass --mfa-hash %s to skip the token next time\n",
                        new_hash.c_str());
            if (!message.empty()) printf("%s\n", message.c_str());
            break;
        }
        if (!new_hash.empty()) mfa_hash = new_hash;
        if (reason == "mfa_challenge" && attempt < 3) {
            fprintf(stderr, "%s\n", message.empty() ? "MFA token required" : message.c_str());
            if (!read_secret("MFA token: ", &token)) return 1;
            continue;
        }
        if (reason == "tos" || reason == "critical") {
            fprintf(stderr, "login refused: you must first read and accept the %s.\n%s\n"
                            "After reading it, run again with --agree-tos.\n",
                    reason == "tos" ? "Terms of Service" : "critical message", message.c_str());
            return 1;
        }
        fprintf(stderr, "login failed (%s): %s\n", reason.c_str(), message.c_str());
        return 1;
    }

    printf("logged in as %s %s (%s)\n", cl->first.c_str(), cl->last.c_str(),
           uuid_str(cl->agent_id).c_str());
    printf("type /help for commands, /quit to log out\n");
    fflush(stdout);
    client_connect(cl);

    /* ---- main loop: stdin + udp sockets, http is pumped from client_tick ---- */
    bool stdin_open = true;
    std::string linebuf;
    while (!cl->done) {
        if (g_sigint && !cl->logging_out) client_logout(cl);

        struct pollfd pf[3];
        int n = 0, si = -1;
        if (stdin_open) { pf[n].fd = 0; pf[n].events = POLLIN; pf[n].revents = 0; si = n++; }
        if (cl->cur.open) { pf[n].fd = cl->cur.fd; pf[n].events = POLLIN; pf[n].revents = 0; n++; }
        if (cl->nxt.open) { pf[n].fd = cl->nxt.fd; pf[n].events = POLLIN; pf[n].revents = 0; n++; }
        poll(pf, (nfds_t)n, 50);

        if (si >= 0 && (pf[si].revents & (POLLIN | POLLHUP))) {
            char buf[1024];
            ssize_t r = read(0, buf, sizeof buf);
            if (r <= 0) { stdin_open = false; client_logout(cl); }
            else {
                linebuf.append(buf, (size_t)r);
                size_t nl;
                while ((nl = linebuf.find('\n')) != std::string::npos) {
                    std::string line = linebuf.substr(0, nl);
                    linebuf.erase(0, nl + 1);
                    client_command(cl, line);
                }
                if (linebuf.size() > 8192) linebuf.clear();
            }
        }

        client_recv(cl, &cl->cur);
        client_recv(cl, &cl->nxt);
        client_tick(cl);
    }

    client_shutdown(cl);
    delete cl;
    curl_global_cleanup();
    return 0;
}
