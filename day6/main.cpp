#include <numeric>
#include "solution.h"

using fish_map = std::map<int, uint64_t>;

class day6 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {
        fish_map fish_timers;
        auto numbers = aoc::csv_elements_from_stream<int>(in);
        for (auto& n: numbers) {
            fish_timers[n]++;
        }

        for (int day = 0; day < 256; day++) {
            fish_map new_timers;
            for (int t = 0; t <= 8; t++) {
                uint64_t count = fish_timers[t];
                if (t == 0) {
                    new_timers[6] += count;
                    new_timers[8] = count;
                } else {
                    new_timers[t - 1] += count;
                }
            }
            fish_timers = new_timers;
            if (day == 79) {
                out << std::accumulate(std::begin(fish_timers),
                                       std::end(fish_timers),
                                       0ULL,
                                       [](uint64_t value, const std::map<int, uint64_t>::value_type& p) -> uint64_t {
                                           return value + p.second;
                                       }) << std::endl;
            }
        }

        out << std::accumulate(std::begin(fish_timers),
                               std::end(fish_timers),
                               0ULL,
                               [](uint64_t value, const std::map<int, uint64_t>::value_type& p) -> uint64_t {
                                   return value + p.second;
                               }) << std::endl;
    }
};

int main(int argc, char **argv) {
    day6 solution{};
    return solution.execute(argc, argv);
}
