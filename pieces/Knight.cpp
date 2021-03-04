#include "Knight.h"

// Two squares vertically and one square horizontally, or two squares horizontally and one square vertically.
const set<Point> Knight::move_relative = {{1,  2},
                                          {1,  -2},
                                          {2,  1},
                                          {2,  -1},
                                          {-1, 2},
                                          {-1, -2},
                                          {-2, 1},
                                          {-2, -1}};
