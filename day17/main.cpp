#include "solution.h"

struct vec2 {
    int x, y;
};

vec2 operator+(vec2 a, vec2 b) {
    return {a.x + b.x, a.y + b.y};
}

vec2 step_velocity(vec2 v) {
    vec2 result = v;
    if (result.x > 0) {
        result.x--;
    } else if (result.x < 0) {
        result.x++;
    }
    result.y--;
    return result;
}

static bool position_out_of_bounds(vec2 position, std::pair<int, int> x_bounds, std::pair<int, int> y_bounds) {
    if (position.x > x_bounds.second) return true;
    if (position.y < y_bounds.second) return true;
    return false;
}

static bool position_on_target(vec2 position, std::pair<int, int> x_bounds, std::pair<int, int> y_bounds) {
    if (position.x > x_bounds.second) return false;
    if (position.x < x_bounds.first) return false;
    if (position.y < y_bounds.second) return false;
    if (position.y > y_bounds.first) return false;
    return true;
}

std::ostream& operator<<(std::ostream& out, const vec2& v) {
    out << "{x:" << v.x << ", y:" << v.y << "}";
    return out;
}

class day17 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {

        std::pair<int, int> x_bounds = {29, 73};
        std::pair<int, int> y_bounds = {-194, -248};

        vec2 max_velocity{0, 0};
        int max_velocity_y{0};
        size_t part2{0};
        for (int x = 0; x < 200; x++) {
            for (int y = -250; y < 300; y++) {
                vec2 velocity = {x, y};

                vec2 position = {0, 0};
                int max_y = {0};
                for (;;) {
                    position = position + velocity;
                    velocity = step_velocity(velocity);
                    max_y = std::max(max_y, position.y);

                    if (position_on_target(position, x_bounds, y_bounds)) {
                        part2++;
                        if (max_y > max_velocity_y) {
                            max_velocity_y = max_y;
                            max_velocity = vec2{x, y};
                        }
                        break;
                    } else if (position_out_of_bounds(position, x_bounds, y_bounds)) {
                        break;
                    }
                }
            }
        }

        std::cout << "Max height of " << max_velocity_y << " for velocity: " << max_velocity << std::endl;
        std::cout << part2 << std::endl;
    }
};

int main(int argc, char **argv) {
    day17 solution{};
    return solution.execute(argc, argv);
}
