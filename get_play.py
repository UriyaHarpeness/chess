import argparse
import chess.pgn
import concurrent.futures
import io
import multiprocessing
import random
import requests
from typing import Optional


def try_to_find_game():
    try:
        random_game_id = random.randint(1, 2 ** 32)
        game = io.StringIO(
            requests.post(url='https://www.chessgames.com/perl/nph-chesspgn',
                          data=f'text=1&gid={random_game_id}').text)
        print(f'Retrieved game {random_game_id}.')
        return game
    except requests.exceptions.ConnectionError:
        return None


def get_chess_game(game_id: Optional[int]):
    if game_id is not None:
        game = io.StringIO(
            requests.post(url='https://www.chessgames.com/perl/nph-chesspgn',
                          data=f'text=1&gid={game_id if game_id is not None else random_game_id}').text)
    else:
        game = None
        with concurrent.futures.ThreadPoolExecutor() as executor:
            while game is None:
                futures = [executor.submit(try_to_find_game) for _ in range(multiprocessing.cpu_count())]
                games = list(filter(None, [f.result() for f in futures]))
                if len(games) > 0:
                    game = games[0]

    print(' '.join(move.uci() for move in chess.pgn.read_game(game).mainline_moves()))


def main():
    parser = argparse.ArgumentParser(description='Retrieve chess game.')

    parser.add_argument('--id', dest='game_id', type=int, required=False, help='Game ID (random if not satisfied)')

    args = parser.parse_args()

    if args.game_id is None:
        print(f'Retrieving random chess game.')
    else:
        print(f'Retrieving chess game {args.game_id}.')

    get_chess_game(args.game_id)


if __name__ == '__main__':
    main()
