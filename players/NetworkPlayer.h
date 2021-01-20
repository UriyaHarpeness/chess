#pragma once

#include "Player.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <stdexcept>

using namespace std;

class NetworkPlayer : public Player {
public:
    vector<turn_t> get_turns() override;

    void forward_turns(const vector<turn_t> &turns) override;

    turn_t
    get_turn(Board &board, const vector<tuple<Point, Point, char>> &turns, map<Point, set<Point>> possible_moves,
             map<Point, map<Point, play>> possible_play_moves, Color color, unsigned int turn) override;

    void forward_turn(const turn_t &turn) override;

protected:
    NetworkPlayer();

    virtual ~NetworkPlayer();

    int m_socket;
};
