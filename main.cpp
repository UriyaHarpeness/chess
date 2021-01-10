#include "game/Game.h"
#include "players/Client.h"
#include "players/Local.h"
#include "players/Server.h"

using namespace std;

unique_ptr<Player> create_player(const string &argument) {
    vector<string> split_arguments;
    int index = 0, new_index;
    do {
        new_index = argument.find(':', index);
        split_arguments.emplace_back(argument.substr(index, (new_index > 0 ? new_index : argument.length()) - index));
        index = new_index + 1;
    } while (new_index != -1);

    if (split_arguments[0] == "local") {
        return make_unique<Local>();
    }
    if (split_arguments[0] == "client") {
        return make_unique<Client>(split_arguments[1], atoi(split_arguments[2].c_str()));
    }
    if (split_arguments[0] == "server") {
        return make_unique<Server>(split_arguments[1], atoi(split_arguments[2].c_str()));
    }

    throw runtime_error("Unknown player " + split_arguments[0]);
}

int main(int argc, char *argv[]) {
    // Set default game settings.
    bool walk_through = false;
    string turns;
    unique_ptr<Player> white;
    unique_ptr<Player> black;

    // Parse arguments.
    vector<string> arguments(argc - 1);
    for (int i = 1; i < argc; i++) {
        arguments[i - 1] = argv[i];
    }
    for (int index = 0; index < argc - 1; index++) {
        if (arguments[index] == "--white") {
            white = create_player(arguments[index + 1]);
            index++;
        } else if (arguments[index] == "--black") {
            black = create_player(arguments[index + 1]);
            index++;
        } else if (arguments[index] == "--turns") {
            turns = arguments[index + 1];
            index++;
        } else if (arguments[index] == "--walk-through") {
            walk_through = true;
        } else {
            throw runtime_error("Unknown argument " + arguments[index]);
        }
    }

    // Fallback default players if not specified.
    if (white == nullptr) {
        white = create_player("local");
    }
    if (black == nullptr) {
        black = create_player("local");
    }

    // Initialize the game.
    Game game(move(white), move(black), walk_through, turns);

    // Play.
    game.play_game();

    return 0;
}
