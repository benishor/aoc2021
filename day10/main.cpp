#include "solution.h"

class day10 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {

        uint64_t part1{0};
        std::vector<uint64_t> part2_scores;
        for (std::string line; std::getline(in, line);) {
            std::stack<char> state;
            bool is_line_corrupted = false;
            for (auto& c: line) {
                if (is_line_corrupted) break;
                switch (c) {
                    case '(':
                    case '[':
                    case '{':
                    case '<':
                        state.push(c);
                        break;
                    case ')':
                    case ']':
                    case '}':
                    case '>':
                        if (state.empty() || !is_valid_pair(state.top(), c)) {
                            part1 += score_for_illegal_closing_char(c);
                            is_line_corrupted = true;
                        } else {
                            state.pop();
                        }
                        break;
                    default:
                        std::cerr << "Unexpected line character: " << c << std::endl;
                        break;
                }
            }

            bool is_line_incomplete = !is_line_corrupted && !state.empty();
            if (is_line_incomplete) {
                uint64_t score{0};
                while (!state.empty()) {
                    score *= 5;
                    score += part2_score_for_missing_close(state.top());
                    state.pop();
                }
                part2_scores.push_back(score);
            }
        }

        out << part1 << std::endl;
        std::sort(std::begin(part2_scores), std::end(part2_scores));
        out << part2_scores[part2_scores.size() / 2] << std::endl;
    }

    static uint64_t part2_score_for_missing_close(char open) {
        switch (open) {
            case '(':
                return 1;
            case '[':
                return 2;
            case '{':
                return 3;
            case '<':
                return 4;
            default:
                return 0;
        }
    }

    static bool is_valid_pair(char open, char close) {
        return (close - open) <= 2;
    }

    static uint64_t score_for_illegal_closing_char(char close) {
        switch (close) {
            case ')':
                return 3;
            case ']':
                return 57;
            case '}':
                return 1197;
            case '>':
                return 25137;
            default:
                return 0;
        }
    }
};

int main(int argc, char **argv) {
    day10 solution{};
    return solution.execute(argc, argv);
}
