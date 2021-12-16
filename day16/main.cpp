#include "solution.h"
#include "packet.h"

class day16 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {
        std::string line;
        std::getline(in, line);

        bitstream bits{line};
        auto pkt = parse_packet_from_bits(bits);
        out << pkt.version_sum() << std::endl;
        out << pkt.evaluate() << std::endl;
    }
};

int main(int argc, char **argv) {
    day16 solution{};
    return solution.execute(argc, argv);
}
