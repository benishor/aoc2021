#include "solution.h"

const int BitsPerLine = 12;
using bitset = std::bitset<BitsPerLine>;
using bitset_vector = std::vector<bitset>;

enum class bit_criteria {
    oxygen_generator,
    co2_scrubber
};

class day3 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {
        auto lines = parse_data(in);
        const size_t half_records = lines.size() >> 1;

        // part 1
        std::array<size_t, BitsPerLine> bit_counts{0};
        for (auto& l: lines) {
            for (size_t i = 0; i < BitsPerLine; i++) {
                bit_counts[i] += l.test(i);
            }
        }

        std::bitset<BitsPerLine> gamma_rate{0};
        for (size_t i = 0; i < BitsPerLine; i++) {
            gamma_rate.set(i, bit_counts[i] > half_records);
        }
        out << (gamma_rate.to_ulong() * gamma_rate.flip().to_ulong()) << std::endl;

        // part 2
        auto oxygen_generator_rating = find_rating(lines, bit_criteria::oxygen_generator);
        auto co2_scrubber_rating = find_rating(lines, bit_criteria::co2_scrubber);
        out << oxygen_generator_rating * co2_scrubber_rating << std::endl;
    }

    static bitset_vector parse_data(std::istream& in) {
        bitset_vector lines;
        for (std::string line; std::getline(in, line);) {
            bitset bits{0};
            for (size_t i = 0; i < line.size(); i++) {
                if (line[i] == '1') {
                    bits.set(BitsPerLine - 1 - i, true);
                }
            }
            lines.emplace_back(bits);
        }
        return lines;
    }

    static unsigned long find_rating(const bitset_vector& lines, bit_criteria type) {
        auto candidates = lines;
        for (int i = BitsPerLine - 1; i >= 0; i--) {
            size_t how_many_ones{0};
            for (auto& l: candidates) {
                if (l.test(i)) how_many_ones++;
            }
            size_t how_many_zeroes = candidates.size() - how_many_ones;

            bitset_vector new_candidates;
            for (auto& l: candidates) {
                switch (type) {
                    case bit_criteria::oxygen_generator:
                        if ((l.test(i) && how_many_ones >= how_many_zeroes) ||
                            (!l.test(i) && how_many_zeroes > how_many_ones)) {
                            new_candidates.push_back(l);
                        }
                        break;
                    case bit_criteria::co2_scrubber:
                        if ((l.test(i) && how_many_ones < how_many_zeroes) ||
                            (!l.test(i) && how_many_zeroes <= how_many_ones)) {
                            new_candidates.push_back(l);
                        }
                        break;
                }
            }
            candidates = new_candidates;
            if (candidates.size() == 1) {
                return candidates.front().to_ulong();
            }
        }
        return 0;
    }
};

int main(int argc, char **argv) {
    day3 solution{};
    return solution.execute(argc, argv);
}
