#include "Game.h"

Game::Game() : m_board() {}

/*char Game::getch() {
    struct termios old_settings{}, new_settings{};

    tcgetattr(0, &old_settings); //grab old terminal i/o settings
    new_settings = old_settings; //make new settings same as old settings
    new_settings.c_lflag &= ~ICANON; //disable buffered i/o
    new_settings.c_lflag &= ~ECHO; //set echo mode
    tcsetattr(0, TCSANOW, &new_settings); //apply terminal io settings

    char ch;
    ch = (char) getchar();

    tcsetattr(0, TCSANOW, &old_settings);

    return ch;
}*/

char Game::getch() {
    static unsigned int index = 0;
    string test = "b8 c6 b1 a3 e7 e5 h2 h3 e5";
    if (index >= test.size())exit(0);
    return test[index++];
}

void Game::turn(bool &quit, Color color) {
    char action;

    const map<Point, set<Point>> possible_moves = m_board.get_possible_moves(color);
    m_board.draw_board(possible_moves);

    map<Point, set<Point>> matches;
    set<Point> piece_matches;

    cout << "Enter source: ";

    // Loop until a valid action has been made.
    while (true) {
        // Get action.
        do {
            action = (char) tolower(getch());
        } while (isspace(action));

        if (action == undo_action) {
            // Undo.
            /*if (!m_board.undo_board()) {
                cout << "Undo is not possible, retry: ";
            } else {
                m_moves++;

                break;
            }*/
        } else if (false) {//(quit_actions.find(action) != quit_actions.end()) {
            // Quit.
            quit = true;
            break;
        } else {
            // Change base.
            matches.clear();
            copy_if(possible_moves.begin(), possible_moves.end(), inserter(matches, matches.end()),
                    [action](const auto &point) { return point.first.get_x() == action - 'a'; });

            if (matches.empty()) continue;

            cout << action;
            unsigned char x = action - 'a';

            if (matches.size() == 1) {
                action = matches.begin()->first.get_y() + 1 + '0';
            } else {
                do {
                    action = (char) tolower(getch());
                } while (isspace(action) || (possible_moves.find(Point(x, action - '0' - 1)) == possible_moves.end()));
            }

            unsigned char y = action - '0' - 1;
            cout << action << endl;
            Point source(x, y);

            const auto piece_possible_moves = possible_moves.at({x, y});
            m_board.draw_board(piece_possible_moves, {x, y});
            cout << "Enter destination: ";

            do {
                piece_matches.clear();
                action = (char) tolower(getch());
                copy_if(piece_possible_moves.begin(), piece_possible_moves.end(),
                        inserter(piece_matches, piece_matches.end()),
                        [action](const auto &point) { return point.get_x() == action - 'a'; });
            } while (isspace(action) || piece_matches.empty());

            x = action - 'a';
            cout << action;

            // todo: maybe shorten based on Y axis as well...
            if (piece_matches.size() == 1) {
                action = piece_matches.begin()->get_y() + 1 + '0';
            } else {
                do {
                    action = (char) tolower(getch());
                } while (isspace(action) ||
                         (piece_possible_moves.find(Point(x, action - '0' - 1)) == piece_possible_moves.end()));
            }
            y = action - '0' - 1;
            cout << action << endl;

            Point destination(x, y);

            // todo: not like this
            m_board.do_move(source, destination);

            break;

            //char x, y;
            //cout << "Enter new base (x y):" << endl;
            //cin >> y >> x;
            //if (!m_board.set_base({x, y})) {
            //    cout << "Position is not valid, retry: ";
            //} else {
            //    break;
            //}
        }
    }
}

bool Game::play() {
    // Display title.
    bool quit;
    auto color = WHITE;
    while (true) {
        turn(quit, color);
        color = color ? BLACK : WHITE;
    }
}
