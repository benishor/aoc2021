#include <numeric>
#include "solution.h"

using group_frequency = std::map<std::string, uint64_t>;
using letter_frequency = std::unordered_map<char, uint64_t>;
using rule_map = std::unordered_map<std::string, char>;

struct data {
    std::string polymer_template;
    rule_map pair_insertion_rules;
};

class day14 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {
        data input = parse_data(in);

        group_frequency frequency = to_group_frequency(input.polymer_template);
        for (int i = 0; i < 40; i++) {
            frequency = apply_insertion_rules(frequency, input.pair_insertion_rules);
            if (i == 9) {
                auto part1 = calculate_score(frequency, input.polymer_template.front(), input.polymer_template.back());
                out << part1 << std::endl;
            }
        }

        auto part2 = calculate_score(frequency, input.polymer_template.front(), input.polymer_template.back());
        out << part2 << std::endl;
    }

    static inline group_frequency to_group_frequency(const std::string& input) {
        group_frequency groups;
        for (size_t i = 0; i < input.size() - 1; i++) {
            groups[input.substr(i, 2)]++;
        }
        return groups;
    }

    static inline group_frequency apply_insertion_rules(const group_frequency& groups, const rule_map& rules) {
        group_frequency result;
        for (auto& kv: groups) {
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

    static inline uint64_t calculate_score(const group_frequency& groups, char start_char, char end_char) {
        auto letters = get_letter_frequency(groups, start_char, end_char);

        uint64_t min_freq{std::numeric_limits<uint64_t>::max()};
        uint64_t max_freq{0};
        for (auto& kv: letters) {
            min_freq = std::min(min_freq, kv.second);
            max_freq = std::max(max_freq, kv.second);
        }
        return max_freq - min_freq;
    }

    static inline letter_frequency get_letter_frequency(const group_frequency& frequency, char start_char, char end_char) {
        letter_frequency letters;
        for (auto& kv: frequency) {
            letters[kv.first.front()] += kv.second;
            letters[kv.first.back()] += kv.second;
        }

        // each letter is counted twice, except the start and end of the polymer template
        for (auto& kv: letters) {
            kv.second /= 2;
        }
        letters[start_char]++;
        letters[end_char]++;
        return letters;
    }

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
};

int main(int argc, char **argv) {
    day14 solution{};
    return solution.execute(argc, argv);
}
