#include "Game.h"

#include <utility>

const set<char> Game::quit_actions = {'q', 127, 27};

const set<char> Game::promotion_options = {'q', 'r', 'b', 'n'};

const map<GameStatus, string> Game::finish_messages = {{BLACK_RESIGN, "Black has quit, either a professional forfeit, or a total coward."},
                                                       {WHITE_RESIGN, "White has resign, it's so like him, isn't it?"},
                                                       {BLACK_WIN,    "OUR HEAVY WEIGHT WRESTLING CHAMPION IS... Oh, wrong match, black wins, woohoo..."},
                                                       {WHITE_WIN,    "Congrats, white, now everyone hates you."}};

Game::Game(bool walk_through, string pre_moves) : m_walk_through(walk_through), m_pre_moves(move(pre_moves)),
                                                  m_pre_moves_index(0), m_board(), m_turn(0) {}

template<class T>
set<Point> Game::get_keys(const map<Point, T> &mapping) {
    set<Point> points;

    for_each(mapping.begin(), mapping.end(), [&points](const auto &p) { points.insert(p.first); });

    return move(points);
}

Point Game::moves_fast_match(const set<Point> &options, char input, bool get) {
#if AUTOFILL
    if (options.size() == 1) {
        cout << (char) (options.begin()->get_x() + 'A') << options.begin()->get_y() + 1 << endl;
        return *options.begin();
    }
#endif // AUTOFILL

    set<Point> matches, final_matches;

    do {
        do {
            if (get) {
                input = (char) tolower(get_input());
            }
            get = true;
        } while (isspace(input));

        copy_if(options.begin(), options.end(), inserter(matches, matches.end()),
                [&, input](const auto &point) {
                    return ((isdigit(input) && (point.get_y() == input - '0' - 1)) ||
                            (isalpha(input) && (point.get_x() == input - 'a')));
                });
    } while (matches.empty());

    const bool first_char = isalpha(input);
    cout << (char) toupper(input);

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
            input = (char) tolower(get_input());
        } while (isspace(input));

        copy_if(matches.begin(), matches.end(), inserter(final_matches, final_matches.end()),
                [&, input](const auto &point) {
                    return ((isdigit(input) && (first_char) && (point.get_y() == input - '0' - 1)) ||
                            (isalpha(input) && (!first_char) && (point.get_x() == input - 'a')));
                });
    } while (final_matches.size() != 1);

    cout << (char) toupper(input) << endl;

    return *final_matches.begin();
}

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

char Game::get_input() {
    if (m_pre_moves.length() > m_pre_moves_index) {
        return m_pre_moves[m_pre_moves_index++];
    }
    return getch();
}

void Game::turn(GameStatus &status, Color color) {
    m_turn++;
    char action;
    set<Point> options;

    map<Point, set<Point>> possible_moves = m_board.get_possible_moves(color, m_turn);
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

    m_board.filter_illegal_moves(possible_moves, possible_play_moves, color, m_turn);

    options = get_keys(possible_moves);
    auto tmp = get_keys(possible_play_moves);
    options.insert(tmp.begin(), tmp.end());

    if (options.empty()) {
        status = color ? BLACK_WIN : WHITE_WIN;
        return;
    }

    const auto king_position = m_board.find_king(color);
    const bool king_threatened = m_board.is_threatened(king_position, color, m_turn, true);
    cout << endl << (color ? "White" : "Black") << "'s turn" << (king_threatened ? " (Check)" : "") << endl;

    m_board.draw_board(options);

    set<Point> piece_matches;

    // Loop until a valid action has been made.
    while (!status) {
        // Get action.
        cout << "Enter action: ";

        do {
            action = (char) tolower(get_input());
        } while (isspace(action));

        if (quit_actions.find(action) != quit_actions.end()) {
            // Quit.
            status = color ? WHITE_RESIGN : BLACK_RESIGN;
            break;
        } else if (action == 'p') {
            // Print.
            cout << action << endl << "Turns: ";
            for (const auto &turn : m_turns) {
                cout << (char) (get<0>(turn).get_x() + 'A') << get<0>(turn).get_y() + 1
                     << (char) (get<1>(turn).get_x() + 'A') << get<1>(turn).get_y() + 1;
                if (get<2>(turn)) {
                    cout << (char) toupper(get<2>(turn));
                }
                cout << " ";
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

            // Promotion.
            if (MultiPiece::is_promotion(m_board, destination)) {
                cout << "Enter promotion piece of choice (QRBK): ";
                do {
                    action = (char) tolower(get_input());
                } while (isspace(action) || (promotion_options.find(action) == promotion_options.end()));
                cout << (char) toupper(action) << endl;
                MultiPiece::perform_promotion(m_board, destination, action);
                m_turns.emplace_back(source, destination, action);
            } else {
                m_turns.emplace_back(source, destination, NULL);
            }

            if ((m_pre_moves.length() - 1 > m_pre_moves_index) && m_walk_through) getch();

            break;
        }
    }
}

GameStatus Game::play_game() {
    // Display title.
    m_turns.clear();
    auto color = BLACK;
    GameStatus status = ONGOING;

    while (!status) {
        turn(status, color);
        color = color ? BLACK : WHITE;
    }

    cout << endl << finish_messages.at(status) << endl;

    return status;
}
