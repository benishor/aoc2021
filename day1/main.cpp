#include "solution.h"

class day1 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {
        auto values = aoc::elements_from_stream<int>(in);

        uint32_t part1{0}, part2{0};
        int previous_sum{0}, current_sum{0}, previous_value{0};

        for (size_t i = 0; i < values.size(); i++) {
            if (i > 0 && values[i] > previous_value) {
                part1++;
            }

            current_sum += values[i];
            if (i > 2) {
                current_sum -= values[i - 3];
                if (current_sum > previous_sum) {
                    part2++;
                }
            }
            previous_value = values[i];
            previous_sum = current_sum;
        }

        out << part1 << std::endl;
        out << part2 << std::endl;
    }
};

int main(int argc, char **argv) {
    day1 solution{};
    return solution.execute(argc, argv);
}
