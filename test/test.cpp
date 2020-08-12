#include "../othello.hpp"
#include <algorithm>
#include <iostream>
#include <limits>
int alpha_beta(const othello::board &b, const othello::stone &s, unsigned int depth, int alpha = std::numeric_limits<int>::min(), int beta = std::numeric_limits<int>::max(), int depth_now = 0)
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
void draw_othelloboard(const othello::board &b)
{
    std::cout << " abcdefgh" << std::endl;
    for (int y = 0; y < 8; ++y) {
        std::cout << y + 1;
        for (int x = 0; x < 8; ++x) {
            if (auto s = b(x, y)) {
                std::cout << (s == othello::stone::black() ? 'b' : 'w');
            } else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
}
int main()
{
    constexpr bool is_player_first = true;
    constexpr int reading_depth = 4;
    const auto s = is_player_first ? othello::stone::black() : othello::stone::white();
    othello::board b;
    draw_othelloboard(b);
    std::cout << "--------" << std::endl;
    while (true) {
        bool player_passed = false;
        bool enemy_passed = false;
        if (!is_player_first) {
            std::cout << "enemy thinking..." << std::endl;
            try {
                auto c = play_best_hand(b, othello::stone::black(), reading_depth);
                std::cout << "enemy: " << c.x + 'a' << c.y + 1 << std::endl;
            } catch (...) {
                enemy_passed = true;
                std::cout << "enemy passed." << std::endl;
            }
            draw_othelloboard(b);
            std::cout << "--------" << std::endl;
        }
        if (auto puttable = b.get_puttable_places(s); !puttable.empty()) {
            while (true) {
                char c;
                int n;
                std::cout << "> ";
                std::cin >> c >> n;
                if (('a' <= c && c <= 'h' || 'A' <= c && c <= 'H') && 1 <= n && n <= 8) {
                    if (c <= 'H') {
                        c = c - 'A' + 'a';
                    }
                    if (int x = c - 'a', y = n - 1; std::count(puttable.begin(), puttable.end(), othello::board::coordinate{x, y})) {
                        b.put({x, y}, s);
                        break;
                    }
                }
                std::cout << "impossible hand." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        } else {
            player_passed = true;
            std::cout << "can put stone nowhere. passed." << std::endl;
        }
        draw_othelloboard(b);
        std::cout << "--------" << std::endl;
        if (is_player_first) {
            std::cout << "enemy thinking..." << std::endl;
            try {
                auto c = play_best_hand(b, othello::stone::white(), reading_depth);
                std::cout << "enemy: " << static_cast<char>(c.x + 'a') << c.y + 1 << std::endl;
            } catch (...) {
                enemy_passed = true;
                std::cout << "enemy passed." << std::endl;
            }
            draw_othelloboard(b);
            std::cout << "--------" << std::endl;
        }
        if (player_passed && enemy_passed) break;
    }
    int count_player = b.count(s), count_enemy = b.count(s.get_reversed());
    std::cout << count_player << ':' << count_enemy << std::endl;
    if (count_player > count_enemy) {
        std::cout << "YOU WIN" << std::endl;
    } else if (count_player < count_enemy) {
        std::cout << "ENEMY WIN" << std::endl;
    } else {
        std::cout << "DRAW" << std::endl;
    }
}
