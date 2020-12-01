#include "game/Game.h"

using namespace std;

int main() {
    /*for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            cout << "\u001b[48;5;" << 16 * 16 - 1 - (i * 16 + j) << ";38;5;" << (i * 16 + j) << "m"
                 << Board::zfill(to_string(i * 16 + j), 3, '0') << "\u001b[0m";
        }
        cout << endl;
    }*/

    Game g;
    g.play();

    return 0;
}
