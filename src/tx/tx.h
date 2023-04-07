#pragma once

#include <asm-generic/socket.h>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <wait.h>
#include <netdb.h>
#include <cstring>
#include <sched.h>
#include <sys/socket.h>
#include <unistd.h>
#include <map>
#include <functional>

#include "utils.h"

class Transmitter {
    typedef bool (Transmitter::*MemberFunction)(strv const& command, void* result);
    using ResolveMap = std::map<const str, MemberFunction>;

    addrinfo _hints;
    addrinfo* _res;
    addrinfo* _rec;
    int _tx_socket;
    bool _should_close = false;

public:
    Transmitter(str const& host, str const& port);
    Transmitter();
    void init(str const& host, str const& port);
    void loop();
    void run();

private:
    void connect_socket();
    void send_msg(str msg);
    bool resolve_command(str const& str_command, void* result);
    void init_resolve_map();

private:
    uptr<ResolveMap> _resolve_map;
    bool connect_command(strv const& command, void* result);
    bool close_command(strv const& command, void* result);
};
