#include "solution.h"

class day7 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {
        auto numbers = aoc::csv_elements_from_stream<uint16_t>(in);
        std::sort(std::begin(numbers), std::end(numbers));

        uint64_t min_fuel = std::numeric_limits<uint64_t>::max();
        for (auto i = numbers.front(); i <= numbers.back(); i++) {
            auto cost = calculate_fuel_cost(numbers, i);
            if (cost < min_fuel) {
                min_fuel = cost;
            }
        }
        out << min_fuel << std::endl;

        min_fuel = std::numeric_limits<uint64_t>::max();
        for (auto i = numbers.front(); i <= numbers.back(); i++) {
            auto cost = calculate_fuel_cost2(numbers, i);
            if (cost < min_fuel) {
                min_fuel = cost;
            }
        }
        out << min_fuel << std::endl;
    }

    static uint64_t calculate_fuel_cost(const std::vector<uint16_t>& numbers, uint16_t median) {
        uint64_t result{0};
        for (auto& x: numbers) {
            result += abs(x - median);
        }
        return result;
    }

    static uint64_t calculate_fuel_cost2(const std::vector<uint16_t>& numbers, uint16_t median) {
        uint64_t result{0};
        for (auto& x: numbers) {
            auto n = abs(x - median);
            result += n * (n + 1) / 2;
        }
        return result;
    }
};

int main(int argc, char **argv) {
    day7 solution{};
    return solution.execute(argc, argv);
}
