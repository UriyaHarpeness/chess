#include "Server.h"

Server::Server(const string &address, int port) : NetworkPlayer(), m_server_socket(-1) {
    struct sockaddr_in socket_address{};
    int opt = 1;

    // Creating socket file descriptor.
    if ((m_server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        throw runtime_error("Socket creation error");
    }

    // Set socket options.
    if (setsockopt(m_server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        throw runtime_error("Set socket options failed");
    }

    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = INADDR_ANY;
    socket_address.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form.
    if (inet_aton(address.c_str(), (in_addr *) &socket_address.sin_addr.s_addr) == 0) {
        throw runtime_error("Invalid address");
    }

    // Attaching the socket to the port.
    if (bind(m_server_socket, (struct sockaddr *) &socket_address, sizeof(socket_address)) == -1) {
        throw runtime_error("Bind failed");
    }

    // Listening on the socket.
    if (listen(m_server_socket, 3) < 0) {
        throw runtime_error("Listen failed");
    }

    // Accepting a connection.
    if ((m_socket = accept(m_server_socket, (struct sockaddr *) &socket_address, (socklen_t *) &socket_address)) ==
        -1) {
        throw runtime_error("Accept failed");
    }
}

Server::~Server() {
    close(m_server_socket);
}
