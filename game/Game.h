#pragma once

#include "../board/Board.h"
#include "../multipiece/MultiPiece.h"
#include "../players/Player.h"

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

    Game(unique_ptr<Player> white_player, unique_ptr<Player> black_player, bool walk_through, string pre_moves);

    char get_input();

    bool is_args_input();

    void turn(GameStatus &status, Color color);

    GameStatus play_game();

private:
    bool m_walk_through;

    const string m_pre_moves;

    size_t m_pre_moves_index;

    Board m_board;

    unsigned int m_turn;

    vector<tuple<Point, Point, char>> m_turns;

    unique_ptr<Player> m_white_player;

    unique_ptr<Player> m_black_player;
};
