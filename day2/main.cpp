#include "solution.h"

class day2 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {

        int horiz{0}, depth1{0}, depth2{0}, aim{0};
        for (std::string line; std::getline(in, line);) {
            if (line.rfind("forward", 0) == 0) {
                int amount = std::stoi(line.substr(8));
                horiz += amount;
                depth2 += aim * amount;
            } else if (line.rfind("up", 0) == 0) {
                int amount = std::stoi(line.substr(3));
                depth1 -= amount;
                aim -= amount;
            } else {
                // down
                int amount = std::stoi(line.substr(5));
                depth1 += amount;
                aim += amount;
            }
        }
        std::cout << (horiz * depth1) << std::endl;
        std::cout << (horiz * depth2) << std::endl;
    }
};

int main(int argc, char **argv) {
    day2 solution{};
    return solution.execute(argc, argv);
}
