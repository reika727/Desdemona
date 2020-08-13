#ifndef _OTHELLO_H
#define _OTHELLO_H
#include <optional>
#include <stdexcept>
#include <vector>
namespace othello {
    // オセロの石
    class stone final {
    public:
        // 黒い石のファクトリ
        static stone black();
        // 白い石のファクトリ
        static stone white();
        // ひっくり返された石を返す
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
    // オセロ盤
    class board final {
    public:
        // 盤上の座標
        struct coordinate final {
            int x, y;
            bool operator==(const coordinate &c) const;
            bool operator!=(const coordinate &c) const;
        };
        // 不正な操作の時投げる例外
        class operation_error final : public std::runtime_error {
            using std::runtime_error::runtime_error;
        };
        board();
        // 盤上の石の個数を数える
        int count(const std::optional<stone> &s) const;
        // 石を置ける場所の一覧を返す
        std::vector<coordinate> get_puttable_places(const stone &s) const;
        // 石を置く
        board put(const coordinate &c, const stone &s);
        // 石を置いた時のコピーを返す
        board get_put(const coordinate &c, const stone &s) const;
        // 盤面へのアクセス
        std::optional<stone> operator()(std::size_t x, std::size_t y) const;

    private:
        // データの本体
        std::vector<std::vector<std::optional<stone>>> data;
        // 石が置かれたらひっくり返るであろう石(の座標)の一覧を返す
        std::vector<coordinate> would_be_reversed(const coordinate &c, const stone &s) const;
    };
}
#endif
