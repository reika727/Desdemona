#include "othello.hpp"
#include <algorithm>
#include <numeric>
#include <stdexcept>
othello::stone othello::stone::black()
{
    return {color::black};
}
othello::stone othello::stone::white()
{
    return {color::white};
}
othello::stone othello::stone::get_reversed() const
{
    return {c == othello::stone::color::black ? othello::stone::color::white : othello::stone::color::black};
}
bool othello::stone::operator==(const othello::stone &s) const
{
    return c == s.c;
}
bool othello::stone::operator!=(const othello::stone &s) const
{
    return !operator==(s);
}
othello::stone::stone(othello::stone::color c)
    : c(c)
{
}
bool othello::board::coordinate::operator==(const othello::board::coordinate &c) const
{
    return x == c.x && y == c.y;
}
bool othello::board::coordinate::operator!=(const othello::board::coordinate &c) const
{
    return !operator==(c);
}
othello::board::board()
    : data(8, std::vector<std::optional<othello::stone>>(8, std::nullopt))
{
    data[3][4] = data[4][3] = othello::stone::black();
    data[3][3] = data[4][4] = othello::stone::white();
}
int othello::board::count(const std::optional<othello::stone> &s) const
{
    return std::accumulate(data.begin(), data.end(), 0,
                           [s](int acc, const std::vector<std::optional<othello::stone>> &v) {
                               return acc + std::count(v.begin(), v.end(), s);
                           });
}
std::vector<othello::board::coordinate> othello::board::get_puttable_places(const othello::stone &s) const
{
    std::vector<othello::board::coordinate> ret;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (!would_be_reversed({x, y}, s).empty()) {
                ret.push_back({x, y});
            }
        }
    }
    return ret;
}
othello::board othello::board::put(const othello::board::coordinate &c, const othello::stone &s)
{
    auto wbr = would_be_reversed(c, s);
    if (wbr.empty()) {
        throw std::runtime_error("can't put this stone here");
    }
    data[c.y][c.x] = s;
    for (const auto &w : wbr) {
        data[w.y][w.x] = data[w.y][w.x]->get_reversed();
    }
    return *this;
}
othello::board othello::board::get_put(const othello::board::coordinate &c, const othello::stone &s) const
{
    othello::board ret = *this;
    return ret.put(c, s);
}
std::optional<othello::stone> othello::board::operator()(std::size_t x, std::size_t y) const
{
    if (x >= 8 || y >= 8) throw std::out_of_range("out of range");
    return data[y][x];
}
std::vector<othello::board::coordinate> othello::board::would_be_reversed(const othello::board::coordinate &c, const othello::stone &s) const
{
    std::vector<othello::board::coordinate> ret;
    if (data[c.y][c.x]) return ret;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dy == 0 && dx == 0) continue;
            for (int y = c.y + dy, x = c.x + dx; 0 <= y && y < 8 && 0 <= x && x < 8; y += dy, x += dx) {
                if (!data[y][x]) break;
                if (data[y][x] == s) {
                    for (int yy = y - dy, xx = x - dx; yy != c.y || xx != c.x; yy -= dy, xx -= dx) {
                        ret.push_back({xx, yy});
                    }
                    break;
                }
            }
        }
    }
    return ret;
}
