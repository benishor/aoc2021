#include "solution.h"

static std::map<uint8_t, uint8_t> pattern_to_digit = {
        {0b01110111, 0},
        {0b00100100, 1},
        {0b01011101, 2},
        {0b01101101, 3},
        {0b00101110, 4},
        {0b01101011, 5},
        {0b01111011, 6},
        {0b00100101, 7},
        {0b01111111, 8},
        {0b01101111, 9}
};

class day8 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {

        uint32_t part1{0};
        uint64_t part2{0};

        for (std::string line; std::getline(in, line);) {
            auto pieces = aoc::split_string(line, "|");
            auto l2 = aoc::trim(pieces[1]);

            auto patterns = aoc::split_string(aoc::trim(pieces[0]), " ");
            std::sort(std::begin(patterns), std::end(patterns), [](const std::string& a, const std::string& b) -> bool {
                return a.size() < b.size();
            });

            uint8_t binary_patterns[10];
            uint8_t binary_segment_frequencies[7] = {0, 0, 0, 0, 0, 0, 0};
            for (size_t i = 0; i < 10; i++) {
                auto binary_pattern = pattern_to_binary(patterns[i]);
                binary_patterns[i] = binary_pattern;

                uint8_t mask = 1;
                for (size_t j = 0; j < 7; j++, mask <<= 1) {
                    if ((binary_pattern & mask) == mask) {
                        binary_segment_frequencies[j]++;
                    }
                }
            }

            uint8_t binary_frequency_to_segments[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            for (size_t i = 0; i < 7; i++) {
                binary_frequency_to_segments[binary_segment_frequencies[i]] |= 1 << i;
            }

            uint8_t binary_translation_map[7] = {0, 0, 0, 0, 0, 0, 0};
            binary_translation_map[4] = binary_frequency_to_segments[4]; // e
            binary_translation_map[1] = binary_frequency_to_segments[6]; // b
            binary_translation_map[5] = binary_frequency_to_segments[9]; // f
            binary_translation_map[2] = binary_patterns[0] & ~binary_translation_map[5]; // c
            binary_translation_map[3] = (binary_patterns[2] & ~binary_patterns[0]) & ~binary_translation_map[1]; // d
            binary_translation_map[0] = binary_patterns[1] & ~binary_patterns[0]; // a
            binary_translation_map[6] = 0b01111111 & ~(binary_translation_map[0] | binary_translation_map[1] |
                                                       binary_translation_map[2] | binary_translation_map[3] |
                                                       binary_translation_map[4] | binary_translation_map[5]); // g

            uint8_t inverse_translation_map[7];
            for (size_t i = 0; i < 7; i++) {
                inverse_translation_map[(uint8_t) log2(binary_translation_map[i])] = i;
            }

            auto digits = aoc::split_string(l2, " ");

            uint16_t number{0};
            uint16_t factor{1000};
            for (size_t i = 0; i < 4; i++) {
                auto digit = decode_digit(digits[i], inverse_translation_map);
                if (digit == 1 || digit == 4 || digit == 7 || digit == 8) {
                    part1++;
                }
                number += digit * factor;
                factor /= 10;
            }
            part2 += number;
        }
        out << part1 << std::endl;
        out << part2 << std::endl;
    }

    static uint8_t pattern_to_binary(const std::string& pattern) {
        uint8_t result{0};
        for (auto& c: pattern) {
            result |= 1 << (uint8_t) (c - 'a');
        }
        return result;
    }

    static uint8_t decode_digit(const std::string& pattern, const uint8_t *inverse_translation_map) {
        uint8_t numeric_pattern{0};
        for (auto& c: pattern) {
            numeric_pattern |= 1 << inverse_translation_map[c - 'a'];
        }
        return pattern_to_digit[numeric_pattern];
    }
};

int main(int argc, char **argv) {
    day8 solution{};
    return solution.execute(argc, argv);
}
