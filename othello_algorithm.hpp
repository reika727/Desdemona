#ifndef _OTHELLO_ALGORITHM_H
#define _OTHELLO_ALGORITHM_H
#include "othello.hpp"
#include <limits>
// αβ法の実装
int alpha_beta(const othello::board &b, const othello::stone &s, unsigned int depth, int alpha = std::numeric_limits<int>::min(), int beta = std::numeric_limits<int>::max(), unsigned int depth_now = 0);
// αβ法に従って最善手を打つ
othello::board::coordinate play_best_hand(othello::board &b, const othello::stone &s, unsigned int depth);
#endif
