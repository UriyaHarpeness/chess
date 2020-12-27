#include "Server.h"

Server::Server(const string &address, int port) : m_server_socket(-1), m_socket(-1) {
    struct sockaddr_in socket_address{};
    int opt = 1;

    // Creating socket file descriptor
    if ((m_server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        throw runtime_error("Socket creation error");
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(m_server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        throw runtime_error("Set socket options failed");
    }

    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = INADDR_ANY;
    socket_address.sin_port = htons(port);

    if (inet_aton(address.c_str(), (in_addr *) &socket_address.sin_addr.s_addr) == 0) {
        throw runtime_error("Invalid address");
    }

    // Forcefully attaching socket to the port
    if (bind(m_server_socket, (struct sockaddr *) &socket_address, sizeof(socket_address)) == -1) {
        throw runtime_error("Bind failed");
    }
    if (listen(m_server_socket, 3) < 0) {
        throw runtime_error("Bind failed");
    }
    if ((m_socket = accept(m_server_socket, (struct sockaddr *) &socket_address, (socklen_t *) &socket_address)) ==
        -1) {
        throw runtime_error("Accept failed");
    }
}

Server::~Server() {
    close(m_socket);
    close(m_server_socket);
}

turn_t
Server::get_turn(Board &board, const vector<tuple<Point, Point, char>> &turns, map<Point, set<Point>> possible_moves,
                 map<Point, map<Point, play>> possible_play_moves, Color color, unsigned int turn) {
    char buffer[5];
    read(m_socket, &buffer, 5);
    cout << "Turn: " << buffer << endl;
    return string_to_turn(buffer);
}

void Server::forward_turn(const turn_t &turn) {
    const Point &source = get<0>(turn);
    const Point &destination = get<1>(turn);
    const char &promotion = get<2>(turn);
    string turn_s = turn_to_string(turn);
    write(m_socket, turn_s.c_str(), 5);
}
