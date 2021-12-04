#include "solution.h"

using board = std::array<std::array<int, 5>, 5>;
struct state {
    std::vector<int> numbers;
    std::vector<board> boards;
};

static void check_number_on_board(board& b, int number) {
    for (size_t row = 0; row < 5; row++) {
        for (size_t col = 0; col < 5; col++) {
            if (b[row][col] == number) {
                b[row][col] = -1;
                return;
            }
        }
    }
}

static bool does_board_have_bingo(const board& b) {
    // test rows first
    for (size_t row = 0; row < 5; row++) {
        bool all_checked{true};
        for (size_t i = 0; i < 5; i++) {
            if (b[row][i] != -1) {
                all_checked = false;
                break;
            }
        }
        if (all_checked) return true;
    }

    // test columns
    for (size_t col = 0; col < 5; col++) {
        bool all_checked{true};
        for (size_t i = 0; i < 5; i++) {
            if (b[i][col] != -1) {
                all_checked = false;
                break;
            }
        }
        if (all_checked) return true;
    }
    return false;
}

static int board_unmarked_sum(const board& b) {
    int sum{0};
    for (size_t row = 0; row < 5; row++) {
        for (size_t col = 0; col < 5; col++) {
            if (b[row][col] != -1) {
                sum += b[row][col];
            }
        }
    }
    return sum;
}

class day4 : public aoc::solution {
protected:

    static state parse_data(std::istream& in) {
        state data;
        bool numbers_parsed{false};
        board current_board;
        size_t current_row{0};
        for (std::string line; std::getline(in, line);) {
            line = aoc::trim(line);
            if (line.empty()) continue;

            std::stringstream ss{line};
            if (numbers_parsed) {
                auto values = aoc::elements_from_stream<int>(ss);
                for (size_t i = 0; i < 5; i++) {
                    current_board[current_row][i] = values[i];
                }
                current_row++;
                if (current_row == 5) {
                    current_row = 0;
                    data.boards.push_back(current_board);
                }
            } else {
                data.numbers = aoc::csv_elements_from_stream<int>(ss);
                numbers_parsed = true;
                continue;
            }
        }
        return data;
    }

    void run(std::istream& in, std::ostream& out) override {
        auto data = parse_data(in);

        std::map<size_t, size_t> boardIndexToWinningNumberMap;
        for (size_t board_index = 0; board_index < data.boards.size(); board_index++) {
            for (size_t extracted_number_index = 0;
                 extracted_number_index < data.numbers.size(); extracted_number_index++) {
                check_number_on_board(data.boards[board_index], data.numbers[extracted_number_index]);
                if (does_board_have_bingo(data.boards[board_index])) {
                    boardIndexToWinningNumberMap[board_index] = extracted_number_index;
                    break;
                }
            }
        }

        auto min_idx = min_element(boardIndexToWinningNumberMap.begin(), boardIndexToWinningNumberMap.end(),
                                   [](decltype(boardIndexToWinningNumberMap)::value_type& l,
                                      decltype(boardIndexToWinningNumberMap)::value_type& r) -> bool {
                                       return l.second < r.second;
                                   });
        auto max_idx = min_element(boardIndexToWinningNumberMap.begin(), boardIndexToWinningNumberMap.end(),
                                   [](decltype(boardIndexToWinningNumberMap)::value_type& l,
                                      decltype(boardIndexToWinningNumberMap)::value_type& r) -> bool {
                                       return l.second >= r.second;
                                   });

        out << board_unmarked_sum(data.boards[min_idx->first]) * data.numbers[min_idx->second] << std::endl;
        out << board_unmarked_sum(data.boards[max_idx->first]) * data.numbers[max_idx->second] << std::endl;
    }
};

int main(int argc, char **argv) {
    day4 solution{};
    return solution.execute(argc, argv);
}