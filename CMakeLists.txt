cmake_minimum_required(VERSION 3.10)
project(chess)

set(CMAKE_CXX_STANDARD 17)

add_executable(chess main.cpp
        board/Board.cpp
        board/Board.h
        config.h
        game/Game.cpp
        game/Game.h
        multipiece/MultiPiece.cpp
        multipiece/MultiPiece.h
        pieces/Bishop.cpp
        pieces/Bishop.h
        pieces/King.cpp
        pieces/King.h
        pieces/Knight.cpp
        pieces/Knight.h
        pieces/Pawn.cpp
        pieces/Pawn.h
        pieces/Piece.cpp
        pieces/Piece.h
        pieces/Queen.cpp
        pieces/Queen.h
        pieces/Rock.cpp
        pieces/Rock.h
        point/Point.cpp
        point/Point.h)
