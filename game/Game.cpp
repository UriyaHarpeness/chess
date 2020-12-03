#include "Game.h"

Game::Game() : m_board(), m_turn(0) {}

template<class T>
set<Point> Game::get_keys(const map<Point, T> &mapping) {
    set<Point> points;

    for_each(mapping.begin(), mapping.end(), [&points](const auto &p) { points.insert(p.first); });

    return move(points);
}

Point Game::moves_fast_match(const set<Point> &options, char &input) {
#if AUTOFILL
    if (options.size() == 1) {
        cout << (char) (options.begin()->get_x() + 'a') << options.begin()->get_y() + 1 << endl;
        return *options.begin();
    }
#endif // AUTOFILL

    set<Point> matches, final_matches;

    do {
        do {
            input = (char) tolower(getch());
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
    string test = "d7 d5 e2 e3 d5 d4 e3 d4 g8 f6 d4 d5 c7 c5 d5 c6 e8 d7 h2 h4 d8 b6 c6 d6";
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
    const auto possible_plays = MultiPiece::get_plays(m_board.get_board(), color, m_turn);
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

    cout << "Enter source: ";

    // Loop until a valid action has been made.
    while (true) {
        // Get action.
        Point source = Game::moves_fast_match(options, action);

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
            options.clear();
            if (possible_moves.find(source) != possible_moves.end()) {
                options.insert(possible_moves.at(source).begin(), possible_moves.at(source).end());
            }
            if (possible_play_moves.find(source) != possible_play_moves.end()) {
                for_each(possible_play_moves.at(source).begin(), possible_play_moves.at(source).end(),
                         [&options](const auto &option) { options.insert(option.first); });
            }
            auto piece_possible_moves_addition = possible_play_moves;
            m_board.draw_board(options, source, m_turn);
            cout << "Enter destination: ";

            Point destination = Game::moves_fast_match(options, action);

            if ((possible_moves.find(source) != possible_moves.end()) &&
                (possible_moves.at(source).find(destination) != possible_moves.at(source).end())) {
                m_board.do_move(m_turn, source, destination);
            } else {
                m_board.do_move(possible_play_moves.at(source).at(destination));
            }

            break;
        }
    }
}

bool Game::play_game() {
    // Display title.
    bool quit;
    auto color = WHITE;
    while (true) {
        turn(quit, color);
        color = color ? BLACK : WHITE;
    }
}
