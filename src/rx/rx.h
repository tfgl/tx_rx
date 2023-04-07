#pragma once

#include <asm-generic/socket.h>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <wait.h>
#include <netdb.h>
#include <cstring>
#include <sched.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils.h"

class Receiver {
    addrinfo _info;
    addrinfo* _res;
    addrinfo* _rec;
    std::string _port;
    uint8_t _max_client;
    int _socket;

public:
    Receiver(str const& port, uint8_t max_client);
    void bind_socket();
    void run();
};
