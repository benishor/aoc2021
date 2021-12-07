#include "solution.h"

class day7 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {

        auto numbers = aoc::csv_elements_from_stream<uint16_t>(in);
        std::sort(std::begin(numbers), std::end(numbers));

        uint64_t min_fuel_1 = std::numeric_limits<uint64_t>::max();
        uint64_t min_fuel_2 = std::numeric_limits<uint64_t>::max();

        for (auto i = numbers.front(); i <= numbers.back(); i++) {
            auto cost_1 = calculate_fuel_cost_1(numbers, i);
            auto cost_2 = calculate_fuel_cost_2(numbers, i);
            if (cost_1 < min_fuel_1) {
                min_fuel_1 = cost_1;
            }
            if (cost_2 < min_fuel_2) {
                min_fuel_2 = cost_2;
            }
        }
        
        out << min_fuel_1 << std::endl;
        out << min_fuel_2 << std::endl;
    }

    static uint64_t calculate_fuel_cost_1(const std::vector<uint16_t>& numbers, uint16_t median) {
        uint64_t result{0};
        for (auto& x: numbers) {
            result += abs(x - median);
        }
        return result;
    }

    static uint64_t calculate_fuel_cost_2(const std::vector<uint16_t>& numbers, uint16_t median) {
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
