#ifndef _OTHELLO_H
#define _OTHELLO_H
#include <optional>
#include <vector>
namespace othello {
    class stone final {
    public:
        static stone black();
        static stone white();
        stone get_reversed() const;
        bool operator==(const stone &s) const;
        bool operator!=(const stone &s) const;

    private:
        enum class color {
            black,
            white,
        } c;
        stone(color c);
    };
    class board final {
    public:
        struct coordinate {
            int x, y;
            bool operator==(const coordinate &c) const;
            bool operator!=(const coordinate &c) const;
        };
        board();
        int count(const std::optional<stone> &s) const;
        std::vector<coordinate> get_puttable_places(const stone &s) const;
        board put(const coordinate &c, const stone &s);
        board get_put(const coordinate &c, const stone &s) const;
        std::optional<stone> operator()(std::size_t x, std::size_t y) const;

    private:
        std::vector<std::vector<std::optional<stone>>> data;
        std::vector<coordinate> would_be_reversed(const coordinate &c, const stone &s) const;
    };
}
#endif
