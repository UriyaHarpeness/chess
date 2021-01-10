#include "Game.h"

#include <utility>

const map<GameStatus, string> Game::finish_messages = {
        {BLACK_RESIGN, "Black has quit, either a professional forfeit, or a total coward."},
        {WHITE_RESIGN, "White has resign, it's so like him, isn't it?"},
        {BLACK_WIN,    "OUR HEAVY WEIGHT WRESTLING CHAMPION IS... Oh, wrong match, black wins, woohoo..."},
        {WHITE_WIN,    "Congrats, white, now everyone hates you."},
        {PAT,          "You are both just terrible."}};

Game::Game(unique_ptr<Player> white_player, unique_ptr<Player> black_player, bool walk_through, const string &pre_turns)
        : m_white_player(move(white_player)), m_black_player(move(black_player)), m_walk_through(walk_through),
          m_board(), m_turn(0) {
    setup_turns(pre_turns);
}

void Game::setup_turns(const string &pre_turns) {
    vector<turn_t> turns;
    vector<vector<turn_t>> turns_options;
    if (!pre_turns.empty()) {
        vector<string> split_arguments;
        int index = 0, new_index;
        do {
            new_index = pre_turns.find(' ', index);
            split_arguments.emplace_back(
                    pre_turns.substr(index, (new_index > 0 ? new_index : pre_turns.length()) - index));
            index = new_index + 1;
        } while (new_index != -1);
        turns.reserve(split_arguments.size());
        for (const auto &t : split_arguments) {
            turns.push_back(Player::string_to_turn(t));
        }
        turns_options.push_back(turns);
    }

    m_white_player->forward_turns(turns);
    m_black_player->forward_turns(turns);
    auto white_turns = m_white_player->get_turns();
    auto black_turns = m_black_player->get_turns();
    if (!white_turns.empty()) {
        turns_options.push_back(white_turns);
    }
    if (!black_turns.empty()) {
        turns_options.push_back(black_turns);
    }
    m_white_player->forward_turns(black_turns);
    m_black_player->forward_turns(white_turns);
    white_turns = m_white_player->get_turns();
    black_turns = m_black_player->get_turns();
    if (!white_turns.empty()) {
        turns_options.push_back(white_turns);
    }
    if (!black_turns.empty()) {
        turns_options.push_back(black_turns);
    }

    if (turns_options.size() > 1) {
        throw runtime_error("Too many players setting turns (max 1)");
    }

    if (!turns_options.empty()) {
        m_pre_turns = move(turns_options[0]);
    }
}

void Game::turn(GameStatus &status, Color color) {
    m_turn++;
    set<Point> piece_options;
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

    piece_options = Player::get_keys(possible_moves);
    auto tmp = Player::get_keys(possible_play_moves);
    piece_options.insert(tmp.begin(), tmp.end());

    const auto king_position = m_board.find_king(color);
    const bool king_threatened = m_board.is_threatened(king_position, color, m_turn, true);

    if (piece_options.empty()) {
        if (king_threatened) {
            status = color ? BLACK_WIN : WHITE_WIN;
        } else {
            status = PAT;
        }
        return;
    }

    cout << endl << (color ? "White" : "Black") << "'s turn" << (king_threatened ? " (Check)" : "") << endl;

    set<Point> piece_matches;

    const unique_ptr<Player> &player = (color ? m_white_player : m_black_player);
    const unique_ptr<Player> &other_player = (color ? m_black_player : m_white_player);
    turn_t turn;
    if (m_turn > m_pre_turns.size()) {
        turn = player->get_turn(m_board, m_turns, possible_moves, possible_play_moves, color, m_turn);
        other_player->forward_turn(turn);
    } else {
        turn = m_pre_turns[m_turn - 1];

        // Walk through.
        if (m_walk_through) {
            cout << "Press any key to continue... " << endl;
            Local::getch();
        }
    }
    Point source = get<0>(turn);
    Point destination = get<1>(turn);
    char promotion = get<2>(turn);

    if (tie(get<0>(Player::quit_turn), get<1>(Player::quit_turn), get<2>(Player::quit_turn)) ==
        tie(get<0>(turn), get<1>(turn), get<2>(turn))) {
        status = color ? WHITE_RESIGN : BLACK_RESIGN;
        return;
    }

    if ((possible_moves.find(source) != possible_moves.end()) &&
        (possible_moves.at(source).find(destination) != possible_moves.at(source).end())) {
        m_board.do_move(m_turn, source, destination);
    } else {
        m_board.do_move(possible_play_moves.at(source).at(destination));
    }

    // Promotion.
    if (promotion) {
        MultiPiece::perform_promotion(m_board, destination, promotion);
    }

    // Save turn.
    m_turns.emplace_back(turn);
}

GameStatus Game::play_game() {
    // Display title.
    m_turns.clear();
    auto color = WHITE;
    GameStatus status = ONGOING;

    while (!status) {
        cout << endl << "Current board" << endl;
        m_board.draw_board({}, color, m_turn);
        turn(status, color);
        color = color ? BLACK : WHITE;
    }

    cout << endl << "Final board" << endl;
    m_board.draw_board({}, color, m_turn);

    cout << endl << finish_messages.at(status) << endl;

    return status;
}
