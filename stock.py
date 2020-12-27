import abc
import argparse
import socket
from typing import Optional
from stockfish import Stockfish


class Player(abc.ABC):
    QUIT_MOVE = 'a1a1'

    def __init__(self, first: bool, host: str, port: int, skill_level: Optional[int], depth: Optional[int]):
        # Downloaded from https://stockfishchess.org/download/
        self.player_turn = first
        self.host = host
        self.port = port
        self.skill_level = skill_level
        self.depth = depth
        self.moves = []

        self.stockfish = Stockfish("./stockfish_20090216_x64")
        if self.skill_level is not None:
            self.stockfish.set_skill_level(self.skill_level)
        if self.depth is not None:
            self.stockfish.set_depth(self.depth)

    def choose_move(self):
        self.stockfish.set_position(self.moves)
        move = self.stockfish.get_best_move()
        # todo: make sure check mate is handled well.
        if not move:
            raise RuntimeError('game over')
        print('Move chosen:', move)
        self.socket.send(move.ljust(5, '\0').encode('utf-8'))
        self.moves.append(move)

    def get_move(self):
        move = self.socket.recv(5).decode('utf-8').rstrip('\0')
        if move == self.QUIT_MOVE:
            raise RuntimeError('game over')
        self.moves.append(move)
        print('Move got:', move)

    def turn(self):
        if self.player_turn:
            self.choose_move()
        else:
            self.get_move()

        self.player_turn = not self.player_turn

    def print_board(self):
        self.stockfish.set_position(self.moves)
        print(self.stockfish.get_board_visual())


class Client(Player):
    def __init__(self, first: bool, host: str, port: int, skill_level: Optional[int], depth: Optional[int]):
        super().__init__(first, host, port, skill_level, depth)

        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect((self.host, self.port))

    def __del__(self):
        self.socket.close()


class Server(Player):
    def __init__(self, first: bool, host: str, port: int, skill_level: Optional[int], depth: Optional[int]):
        super().__init__(first, host, port, skill_level, depth)

        accepting_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        accepting_socket.bind((self.host, self.port))
        accepting_socket.listen()
        self.socket = accepting_socket.accept()[0]
        accepting_socket.close()

    def __del__(self):
        self.socket.close()


def play(white: Optional[str], black: Optional[str], skill_level: Optional[int], depth: Optional[int]):
    first = bool(white)
    definition = (white or black).split(':')

    player = (Client if definition[0] == 'client' else Server) \
        (first, definition[1], int(definition[2]), skill_level, depth)
    while True:
        try:
            player.turn()
        except RuntimeError:
            break
        finally:
            player.print_board()


def main():
    parser = argparse.ArgumentParser(description='Resolve changes and dependencies between two branches.')

    parser.add_argument('--white', dest='white', required=False, type=str, help='Stockfish player (white) definition.')
    parser.add_argument('--black', dest='black', required=False, type=str, help='Stockfish player (white) definition.')
    parser.add_argument('--skill-level', dest='skill_level', required=False, type=int,
                        help='Stockfish\'s engine skill level.')
    parser.add_argument('--depth', dest='depth', required=False, type=int, help='Stockfish\'s engine depth.')

    args = parser.parse_args()
    if not args.white and not args.black:
        parser.error('"white" or "black" must be satisfied')
    if args.white and args.black:
        parser.error('"white" and "black" cannot be used together')

    play(args.white, args.black, args.skill_level, args.depth)


if __name__ == '__main__':
    main()
