#include "solution.h"

static inline unsigned char hex_nibble_to_dec(char hex_char) {
    if (hex_char >= '0' && hex_char <= '9') {
        return hex_char - '0';
    } else {
        return hex_char - 'A' + 10;
    }
}


struct packet {
    uint8_t version;
    uint8_t type;
    uint8_t length_type_id;
    uint64_t literal;
    std::vector<packet> sub_packets;

    [[nodiscard]] uint64_t version_sum() const {
        uint64_t result{version};
        for (auto& s: sub_packets) {
            result += s.version_sum();
        }
        return result;
    }

    [[nodiscard]] uint64_t evaluate() const {
        switch (type) {
            case 0: {
                uint64_t result{0};
                for (auto& s: sub_packets) {
                    result += s.evaluate();
                }
                return result;
            }
            case 1: {
                uint64_t result{1};
                for (auto& s: sub_packets) {
                    result *= s.evaluate();
                }
                return result;
            }
            case 2: {
                uint64_t result{std::numeric_limits<uint64_t>::max()};
                for (auto& s: sub_packets) {
                    result = std::min(result, s.evaluate());
                }
                return result;
            }
            case 3: {
                uint64_t result{std::numeric_limits<uint64_t>::min()};
                for (auto& s: sub_packets) {
                    result = std::max(result, s.evaluate());
                }
                return result;
            }
            case 4: {
                return literal;
            }
            case 5: {
                return sub_packets[0].evaluate() > sub_packets[1].evaluate() ? 1 : 0;
            }
            case 6: {
                return sub_packets[0].evaluate() < sub_packets[1].evaluate() ? 1 : 0;
            }
            case 7: {
                return sub_packets[0].evaluate() == sub_packets[1].evaluate() ? 1 : 0;
            }
            default:
                std::cerr << "Unknown packet type " << type << std::endl;
                return 0;
        }
    }
};


struct bit_provider {

    explicit bit_provider(const std::string& hex_string) {
        for (size_t i = 0; i < hex_string.size(); i += 2) {
            bytes.push_back(hex_nibble_to_dec(hex_string[i]) << 4 | hex_nibble_to_dec(hex_string[i + 1]));
        }
    }

    bool available() const {
        return cursor < bytes.size() * 8;
    }

    [[nodiscard]] bool available(size_t how_many_bits) const {
        return (bytes.size() * 8 - cursor) >= how_many_bits;
    }

    void reset() {
        cursor = 0;
    }

    unsigned char next_bits(int how_many) {
        uint8_t result{0};
        for (int i = 0; i < how_many; i++) {
            result |= next_bit() << (how_many - i - 1);
        }
        return result;
    }

    [[nodiscard]] size_t get_offset() const {
        return cursor;
    }

    unsigned char next_bit() {
        size_t byte_index = cursor / 8;
        size_t bit_index = cursor & 7;
        cursor++;
        return (bytes[byte_index] & (1 << (7 - bit_index))) > 0 ? 1 : 0;
    }

    std::vector<unsigned char> bytes;
    size_t cursor{0};
};


packet parse_packet_from_bits(bit_provider& bits) {
    packet result{};
    result.version = bits.next_bits(3);
    result.type = bits.next_bits(3);
    if (result.type == 4) {
        std::vector<unsigned char> nibbles;
        while (bits.available(5)) {
            unsigned char value = bits.next_bits(5);
            nibbles.push_back(value & 0b1111);
            if ((value & 0b10000) == 0) {
                break;
            }
        }
        result.literal = 0;
        for (auto& n: nibbles) {
            result.literal <<= 4;
            result.literal |= n;
        }
    } else {
        result.length_type_id = bits.next_bit();
        switch (result.length_type_id) {
            case 0: {
                // next 15 bits are a number that represents the total length in bits of the sub-packets contained by this packet
                uint16_t sub_packets_length{0};
                for (size_t i = 0; i < 15; i++) {
                    sub_packets_length |= bits.next_bit() << (14 - i);
                }

                uint64_t start_offset = bits.get_offset();
                while ((bits.get_offset() - start_offset) < sub_packets_length) {
                    result.sub_packets.push_back(parse_packet_from_bits(bits));
                }
                break;
            }
            case 1: {
                // next 11 bits are a number that represents the number of sub-packets immediately contained by this packet.
                uint16_t sub_packets_count{0};
                for (size_t i = 0; i < 11; i++) {
                    sub_packets_count |= bits.next_bit() << (10 - i);
                }

                for (size_t i = 0; i < sub_packets_count; i++) {
                    result.sub_packets.push_back(parse_packet_from_bits(bits));
                }
                break;
            }
            default:
                break;
        }
    }

    return result;
}

class day16 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {
        std::string line;
        std::getline(in, line);

        bit_provider bits{line};
        auto pkt = parse_packet_from_bits(bits);
        out << pkt.version_sum() << std::endl;
        out << pkt.evaluate() << std::endl;
    }
};

int main(int argc, char **argv) {
    day16 solution{};
    return solution.execute(argc, argv);
}
