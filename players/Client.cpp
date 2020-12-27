#include "Client.h"

Client::Client(const string &address, int port) {
    struct sockaddr_in serv_addr{};
    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        throw runtime_error("Socket creation error");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, address.c_str(), &serv_addr.sin_addr) <= 0) {
        throw runtime_error("Invalid address");
    }

    if (connect(m_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        throw runtime_error("Connection failed");
    }
}

Client::~Client() {
    close(m_socket);
}

turn_t
Client::get_turn(Board &board, const vector<tuple<Point, Point, char>> &turns, map<Point, set<Point>> possible_moves,
                 map<Point, map<Point, play>> possible_play_moves, Color color, unsigned int turn) {
    char buffer[5];
    read(m_socket, &buffer, 5);
    cout << "Turn: " << buffer << endl;
    return string_to_turn(buffer);
}

void Client::forward_turn(const turn_t &turn) {
    const Point &source = get<0>(turn);
    const Point &destination = get<1>(turn);
    const char &promotion = get<2>(turn);
    string turn_s = turn_to_string(turn);
    write(m_socket, turn_s.c_str(), 5);
}
