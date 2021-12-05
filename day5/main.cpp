#include "solution.h"

struct line {
    uint16_t x1, y1;
    uint16_t x2, y2;

    [[nodiscard]] bool is_vertical() const {
        return x1 == x2;
    }

    [[nodiscard]] bool is_horizontal() const {
        return y1 == y2;
    }

    [[nodiscard]] bool is_diagonal() const {
        return x1 != x2 && y1 != y2;
    }
};

struct canvas {
    std::array<std::array<uint8_t, 1000>, 1000> buffer{};

    void draw(const line& l) {
        if (l.is_horizontal()) {
            auto start = l.x1 <= l.x2 ? l.x1 : l.x2;
            auto end = l.x1 <= l.x2 ? l.x2 : l.x1;
            for (int x = start; x <= end; x++) {
                buffer[l.y1][x]++;
            }
        } else if (l.is_vertical()) {
            auto start = l.y1 <= l.y2 ? l.y1 : l.y2;
            auto end = l.y1 <= l.y2 ? l.y2 : l.y1;

            for (int y = start; y <= end; y++) {
                buffer[y][l.x1]++;
            }
        } else {
            int dx = (l.x2 - l.x1) > 0 ? 1 : -1;
            int dy = (l.y2 - l.y1) > 0 ? 1 : -1;
            auto x = l.x1;
            auto y = l.y1;
            for (;; x += dx, y += dy) {
                buffer[y][x]++;
                if (x == l.x2 && y == l.y2) break;
            }
        }
    }

    uint32_t count_overlaps(uint8_t threshold) {
        uint32_t result{0};
        for (size_t y = 0; y < 1000; y++) {
            for (size_t x = 0; x < 1000; x++) {
                if (buffer[y][x] >= threshold) {
                    result++;
                }
            }
        }
        return result;
    }
};

class day5 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {
        canvas panel{};
        auto lines = parse_lines(in);

        for (auto& l: lines) {
            if (!l.is_diagonal())
                panel.draw(l);
        }
        out << panel.count_overlaps(2) << std::endl;

        for (auto& l: lines) {
            if (l.is_diagonal())
                panel.draw(l);
        }
        out << panel.count_overlaps(2) << std::endl;
    }

    static std::vector<line> parse_lines(std::istream& in) {
        std::vector<line> result{};

        line l{};
        char comma, arrow;
        while (true) {
            in >> l.x1;
            if (in.eof()) break; // account for the last empty line
            in >> comma >> l.y1 >> arrow >> arrow >> l.x2 >> comma >> l.y2;
            result.emplace_back(l);
        }
        return result;
    }
};

int main(int argc, char **argv) {
    day5 solution{};
    return solution.execute(argc, argv);
}
