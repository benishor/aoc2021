#include <numeric>
#include "solution.h"

using group_frequency = std::map<std::string, uint64_t>;
using rule_map = std::unordered_map<std::string, char>;

struct data {
    std::string polymer_template;
    rule_map pair_insertion_rules;
};

class day14 : public aoc::solution {
protected:

    static inline data parse_data(std::istream& in) {
        data result;
        std::getline(in, result.polymer_template);

        for (std::string line; std::getline(in, line);) {
            if (!line.empty()) {
                auto pieces = aoc::split_string(line, " -> ");
                result.pair_insertion_rules[pieces[0]] = pieces[1][0];
            }
        }
        return result;
    }

    static inline group_frequency transform(const group_frequency& frequency, const rule_map& rules) {
        group_frequency result;
        for (auto& kv: frequency) {
            if (rules.count(kv.first)) {
                std::string new_key;
                new_key += kv.first.front();
                new_key += rules.at(kv.first);
                result[new_key] += kv.second;

                new_key = rules.at(kv.first);
                new_key += kv.first.back();
                result[new_key] += kv.second;
            } else {
                result[kv.first] += kv.second;
            }
        }
        return result;
    }

    static inline group_frequency to_group_frequency(const std::string& input) {
        group_frequency frequency;
        for (size_t i = 0; i < input.size() - 1; i++) {
            frequency[input.substr(i, 2)]++;
        }
        return frequency;
    }

    static inline std::unordered_map<char, uint64_t>
    get_letter_frequency(const group_frequency& frequency, char start, char end) {
        std::unordered_map<char, uint64_t> letter_frequency;
        for (auto& kv: frequency) {
            letter_frequency[kv.first.front()] += kv.second;
            letter_frequency[kv.first.back()] += kv.second;
        }

        // each letter is counted twice, except the start and end of the polymer template
        for (auto& kv: letter_frequency) {
            kv.second /= 2;
        }
        letter_frequency[start]++;
        letter_frequency[end]++;
        return letter_frequency;
    }

    static inline uint64_t calculate_score(const group_frequency& frequency, char start, char end) {
        auto letter_frequency = get_letter_frequency(frequency, start, end);

        uint64_t min_freq{std::numeric_limits<uint64_t>::max()};
        uint64_t max_freq{0};
        for (auto& kv: letter_frequency) {
            min_freq = std::min(min_freq, kv.second);
            max_freq = std::max(max_freq, kv.second);
        }
        return max_freq - min_freq;
    }

    void run(std::istream& in, std::ostream& out) override {
        data input = parse_data(in);

        group_frequency frequency = to_group_frequency(input.polymer_template);
        for (int i = 0; i < 40; i++) {
            frequency = transform(frequency, input.pair_insertion_rules);
            if (i == 9) {
                auto part1 = calculate_score(frequency, input.polymer_template.front(), input.polymer_template.back());
                out << part1 << std::endl;
            }
        }

        auto part2 = calculate_score(frequency, input.polymer_template.front(), input.polymer_template.back());
        out << part2 << std::endl;
    }
};

int main(int argc, char **argv) {
    day14 solution{};
    return solution.execute(argc, argv);
}
