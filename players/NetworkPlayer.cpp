#include "NetworkPlayer.h"

NetworkPlayer::NetworkPlayer() : m_socket(-1) {}

NetworkPlayer::~NetworkPlayer() {
    close(m_socket);
}

vector<turn_t> NetworkPlayer::get_turns() {
    vector<turn_t> turns;
    char buffer[5];
    uint16_t turns_number;

    // Read number of turns.
    read(m_socket, &turns_number, sizeof(turns_number));
    for (uint16_t i = 0; i < turns_number; i++) {
        // Read each turn and convert.
        read(m_socket, &buffer, 5);
        turns.push_back(string_to_turn(buffer));
    }

    return move(turns);
}

void NetworkPlayer::forward_turns(const vector<turn_t> &turns) {
    string turn_s;
    uint16_t turns_number = turns.size();

    // Write number of turns.
    write(m_socket, &turns_number, sizeof(turns_number));
    for (uint16_t i = 0; i < turns_number; i++) {
        // Convert each turn and write.
        turn_s = turn_to_string(turns[i]);
        write(m_socket, turn_s.c_str(), 5);
    }
}

turn_t NetworkPlayer::get_turn(Board &board, const vector<tuple<Point, Point, char>> &turns,
                               map<Point, set<Point>> possible_moves, map<Point, map<Point, play>> possible_play_moves,
                               Color color, unsigned int turn) {
    char buffer[5];

    // Read turn.
    read(m_socket, &buffer, 5);
    cout << "Turn: " << buffer << endl;

    // Convert and return.
    return string_to_turn(buffer);
}

void NetworkPlayer::forward_turn(const turn_t &turn) {
    // Convert and write turn.
    string turn_s = turn_to_string(turn);
    write(m_socket, turn_s.c_str(), 5);
}
