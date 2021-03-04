#pragma once

#include "Player.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <stdexcept>

using namespace std;

/**
 * A base class for network players.
 */
class NetworkPlayer : public Player {
public:

    /**
     * Get the keys of a map of points.
     *
     * @overload
     */
    vector<turn_t> get_turns() override;

    /**
     * Forward turns performed by a player.
     *
     * @overload
     */
    void forward_turns(const vector<turn_t> &turns) override;

    /**
     * Get the player's turn.
     *
     * @overload
     */
    turn_t
    get_turn(Board &board, const vector<tuple<Point, Point, char>> &turns, map<Point, set<Point>> possible_moves,
             map<Point, map<Point, play>> possible_play_moves, Color color, unsigned int turn) override;

    /**
     * Forward a turn performed by a player.
     *
     * @overload
     */
    void forward_turn(const turn_t &turn) override;


protected:

    /**
     * Constructor.
     */
    NetworkPlayer();

    /**
     * Destructor.
     */
    virtual ~NetworkPlayer();

    /// The socket holding the connection.
    int m_socket;
};
