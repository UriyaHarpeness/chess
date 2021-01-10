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

enum GameStatus : char {
    ONGOING = 0,
    BLACK_RESIGN = 1,
    WHITE_RESIGN = 2,
    BLACK_WIN = 3,
    WHITE_WIN = 4,
    PAT = 5
};

class Game {
public:
    static const map<GameStatus, string> finish_messages;

    Game(unique_ptr<Player> white_player, unique_ptr<Player> black_player, bool walk_through, const string &pre_moves);

    void setup_turns(const string &pre_turns);

    void turn(GameStatus &status, Color color);

    GameStatus play_game();

private:
    bool m_walk_through;

    vector<turn_t> m_pre_turns;

    Board m_board;

    unsigned int m_turn;

    vector<turn_t> m_turns;

    unique_ptr<Player> m_white_player;

    unique_ptr<Player> m_black_player;
};
