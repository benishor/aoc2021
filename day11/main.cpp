#include "solution.h"

class day11 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {
        uint64_t part1{0}, part2{0};
        auto octopuses = parse_data(in);
        
        for (size_t step = 0;; step++) {
            increment_levels(octopuses);
            solve_flashes(octopuses);
            auto this_step_flashes = count_step_flashes(octopuses);
            if (this_step_flashes == 100) {
                part2 = step + 1;
                break;
            }

            if (step < 100) {
                part1 += this_step_flashes;
            }
        }
        out << part1 << std::endl;
        out << part2 << std::endl;
    }

    static std::array<std::array<uint8_t, 10>, 10> parse_data(std::istream& in) {
        std::array<std::array<uint8_t, 10>, 10> octopuses{};
        size_t y{0};
        for (std::string line; std::getline(in, line);) {
            size_t x{0};
            for (auto& c: line) {
                octopuses[y][x++] = c - '0';
            }
            y++;
        }
        return octopuses;
    }

    static void increment_levels(std::array<std::array<uint8_t, 10>, 10>& octopuses) {
        for (size_t y = 0; y < 10; y++) {
            for (size_t x = 0; x < 10; x++) {
                if (octopuses[y][x] < 10) {
                    octopuses[y][x]++;
                }
            }
        }
    }

    void solve_flashes(std::array<std::array<uint8_t, 10>, 10>& octopuses) {
        for (size_t y = 0; y < 10; y++) {
            for (size_t x = 0; x < 10; x++) {
                bool already_flashed = (octopuses[y][x] & 0x80) == 0x80;
                if (!already_flashed && octopuses[y][x] > 9) {
                    flash(octopuses, x, y);
                }
            }
        }
    }

    static void flash(std::array<std::array<uint8_t, 10>, 10>& octopuses, size_t x, size_t y) {
        octopuses[y][x] |= 0x80;
        if (x > 0) increase(octopuses, x - 1, y);
        if (x < 9) increase(octopuses, x + 1, y);
        if (y > 0) increase(octopuses, x, y - 1);
        if (y < 9) increase(octopuses, x, y + 1);
        if (x > 0 && y > 0) increase(octopuses, x - 1, y - 1);
        if (x < 9 && y < 9) increase(octopuses, x + 1, y + 1);
        if (x > 0 && y < 9) increase(octopuses, x - 1, y + 1);
        if (x < 9 && y > 0) increase(octopuses, x + 1, y - 1);
    }

    static void increase(std::array<std::array<uint8_t, 10>, 10>& octopuses, size_t x, size_t y) {
        bool already_flashed = (octopuses[y][x] & 0x80) == 0x80;
        if (already_flashed) return;

        octopuses[y][x]++;
        if (octopuses[y][x] > 9) {
            flash(octopuses, x, y);
        }
    }

    static uint64_t count_step_flashes(std::array<std::array<uint8_t, 10>, 10>& octopi) {
        uint64_t result{0};
        for (size_t y = 0; y < 10; y++) {
            for (size_t x = 0; x < 10; x++) {
                bool already_flashed = (octopi[y][x] & 0x80) == 0x80;
                if (already_flashed) {
                    result++;
                    octopi[y][x] = 0;
                }
            }
        }
        return result;
    }
};

int main(int argc, char **argv) {
    day11 solution{};
    return solution.execute(argc, argv);
}
