#include "solution.h"

class day9 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {

        auto heightmap = parse_data(in);

        uint64_t part1{0};
        std::vector<uint32_t> basins{};

        for (size_t y = 0; y < 100; y++) {
            for (size_t x = 0; x < 100; x++) {
                if (is_local_minimum(heightmap, x, y)) {
                    part1 += 1 + heightmap[y][x];

                    flood_fill_basin(heightmap, x, y);
                    basins.push_back(count_flagged(heightmap));
                    clear_flags(heightmap);
                }
            }
        }
        out << part1 << std::endl;

        std::sort(std::rbegin(basins), std::rend(basins));
        uint64_t part2 = basins[0] * basins[1] * basins[2];
        out << part2 << std::endl;
    }


    static std::array<std::array<uint8_t, 100>, 100> parse_data(std::istream& in) {
        std::array<std::array<uint8_t, 100>, 100> heightmap{};
        size_t y{0};
        for (std::string line; std::getline(in, line);) {
            size_t x{0};
            for (auto& c: line) {
                heightmap[y][x++] = c - '0';
            }
            y++;
        }
        return heightmap;
    }

    static bool is_local_minimum(const std::array<std::array<uint8_t, 100>, 100>& heightmap, size_t x, size_t y) {
        auto val = heightmap[y][x];
        if (y > 0 && heightmap[y - 1][x] <= val) return false;
        if (y < 99 && heightmap[y + 1][x] <= val) return false;
        if (x > 0 && heightmap[y][x - 1] <= val) return false;
        if (x < 99 && heightmap[y][x + 1] <= val) return false;
        return true;
    }

    static void clear_flags(std::array<std::array<uint8_t, 100>, 100>& heightmap) {
        for (size_t y = 0; y < 100; y++) {
            for (size_t x = 0; x < 100; x++) {
                heightmap[y][x] &= 0x7f;
            }
        }
    }

    static void flood_fill_basin(std::array<std::array<uint8_t, 100>, 100>& heightmap, size_t x, size_t y) {
        if (heightmap[y][x] < 9) {
            heightmap[y][x] |= 0x80;
            if (x > 0) flood_fill_basin(heightmap, x - 1, y);
            if (x < 99) flood_fill_basin(heightmap, x + 1, y);
            if (y > 0) flood_fill_basin(heightmap, x, y - 1);
            if (y < 99) flood_fill_basin(heightmap, x, y + 1);
        }
    }

    static uint64_t count_flagged(const std::array<std::array<uint8_t, 100>, 100>& heightmap) {
        uint64_t result{0};
        for (size_t y = 0; y < 100; y++) {
            for (size_t x = 0; x < 100; x++) {
                if ((heightmap[y][x] & 0x80) == 0x80)
                    result++;
            }
        }
        return result;
    }
};

int main(int argc, char **argv) {
    day9 solution{};
    return solution.execute(argc, argv);
}
