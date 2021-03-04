#pragma once

#include "../board/Board.h"
#include "../multipiece/MultiPiece.h"
#include "../players/Player.h"
#include "../players/Local.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <termios.h>
#include <vector>

using namespace std;

/// The possible game statuses.
enum GameStatus : char {
    ONGOING = 0,
    BLACK_RESIGN = 1,
    WHITE_RESIGN = 2,
    BLACK_WIN = 3,
    WHITE_WIN = 4,
    PAT = 5
};

/**
 * A class that manages the chess game.
 */
class Game {
public:

    /// The possible messages to display upon game ending.
    static const map<GameStatus, string> finish_messages;

    /**
     * Constructor.
     *
     * @param white_player  The white player.
     * @param black_player  The black player.
     * @param walk_through  Whether to walk through the pre moves, allows to track the game up to this point.
     * @param pre_turns     Previous turns to set up the game, the game starts after all the pre turns are performed on
     *                      the initial board state.
     */
    Game(unique_ptr<Player> white_player, unique_ptr<Player> black_player, bool walk_through, const string &pre_turns);

    /**
     * Set the pre turns.
     *
     * Sync with the other players the pre turns. Only one player can supply the pre turns, if more that one tries to
     * supply - throw an exception.
     *
     * @param pre_turns The pre turns supplied by this player.
     */
    void setup_turns(const string &pre_turns);

    /**
     * Get and perform a singe turn.
     *
     * @param status    The game status.
     * @param color     The color whose turn it is.
     */
    void turn(GameStatus &status, Color color);

    /**
     * Play the whole game course.
     *
     * @return  The finished game status.
     */
    GameStatus play_game();


private:

    /// Whether to walk through the pre moves.
    bool m_walk_through;

    /// The previous turns, turns that are performed on the board's initial state before the game starts.
    vector<turn_t> m_pre_turns;

    /// The board.
    Board m_board;

    /// The turn number.
    unsigned int m_turn;

    /// The turns performed.
    vector<turn_t> m_turns;

    /// The white player.
    unique_ptr<Player> m_white_player;

    /// The black player.
    unique_ptr<Player> m_black_player;
};
