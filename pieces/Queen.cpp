#include "Queen.h"

// Vertically, horizontally, and diagonally.
const set<Point> Queen::move_vector = {{1,  0},
                                       {0,  1},
                                       {-1, 0},
                                       {0,  -1},
                                       {1,  1},
                                       {-1, 1},
                                       {-1, -1},
                                       {1,  -1}};
