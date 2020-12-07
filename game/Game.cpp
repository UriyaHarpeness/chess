#include "Game.h"

Game::Game() : m_board(), m_turn(0) {}

template<class T>
set<Point> Game::get_keys(const map<Point, T> &mapping) {
    set<Point> points;

    for_each(mapping.begin(), mapping.end(), [&points](const auto &p) { points.insert(p.first); });

    return move(points);
}

Point Game::moves_fast_match(const set<Point> &options, char input, bool get) {
#if AUTOFILL
    if (options.size() == 1) {
        cout << (char) (options.begin()->get_x() + 'a') << options.begin()->get_y() + 1 << endl;
        return *options.begin();
    }
#endif // AUTOFILL

    set<Point> matches, final_matches;

    do {
        do {
            if (get) {
                input = (char) tolower(getch());
            }
            get = true;
        } while (isspace(input));

        copy_if(options.begin(), options.end(), inserter(matches, matches.end()),
                [&, input](const auto &point) {
                    return ((isdigit(input) && (point.get_y() == input - '0' - 1)) ||
                            (isalpha(input) && (point.get_x() == input - 'a')));
                });
    } while (matches.empty());

    cout << input;

#if AUTOFILL
    if (matches.size() == 1) {
        cout << (char) (isalpha(input) ? (char) matches.begin()->get_y() + '0' + 1 :
                        (char) matches.begin()->get_x() + 'a') << endl;
        return *matches.begin();
    }
#endif // AUTOFILL

    do {
        final_matches.clear();

        do {
            input = (char) tolower(getch());
        } while (isspace(input));

        copy_if(matches.begin(), matches.end(), inserter(final_matches, final_matches.end()),
                [&, input](const auto &point) {
                    return ((isdigit(input) && (point.get_y() == input - '0' - 1)) ||
                            (isalpha(input) && (point.get_x() == input - 'a')));
                });
    } while (final_matches.size() != 1);

    cout << input << endl;

    return *final_matches.begin();
}

#if SIMULATE

char Game::getch() {
    static unsigned int index = 0;
    // string test = "b8 c6 b1 a3 e7 e5 h2 h3 e5 e4 f2 f3 e4 f3";
    // string test = "e7 e5 a2 a4 e5 e4 b2 b4 g7 g5 b4 b5 g5 g4 f2 f4 e4 f3";
    // string test = "d7 d5 e2 e3 d5 d4 e3 d4 g8 f6 d4 d5 c7 c5 d5 c6 e8 d7 h2 h4 d8 b6 c6 d6"; // En passant
    string test = "d7 d5 e2 e4 b8 a6 g1 h3 c8 e6 f1 d3 d8 d7 e1 g1 e8 d8"; // Castling
    // Castling
    // G8 H6 H2 H3 E7 E5 H3 H4 F8 A3 H4 H5 G7 G5 H1 H4 G5 G4 H4 G4 F7 F5 G2 G3 A3 B4 G4 H4 E8 G8
    // G8 H6 H2 H3 E7 E5 H3 H4 F8 A3 H4 H5 G7 G5 H1 H4 G5 G4 H4 G4 F7 F5 G2 G3 E8 F8 G4 C4 F8 E8 C4 C7 E8 E7
    // G8 H6 H2 H3 E7 E5 H3 H4 F8 A3 H4 H5 G7 G5 H1 H4 G5 G4 H4 G4 F7 F5 G2 G3 H8 G8 G4 H4 G8 H8 H4 B4 E8 F7
    if (index >= test.size()) exit(0);
    return test[index++];
}

#else

char Game::getch() {
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
}

#endif // SIMULATE

void Game::turn(bool &quit, Color color) {
    m_turn++;
    char action;
    set<Point> options;

    const map<Point, set<Point>> possible_moves = m_board.get_possible_moves(color, m_turn);
    map<Point, map<Point, play>> possible_play_moves;
    const auto possible_plays = MultiPiece::get_plays(m_board, color, m_turn, false);
    for (const auto &possible_play : possible_plays) {
        for (const auto &single_change : possible_play) {
            if ((single_change.first != nullptr) && (single_change.second != nullptr) &&
                (single_change.first->get_color() == color)) {
                possible_play_moves[single_change.first->get_position()][single_change.second->get_position()] = possible_play;
            }
        }
    }

    options = get_keys(possible_moves);
    auto tmp = get_keys(possible_play_moves);
    options.insert(tmp.begin(), tmp.end());

    cout << endl << (color ? "White" : "Black") << "'s turn" << endl;

    m_board.draw_board(options);

    set<Point> piece_matches;

    // Loop until a valid action has been made.
    while (true) {
        // Get action.
        cout << "Enter action: ";

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
        } else if (action == 'p') {
            // Print.
            cout << action << endl << "Turns: ";
            for (const auto &turn : m_turns) {
                cout << (char) (turn.first.get_x() + 'A') << turn.first.get_y() + 1 << " "
                     << (char) (turn.second.get_x() + 'A') << turn.second.get_y() + 1 << " ";
            }
            cout << endl;
        } else {
            cout << endl << "Enter source: ";

            Point source = Game::moves_fast_match(options, action);

            options.clear();
            if (possible_moves.find(source) != possible_moves.end()) {
                options.insert(possible_moves.at(source).begin(), possible_moves.at(source).end());
            }
            if (possible_play_moves.find(source) != possible_play_moves.end()) {
                for_each(possible_play_moves.at(source).begin(), possible_play_moves.at(source).end(),
                         [&options](const auto &option) { options.insert(option.first); });
            }
            map<Point, play> optional_plays;
            if (possible_play_moves.find(source) != possible_play_moves.end()) {
                optional_plays = possible_play_moves.at(source);
            }
            m_board.draw_board(options, optional_plays, source, m_turn);
            cout << "Enter destination: ";

            Point destination = Game::moves_fast_match(options, action, true);

            if ((possible_moves.find(source) != possible_moves.end()) &&
                (possible_moves.at(source).find(destination) != possible_moves.at(source).end())) {
                m_board.do_move(m_turn, source, destination);
            } else {
                m_board.do_move(possible_play_moves.at(source).at(destination));
            }

            m_turns.emplace_back(source, destination);

            break;
        }
    }
}

bool Game::play_game() {
    // Display title.
    m_turns.clear();
    bool quit;
    auto color = WHITE;
    while (true) {
        turn(quit, color);
        color = color ? BLACK : WHITE;
    }
}
