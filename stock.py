import socket
from stockfish import Stockfish

# Downloaded from https://stockfishchess.org/download/
stockfish = Stockfish("/c/Users/UriyaHarpeness/Downloads/stockfish_20090216_x64")

HOST = '0.0.0.0'
PORT = 6666

# get string of set and get to support black and white all together

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen()
conn, addr = s.accept()
moves = []
print('Connected by', addr)
while True:
    stockfish.set_position(moves)
    move = stockfish.get_best_move()
    if not move:
        break

    print(move)
    conn.send(move.ljust(5, '\0').encode('utf-8'))
    moves.append(move)
    move = conn.recv(5).decode('utf-8').rstrip('\0')
    if not move:
        break

    moves.append(move)
    print(move)

s.close()
conn.close()

print(stockfish.get_board_visual())
