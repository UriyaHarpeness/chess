import socket
from stockfish import Stockfish

stockfish = Stockfish("/c/Users/UriyaHarpeness/Downloads/stockfish_20090216_x64")

HOST = '127.0.0.1'
PORT = 6666

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
moves = []
while True:
    move = s.recv(5).decode('utf-8').rstrip('\0')
    if not move:
        break

    moves.append(move)
    print(move)
    stockfish.set_position(moves)
    move = stockfish.get_best_move()
    if not move:
        break

    print(move)
    s.send(move.ljust(5, '\0').encode('utf-8'))
    moves.append(move)

s.close()

print(stockfish.get_board_visual())
