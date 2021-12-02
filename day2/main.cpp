#include "solution.h"

class day2 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {

        int horiz{0}, depth{0}, aim{0};

        for (std::string line; std::getline(in, line);) {
            switch (line[0]) {
                case 'f': {
                    int x = std::stoi(line.substr(8));
                    horiz += x;
                    depth += aim * x;
                    break;
                }
                case 'u':
                    aim -= std::stoi(line.substr(3));
                    break;
                case 'd':
                    aim += std::stoi(line.substr(5));
                    break;
                default:
                    std::cerr << "Illegal input in line [" << line << "]";
                    break;
            }
        }
        std::cout << (horiz * aim) << std::endl;
        std::cout << (horiz * depth) << std::endl;
    }
};

int main(int argc, char **argv) {
    day2 solution{};
    return solution.execute(argc, argv);
}
