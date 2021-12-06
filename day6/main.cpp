#include <numeric>
#include "solution.h"

static unsigned long compute(std::ostream& out, const std::vector<int>& numbers) {
    auto start = std::chrono::steady_clock::now();

    size_t data[18]{};
    size_t *fish_timers, *new_timers;
    fish_timers = &data[0];
    new_timers = &data[9];

    for (auto& n: numbers) {
        fish_timers[n]++;
    }

    for (int day = 0; day < 80; day++) {
        new_timers[0] = new_timers[1] = new_timers[2] = new_timers[3] = new_timers[4] = new_timers[5] = new_timers[6] = new_timers[7] = new_timers[8] = 0;
        for (int t = 0; t < 9; t++) {
            uint64_t count = fish_timers[t];
            if (t == 0) {
                new_timers[6] += count;
                new_timers[8] = count;
            } else {
                new_timers[t - 1] += count;
            }
        }

        std::swap(fish_timers, new_timers);
    }
    out << fish_timers[0] +
           fish_timers[1] +
           fish_timers[2] +
           fish_timers[3] +
           fish_timers[4] +
           fish_timers[5] +
           fish_timers[6] +
           fish_timers[7] +
           fish_timers[8] << std::endl;

    for (int day = 80; day < 256; day++) {
        new_timers[0] = new_timers[1] = new_timers[2] = new_timers[3] = new_timers[4] = new_timers[5] = new_timers[6] = new_timers[7] = new_timers[8] = 0;
        for (int t = 0; t < 9; t++) {
            uint64_t count = fish_timers[t];
            if (t == 0) {
                new_timers[6] += count;
                new_timers[8] = count;
            } else {
                new_timers[t - 1] += count;
            }
        }

        std::swap(fish_timers, new_timers);
    }

    out << fish_timers[0] +
           fish_timers[1] +
           fish_timers[2] +
           fish_timers[3] +
           fish_timers[4] +
           fish_timers[5] +
           fish_timers[6] +
           fish_timers[7] +
           fish_timers[8] << std::endl;

    auto end = std::chrono::steady_clock::now();
    auto ns_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    return ns_taken.count();
}

class day6 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {
        auto numbers = aoc::csv_elements_from_stream<int>(in);

        unsigned long total_time{0};
        for (int i = 0; i < 100; i++) {
            total_time += compute(out, numbers);
        }
        std::cout << "ns taken: " << total_time / 100 << std::endl;
    }
};

int main(int argc, char **argv) {
    day6 solution{};
    return solution.execute(argc, argv);
}
