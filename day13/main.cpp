#include "solution.h"

struct point {
    int x, y;
};

struct fold {
    char axis;
    int value;
};

struct data {
    std::vector<point> points;
    std::deque<fold> folds;
};

inline bool operator<(const point& a, const point& b) {
    return a.y < b.y || (a.y == b.y && a.x < b.x);
}

static point transform(const point& p, const fold& f) {
    switch (f.axis) {
        case 'x': {
            int new_x = p.x < f.value ? p.x : f.value - (p.x - f.value);
            int new_y = p.y;
            return {new_x, new_y};
        }
        case 'y': {
            int new_y = p.y < f.value ? p.y : f.value - (p.y - f.value);
            int new_x = p.x;
            return {new_x, new_y};
        }
        default:
            return p;
    }
}

class day13 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {
        auto data = parse_data(in);

        // part 1
        std::set<point> points_transformed_once;
        for (auto& p: data.points) {
            points_transformed_once.insert(transform(p, data.folds[0]));
        }
        out << points_transformed_once.size() << std::endl;
        data.folds.pop_front(); // no need for the first fold rule anymore

        // part 2
        std::set<point> transformed_points = std::move(points_transformed_once);
        for (auto& f: data.folds) {
            std::set<point> new_points;
            for (auto& p: transformed_points) {
                new_points.insert(transform(p, f));
            }
            transformed_points = new_points;
        }

        print_points(transformed_points, out);
    }

    static data parse_data(std::istream& in) {
        data result;
        auto fold_re = std::regex(R"(^fold along (.)=(\d+)$)");

        for (std::string line; std::getline(in, line);) {
            std::smatch sm{};
            if (std::regex_match(line, sm, fold_re)) {
                result.folds.emplace_back(fold{sm.str(1).front(), std::stoi(sm.str(2))});
            } else {
                auto pieces = aoc::split_string(line, ",");
                if (pieces.size() > 1) {
                    result.points.emplace_back(point{std::stoi(pieces[0]), std::stoi(pieces[1])});
                }
            }
        }
        return result;
    }

    static void print_points(const std::set<point>& transformed_points, std::ostream& out) {
        point max_bounds{0, 0};
        for (auto& p: transformed_points) {
            max_bounds = std::max(max_bounds, p);
        }

        const int width = max_bounds.x + 1;
        const int height = max_bounds.y + 1;

        char buffer[width * height];
        memset(buffer, '.', width * height);
        for (auto& p: transformed_points) {
            buffer[p.y * width + p.x] = '#';
        }

        size_t offset{0};
        for (int y = 0; y < height; y++) {
            out.write(&buffer[offset], width);
            offset += width;
            out << std::endl;
        }
    }
};

int main(int argc, char **argv) {
    day13 solution{};
    return solution.execute(argc, argv);
}
