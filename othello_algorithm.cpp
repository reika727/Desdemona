#include "othello_algorithm.hpp"
#include "othello.hpp"
#include <algorithm>
#include <limits>
#include <stdexcept>
int alpha_beta(const othello::board &b, const othello::stone &s, unsigned int depth, int alpha, int beta, int depth_now)
{
    if (depth_now == depth || b.get_puttable_places(s).empty()) {
        int weight[8][8] = {
            {120, -20, 20, 5, 5, 20, -20, 120},
            {-20, -40, -5, -5, -5, -5, -40, -20},
            {20, -5, 15, 3, 3, 15, -5, 20},
            {5, -5, 3, 3, 3, 3, -5, 5},
            {5, -5, 3, 3, 3, 3, -5, 5},
            {20, -5, 15, 3, 3, 15, -5, 20},
            {-20, -40, -5, -5, -5, -5, -40, -20},
            {120, -20, 20, 5, 5, 20, -20, 120},
        };
        int ret = 0;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                ret += weight[i][j] * (b(j, i) == s ? 1 : b(j, i) == s.get_reversed() ? -1 : 0);
            }
        }
        return ret;
    } else if (depth_now % 2 == 0) {
        for (const auto &p : b.get_puttable_places(s.get_reversed())) {
            beta = std::min(beta, alpha_beta(b.get_put(p, s.get_reversed()), s, depth, alpha, beta, depth_now + 1));
            if (alpha >= beta) break;
        }
        return beta;
    } else {
        for (const auto &p : b.get_puttable_places(s)) {
            alpha = std::max(alpha, alpha_beta(b.get_put(p, s), s, depth, alpha, beta, depth_now + 1));
            if (alpha >= beta) break;
        }
        return alpha;
    }
}
othello::board::coordinate play_best_hand(othello::board &b, const othello::stone &s, unsigned int depth)
{
    auto gp = b.get_puttable_places(s);
    if (gp.empty()) {
        throw std::runtime_error("no puttable place");
    }
    auto c = gp[0];
    auto score = std::numeric_limits<int>::min();
    for (const auto &p : gp) {
        if (auto new_score = alpha_beta(b.get_put(p, s), s, depth); new_score > score) {
            c = p;
        }
    }
    b.put(c, s);
    return c;
}
