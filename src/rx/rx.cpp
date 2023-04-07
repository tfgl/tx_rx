#include "rx.h"
#include <cstring>

Receiver::Receiver(str const& port, uint8_t max_client): _port(port), _max_client(max_client) {
    memset(&_info, 0, sizeof(addrinfo));
    _info.ai_family = AF_INET;
    _info.ai_socktype = SOCK_STREAM;
    _info.ai_protocol = IPPROTO_TCP;

    if ( getaddrinfo(NULL, port.data(), &_info, &_res) != 0) {
        std::cout << "Receiver::Receiver(port, max_client) : getaddrinfo" << std::endl;
    }
}

void Receiver::bind_socket() {
    for( _rec = _res; _rec != NULL; _rec = _rec->ai_next) {
        _socket = socket(_rec->ai_family, _rec->ai_socktype, _rec->ai_protocol);
        if (_socket == -1) continue;

        int enable = 1;
        setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
        if (bind(_socket, _rec->ai_addr, _rec->ai_addrlen) == 0) break;

        close(_socket);
    }
}

void Receiver::run() {
    bind_socket();

    if (listen(_socket, _max_client) != 0) {
        std::cout << "Receiver::run(): listen" << std::endl
                  << "\tsocket: " << _socket << std::endl
                  << "\terrno: " << errno << std::endl;
    }
    int client_socket;
    sockaddr client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    const size_t buff_size = 1000;
    char buff[buff_size];

    while( true ) {
        buff[0] = 0;

        if ((client_socket = accept(_socket, &client_addr, &client_addr_len)) < 0) continue;
        if (recv(client_socket, buff, buff_size, 0) == -1) continue;

        std::cout << buff << std::endl;

        close(client_socket);
    }

}
