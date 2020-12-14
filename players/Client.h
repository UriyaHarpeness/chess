#pragma once

#include "Player.h"

using namespace std;

class Client : public Player {
public:
    Client(const string &address, int port);

    ~Client();

    turn_t
    get_turn(Board &board, const vector<tuple<Point, Point, char>> &turns, map<Point, set<Point>> possible_moves,
             map<Point, map<Point, play>> possible_play_moves, Color color, unsigned int turn) override;

    void forward_turn(const turn_t &turn) override;

private:
    int m_socket;
};
