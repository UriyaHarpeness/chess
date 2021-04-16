#include "Bishop.h"

// Diagonally.
const set<Point> Bishop::move_vector = {{1,  1},
                                        {-1, 1},
                                        {-1, -1},
                                        {1,  -1}};
