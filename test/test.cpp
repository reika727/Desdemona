#include "../othello.hpp"
#include "../othello_algorithm.hpp"
#include <algorithm>
#include <iostream>
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
