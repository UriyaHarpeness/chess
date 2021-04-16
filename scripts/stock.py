import abc
import argparse
import socket
import struct
from typing import List, Optional

from stockfish import Stockfish


class Player(abc.ABC):
    """
    An abstract class for player.
    """
    QUIT_MOVE = 'a1a1'

    def __init__(self, first: bool, host: str, port: int, skill_level: Optional[int], depth: Optional[int],
                 stockfish: Optional[str]):
        """
        Initialize a player.

        Args:
            first: Is the player going first.
            host: The host of the game.
            port: The port in the host of the game.
            skill_level: Still level for stockfish.
            depth: Depth to search for best move for stockfish.
            stockfish: Path to the stockfish executable to use.
        """
        self.player_turn = first
        self.host = host
        self.port = port
        self.skill_level = skill_level
        self.depth = depth
        self.moves = []
        self.socket = None

        self.stockfish = Stockfish(stockfish or "./stockfish")
        if self.skill_level is not None:
            self.stockfish.set_skill_level(self.skill_level)
        if self.depth is not None:
            self.stockfish.set_depth(self.depth)

    def get_turns(self) -> List[str]:
        """
        Get previous turns to the game from the other player, gets the board to the desired initial state.

        Returns:
            The pre turns set by the other player.
        """
        moves_num = struct.unpack('H', self.socket.recv(2))[0]
        moves = []
        for _ in range(moves_num):
            moves.append(self.socket.recv(5).decode('utf-8').rstrip('\0'))
        return moves

    def forward_turns(self, moves: Optional[List[str]] = None):
        """
        Send previous turns to the game to the other player.

        Args:
            moves: The pre turns set by this player.
        """
        if moves is None:
            moves = []

        self.socket.send(struct.pack('H', len(moves)))
        for move in moves:
            self.socket.send(move.ljust(5, '\0').encode('utf-8'))

    def set_turns(self, moves: List[str]):
        """
        Set the pre turns of the game.

        Args:
            moves: The pre turns of the game.
        """
        self.moves = moves
        self.player_turn ^= len(moves) % 2

    def choose_move(self):
        """
        Choose a move to perform.
        Chooses the move using stockfish and sends it to the other player.
        """
        self.stockfish.set_position(self.moves)
        move = self.stockfish.get_best_move()
        if not move:
            raise RuntimeError('game over')
        print('Move chosen:', move)
        self.socket.send(move.ljust(5, '\0').encode('utf-8'))
        self.moves.append(move)

    def get_move(self):
        """
        Get the move done by the other player.
        """
        move = self.socket.recv(5).decode('utf-8').rstrip('\0')
        if move == self.QUIT_MOVE:
            raise RuntimeError('game over')
        self.moves.append(move)
        print('Move got:', move)

    def turn(self):
        """
        Perform a single turn, either get the move done by the other player, or do a move.
        """
        if self.player_turn:
            self.choose_move()
        else:
            self.get_move()

        self.player_turn = not self.player_turn

    def print_board(self):
        """
        Print the current board's state.
        """
        self.stockfish.set_position(self.moves)
        print(self.stockfish.get_board_visual())

    def __del__(self):
        """
        Destruct a player.
        Disconnects the socket.
        """
        self.socket and self.socket.close()


class Client(Player):
    """
    A client player.
    """

    def __init__(self, first: bool, host: str, port: int, skill_level: Optional[int], depth: Optional[int],
                 stockfish: Optional[str]):
        """
        Initialize a client player.
        Connects to the other player, which is expected to be a server.

        See Also: Player.__init__
        """
        super().__init__(first, host, port, skill_level, depth, stockfish)

        # Connect to a server.
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect((self.host, self.port))


class Server(Player):
    """
    A server player.
    """

    def __init__(self, first: bool, host: str, port: int, skill_level: Optional[int], depth: Optional[int],
                 stockfish: Optional[str]):
        """
        Initialize a server player.
        Accepts a connection from the other player, which is expected to be a client.

        See Also: Player.__init__
        """
        super().__init__(first, host, port, skill_level, depth, stockfish)

        # Accept and connect to a client.
        accepting_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        accepting_socket.bind((self.host, self.port))
        accepting_socket.listen()
        self.socket = accepting_socket.accept()[0]
        accepting_socket.close()


def play(white: Optional[str], black: Optional[str], skill_level: Optional[int], depth: Optional[int],
         turns: Optional[str], stockfish: Optional[str]):
    """
    Play a full game.

    Args:
        white: Definition for a white player.
        black: Definition for a black player.
        skill_level: Still level for stockfish.
        depth: Depth to search for best move for stockfish.
        turns: The pre turns set by this player.
        stockfish: Path to the stockfish executable to use.
    """
    first = bool(white)
    definition = (white or black).split(':')

    player = (Client if definition[0] == 'client' else Server) \
        (first, definition[1], int(definition[2]), skill_level, depth, stockfish)

    turns = turns and turns.split(' ')
    player.forward_turns(turns)
    player.forward_turns()
    first_turns = player.get_turns()
    second_turns = player.get_turns()
    turns_options = []
    if turns:
        turns_options.append(turns)
    if first_turns:
        turns_options.append(first_turns)
    if second_turns:
        turns_options.append(second_turns)
    if len(turns_options) > 1:
        raise RuntimeError('Too many players setting turns (max 1)')

    player.set_turns(turns_options and turns_options[0])
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
    parser.add_argument('--stockfish', dest='stockfish', required=False, type=str,
                        help='Path to the stockfish\'s executable.')

    args = parser.parse_args()
    if not args.white and not args.black:
        parser.error('"white" or "black" must be satisfied')
    if args.white and args.black:
        parser.error('"white" and "black" cannot be used together')

    play(args.white, args.black, args.skill_level, args.depth, args.turns, args.stockfish)


if __name__ == '__main__':
    main()
