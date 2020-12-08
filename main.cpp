#include "game/Game.h"

using namespace std;

int main(int argc, char *argv[]) {
    // Set default game settings.
    bool walk_through = false;
    string pre_moves;

    // Check input.
    if ((argc > 3) || ((argc >= 2) && (string(argv[1]) == "-h"))) {
        throw runtime_error("Usage: chess [--walk-through] [MOVES]");
    }

    // Parse arguments.
    for (int index = 1; index < argc; index++) {
        if (string(argv[index]) == "--walk-through") {
            walk_through = true;
        } else {
            pre_moves = string(argv[index]);
        }
    }

    // Initialize the game.
    Game g(walk_through, pre_moves);

    // Play.
    g.play_game();

    return 0;
}