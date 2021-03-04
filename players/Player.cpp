#include "Player.h"

const turn_t Player::quit_turn = {{0, 0}, {0, 0}, 0};

string Player::turn_to_string(const turn_t &turn) {
    const Point &source = get<0>(turn);
    const Point &destination = get<1>(turn);
    const char &promotion = get<2>(turn);
    return {static_cast<char>(source.get_x() + 'a'), static_cast<char>(source.get_y() + '0' + 1),
            static_cast<char>(destination.get_x() + 'a'), static_cast<char>(destination.get_y() + '0' + 1),
            promotion};
}

turn_t Player::string_to_turn(const string &turn) {
    return {Point(turn[0] - 'a', turn[1] - '0' - 1), Point(turn[2] - 'a', turn[3] - '0' - 1), turn[4]};
}
