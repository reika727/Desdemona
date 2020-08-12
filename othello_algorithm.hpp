#ifndef _OTHELLO_ALGORITHM_H
#define _OTHELLO_ALGORITHM_H
#include "othello.hpp"
#include <limits>
int alpha_beta(const othello::board &b, const othello::stone &s, unsigned int depth, int alpha = std::numeric_limits<int>::min(), int beta = std::numeric_limits<int>::max(), int depth_now = 0);
othello::board::coordinate play_best_hand(othello::board &b, const othello::stone &s, unsigned int depth);
#endif
