#include "solution.h"

// 0b0gfedcba
uint8_t digitToPattern[10] = {
        0b01110111,// 0 => abcefg
        0b00100100,// 1 => cf
        0b01011101,// 2 => acdeg
        0b01101101,// 3 => acdfg
        0b00101110,// 4 => bcdf
        0b01101011,// 5 => abdfg
        0b01111011,// 6 => abdefg
        0b00100101,// 7 => acf
        0b01111111,// 8 => abcdefg
        0b01101111,// 9 => abcdfg
};

std::map<uint8_t, uint8_t> pattern_to_digit = {
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


uint8_t decode_digit(const std::string& pattern, std::unordered_map<char, char>& inverse_translation_map) {
    uint8_t numeric_pattern{0};
    for (auto& c: pattern) {
        numeric_pattern |= (uint8_t) pow(2, (inverse_translation_map[c] - 'a'));
    }
    return pattern_to_digit[numeric_pattern];
}

class day8 : public aoc::solution {
protected:


    std::set<uint8_t> difference(const std::set<uint8_t>& a, const std::set<uint8_t>& b) {
        std::set<uint8_t> c;
        if (a.empty() || b.empty()) return c;
        for (auto& x: a) {
            if (!b.count(x)) {
                c.insert(x);
            }
        }
        return c;
    }

    std::string difference(const std::string& s, char c) {
        std::string result{};
        for (auto& x: s) {
            if (x != c) {
                result += x;
            }
        }
        return result;
    }

    std::string difference(const std::string& s, const std::string& c) {
        std::string result{};
        for (auto& x: s) {
            if (c.find(x) == std::string::npos) {
                result += x;
            }
        }
        return result;
    }


    void run(std::istream& in, std::ostream& out) override {

        uint32_t part1{0};
        uint64_t part2{0};

        for (std::string line; std::getline(in, line);) {
            auto pieces = aoc::split_string(line, "|");
            auto l2 = aoc::trim(pieces[1]);

            std::set<uint8_t> segment_candidates[7];
            for (size_t i = 0; i < 7; i++) {
                segment_candidates[i] = {0, 1, 2, 3, 4, 5, 6};
            }

            auto patterns = aoc::split_string(aoc::trim(pieces[0]), " ");
            std::sort(std::begin(patterns), std::end(patterns), [](const std::string& a, const std::string& b) -> bool {
                return a.size() < b.size();
            });

            std::map<char, uint8_t> segment_frequencies;
            for (auto& p: patterns) {
                for (auto& c: p) {
                    segment_frequencies[c]++;
                }
            }
            std::unordered_map<uint8_t, std::set<char>> frequency_to_segments;
            for (auto& kv: segment_frequencies) {
                frequency_to_segments[kv.second].insert(kv.first);
            }

            std::map<char, char> translation_map;
            translation_map['e'] = *frequency_to_segments[4].begin();
            translation_map['b'] = *frequency_to_segments[6].begin();
            translation_map['f'] = *frequency_to_segments[9].begin();
            translation_map['c'] = difference(patterns[0], translation_map['f']).front();
            translation_map['d'] = difference(difference(patterns[2], patterns[0]), translation_map['b']).front();
            translation_map['a'] = difference(patterns[1], patterns[0]).front();
            translation_map['b'] = difference(difference(patterns[2], patterns[0]), translation_map['d']).front();
            translation_map['g'] = difference(difference(difference(*patterns.rbegin(), patterns[1]), patterns[2]),
                                              translation_map['e']).front();

            std::unordered_map<char, char> inverse_translation_map;
            for (auto& kv: translation_map) {
                inverse_translation_map[kv.second] = kv.first;
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
};

int main(int argc, char **argv) {
    day8 solution{};
    return solution.execute(argc, argv);
}
