#include "Knight.h"

const set<Point> Knight::move_relative = {{1,  2},
                                          {1,  -2},
                                          {2,  1},
                                          {2,  -1},
                                          {-1, 2},
                                          {-1, -2},
                                          {-2, 1},
                                          {-2, -1}};
