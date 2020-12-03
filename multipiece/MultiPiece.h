#pragma once

#include "../pieces/Pawn.h"
#include "../pieces/Piece.h"

typedef pair<shared_ptr<Piece>, shared_ptr<Piece>> change;
typedef vector<change> play;

class MultiPiece {
public:
    static set<play> get_en_passant(const BoardData &board, Color color, unsigned int turn);

    static set<play> get_plays(const BoardData &board, Color color, unsigned int turn);
};
