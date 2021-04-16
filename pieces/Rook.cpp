#include "Rook.h"

// Horizontally or vertically.
const set<Point> Rook::move_vector = {{1,  0},
                                      {0,  1},
                                      {-1, 0},
                                      {0,  -1}};
