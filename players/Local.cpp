#include "Local.h"

const set<char> Local::quit_actions = {'q', 127, 27};

const set<char> Local::promotion_options = {'q', 'r', 'b', 'n'};

Point Local::moves_fast_match(const set<Point> &options, char input, bool &reselect, bool get) {
    reselect = false;
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
                input = (char) tolower(getch());
            }
            get = true;
        } while (isspace(input));

        if (input == reselect_action) {
            cout << input << endl;
            reselect = true;
            return {-1, -1};
        }

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
                        (char) matches.begin()->get_x() + 'A') << endl;
        return *matches.begin();
    }
#endif // AUTOFILL

    do {
        final_matches.clear();

        do {
            input = (char) tolower(getch());
        } while (isspace(input));

        if (input == reselect_action) {
            cout << input << endl;
            reselect = true;
            return {-1, -1};
        }

        copy_if(matches.begin(), matches.end(), inserter(final_matches, final_matches.end()),
                [&, input](const auto &point) {
                    return ((isdigit(input) && (first_char) && (point.get_y() == input - '0' - 1)) ||
                            (isalpha(input) && (!first_char) && (point.get_x() == input - 'a')));
                });
    } while (final_matches.size() != 1);

    cout << (char) toupper(input) << endl;

    return *final_matches.begin();
}

char Local::getch() {
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

turn_t
Local::get_turn(Board &board, const vector<tuple<Point, Point, char>> &turns, map<Point, set<Point>> possible_moves,
                map<Point, map<Point, play>> possible_play_moves, Color color, unsigned int turn) {
    char action;
    char promotion = 0;
    bool reselect;

    set<Point> options;
    auto piece_options = get_keys(possible_moves);
    auto tmp = get_keys(possible_play_moves);
    piece_options.insert(tmp.begin(), tmp.end());

    set<Point> piece_matches;

    // Loop until a valid action has been made.
    while (true) {
        board.draw_board(piece_options, color, turn);

        // Get action.
        cout << "Enter action: ";

        do {
            action = (char) tolower(getch());
        } while (isspace(action));

        if (quit_actions.find(action) != quit_actions.end()) {
            // Quit.
            return quit_turn;
        } else if (action == 'p') {
            // Print.
            cout << action << endl << "Turns: ";
            for (const auto &single_turn : turns) {
                cout << (char) (get<0>(single_turn).get_x() + 'A') << get<0>(single_turn).get_y() + 1
                     << (char) (get<1>(single_turn).get_x() + 'A') << get<1>(single_turn).get_y() + 1;
                if (get<2>(single_turn)) {
                    cout << (char) toupper(get<2>(single_turn));
                }
                cout << " ";
            }
            cout << endl << endl;
        } else {
            cout << endl << "Enter source: ";

            Point source = moves_fast_match(piece_options, action, reselect);
            if (reselect) continue;

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
            board.draw_board(options, optional_plays, source, turn);
            cout << "Enter destination: ";

            Point destination = moves_fast_match(options, action, reselect, true);
            if (reselect) continue;

            // Promotion.
            if (MultiPiece::is_promotion(board, source, destination)) {
                cout << "Enter promotion piece of choice (QRBK): ";
                do {
                    promotion = (char) tolower(getch());
                } while (isspace(promotion) || (promotion_options.find(promotion) == promotion_options.end()));
                cout << (char) toupper(promotion) << endl;
            }

            return {source, destination, promotion};
        }
    }
}

void Local::forward_turn(const turn_t &turn) {}
