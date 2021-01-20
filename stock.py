import abc
import argparse
import socket
import struct
from typing import List, Optional
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

    def get_moves(self) -> List[str]:
        moves_num = struct.unpack('H', self.socket.recv(2))[0]
        moves = []
        for _ in range(moves_num):
            moves.append(self.socket.recv(5).decode('utf-8').rstrip('\0'))
        return moves

    def forward_moves(self, moves: Optional[List[str]] = None):
        if moves is None:
            moves = []

        self.socket.send(struct.pack('H', len(moves)))
        for move in moves:
            self.socket.send(move.ljust(5, '\0').encode('utf-8'))

    def set_moves(self, moves: List[str]):
        self.moves = moves
        self.player_turn ^= len(moves) % 2

    def choose_move(self):
        self.stockfish.set_position(self.moves)
        move = self.stockfish.get_best_move()
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


def play(white: Optional[str], black: Optional[str], skill_level: Optional[int], depth: Optional[int],
         turns: Optional[str]):
    first = bool(white)
    definition = (white or black).split(':')

    player = (Client if definition[0] == 'client' else Server) \
        (first, definition[1], int(definition[2]), skill_level, depth)

    turns = turns and turns.split(' ')
    player.forward_moves(turns)
    player.forward_moves()
    first_turns = player.get_moves()
    second_turns = player.get_moves()
    turns_options = []
    if turns:
        turns_options.append(turns)
    if first_turns:
        turns_options.append(first_turns)
    if second_turns:
        turns_options.append(second_turns)
    if len(turns_options) > 1:
        raise RuntimeError('Too many players setting turns (max 1)')

    player.set_moves(turns_options[0])
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
    parser.add_argument('--black', dest='black', required=False, type=str, help='Stockfish player (black) definition.')
    parser.add_argument('--skill-level', dest='skill_level', required=False, type=int,
                        help='Stockfish\'s engine skill level.')
    parser.add_argument('--depth', dest='depth', required=False, type=int, help='Stockfish\'s engine depth.')
    parser.add_argument('--turns', dest='turns', required=False, type=str, help='Turns to start the game with.')

    args = parser.parse_args()
    if not args.white and not args.black:
        parser.error('"white" or "black" must be satisfied')
    if args.white and args.black:
        parser.error('"white" and "black" cannot be used together')

    play(args.white, args.black, args.skill_level, args.depth, args.turns)


if __name__ == '__main__':
    main()
