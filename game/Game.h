#pragma once

#include "../board/Board.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <termios.h>
#include <vector>

using namespace std;

class Game {
public:

    static const char undo_action = 'u';

    Game();

    /**
     * Get single keyboard input without enter.
     *
     * A linux implementation for the windows `getch()` function.
     * Based on https://stackoverflow.com/questions/7469139/what-is-the-equivalent-to-getch-getche-in-linux.
     *
     * @return  The input character.
     */
    static char getch();

    void turn(bool &quit, Color color);

    bool play();


private:
    Board m_board;
};
