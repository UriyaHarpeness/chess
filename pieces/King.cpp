#include "King.h"

// One square in any direction.
const set<Point> King::move_relative = {{1,  0},
                                        {0,  1},
                                        {-1, 0},
                                        {0,  -1},
                                        {1,  1},
                                        {-1, 1},
                                        {-1, -1},
                                        {1,  -1}};
