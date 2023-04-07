#include "tx.h"
#include "utils.h"
#include <cerrno>
#include <cstdio>
#include <iostream>
#include <utility>

Transmitter::Transmitter() {
}

Transmitter::Transmitter(str const& host, str const& port) {
    init(host, port);
}

void Transmitter::init(str const& host, str const& port) {
    memset(&_hints, 0, sizeof(addrinfo));
    _hints.ai_family = AF_INET;
    _hints.ai_socktype = SOCK_STREAM;
    _hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(host.c_str(), port.c_str(), &_hints, &_res) != 0) {
        std::cout << "ERROR: Transmitter::Transmitter(host, port): getaddrinfo:" << errno << std::endl;
    }
}

void Transmitter::connect_socket() {
    for (_rec = _res; _rec != NULL; _rec = _rec->ai_next) {
        _tx_socket = socket(_rec->ai_family, _rec->ai_socktype, _rec->ai_protocol);

        if (_tx_socket == -1) continue;
        if (connect(_tx_socket, _rec->ai_addr, _rec->ai_addrlen) != 0) break;
    }
}

void Transmitter::send_msg(str msg) {
    connect_socket();

    if (send(_tx_socket, (void*)(msg.c_str()), msg.size()+1, 0) == -1) {
        std::cout << "ERROR: Transmitter::send_msg(): send:" << errno << std::endl;
    }

    close(_tx_socket);
}

void Transmitter::loop() {
    while (true) {
        str msg;
        getline(std::cin, msg);
        if ( msg == "/quit" ) break;

        send_msg(msg);
    }
}

void Transmitter::run() {
    init_resolve_map();
    while (!_should_close) {
        str command;
        getline(std::cin, command);
        resolve_command(command, NULL);
    }
}

// ===== resolve map =====
void Transmitter::init_resolve_map() {
    _resolve_map = uptr<ResolveMap>(new ResolveMap);
    _resolve_map->insert(std::make_pair("connect", &Transmitter::connect_command));
    _resolve_map->insert(std::make_pair("quit", &Transmitter::close_command));
}

bool Transmitter::resolve_command(str const& str_command, void* result) {
    auto command = split(str_command, ' ');
    if (_resolve_map->count(command[0]) == 0)
        return false;
    auto call_back = _resolve_map->at(command[0]);
    return (this->*call_back)(command, result);
}

// ===== command =====
bool Transmitter::connect_command(strv const& command, void* result) {
    if (command.size() < 3) return false;

    this->init(command[1], command[2]);
    std::cout << "connecting to " << command[1] << ":" << command[2] << std::endl;
    this->loop();

    return true;
}

bool Transmitter::close_command(strv const& command, void* result) {
    _should_close = true;
    return true;
}
