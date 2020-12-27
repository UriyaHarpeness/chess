#pragma once

#include "Player.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <stdexcept>

using namespace std;

class Server : public Player {
public:
    Server(const string &address, int port);

    ~Server();

    turn_t
    get_turn(Board &board, const vector<tuple<Point, Point, char>> &turns, map<Point, set<Point>> possible_moves,
             map<Point, map<Point, play>> possible_play_moves, Color color, unsigned int turn) override;

    void forward_turn(const turn_t &turn) override;

private:
    int m_socket;

    int m_server_socket;
};
