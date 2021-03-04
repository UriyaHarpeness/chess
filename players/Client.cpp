#include "Client.h"

Client::Client(const string &address, int port) : NetworkPlayer() {
    struct sockaddr_in serv_addr{};

    // Creating socket file descriptor.
    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        throw runtime_error("Socket creation error");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form.
    if (inet_pton(AF_INET, address.c_str(), &serv_addr.sin_addr) <= 0) {
        throw runtime_error("Invalid address");
    }

    // Connecting.
    if (connect(m_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        throw runtime_error("Connection failed");
    }
}
