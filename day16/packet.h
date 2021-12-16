#pragma once

#include "bitstream.h"
#include <numeric>

enum class packet_type {
    sum,
    product,
    min,
    max,
    literal,
    greater_than,
    lower_than,
    equal,
    none
};

packet_type to_packet_type(uint8_t value) {
    static std::array<packet_type, 8> val_to_enum = {
            packet_type::sum,
            packet_type::product,
            packet_type::min,
            packet_type::max,
            packet_type::literal,
            packet_type::greater_than,
            packet_type::lower_than,
            packet_type::equal
    };
    return val_to_enum[value];
}


struct packet {
    uint8_t version;
    packet_type type{packet_type::none};
    uint8_t length_type_id;
    uint64_t literal;
    std::vector<packet> sub_packets;

    [[nodiscard]] uint64_t version_sum() const {
        return std::accumulate(std::begin(sub_packets), std::end(sub_packets), version,
                               [](uint64_t value, const packet& p) -> uint64_t {
                                   return value + p.version_sum();
                               });
    }

    [[nodiscard]] uint64_t evaluate() const {
        switch (type) {
            case packet_type::sum: {
                return std::accumulate(std::begin(sub_packets), std::end(sub_packets), 0ULL,
                                       [](uint64_t value, const packet& p) -> uint64_t {
                                           return value + p.evaluate();
                                       });
            }
            case packet_type::product: {
                return std::accumulate(std::begin(sub_packets), std::end(sub_packets), 1ULL,
                                       [](uint64_t value, const packet& p) -> uint64_t {
                                           return value * p.evaluate();
                                       });
            }
            case packet_type::min: {
                uint64_t result{std::numeric_limits<uint64_t>::max()};
                for (auto& s: sub_packets) {
                    result = std::min(result, s.evaluate());
                }
                return result;
            }
            case packet_type::max: {
                uint64_t result{std::numeric_limits<uint64_t>::min()};
                for (auto& s: sub_packets) {
                    result = std::max(result, s.evaluate());
                }
                return result;
            }
            case packet_type::literal: {
                return literal;
            }
            case packet_type::greater_than: {
                return sub_packets[0].evaluate() > sub_packets[1].evaluate() ? 1 : 0;
            }
            case packet_type::lower_than: {
                return sub_packets[0].evaluate() < sub_packets[1].evaluate() ? 1 : 0;
            }
            case packet_type::equal: {
                return sub_packets[0].evaluate() == sub_packets[1].evaluate() ? 1 : 0;
            }
            default:
                std::cerr << "Unknown packet type " << std::endl;
                return 0;
        }
    }
};

static inline uint64_t read_literal(bitstream& bits) {
    uint64_t literal{0};
    while (bits.has_available(5)) {
        unsigned char value = bits.read_bits(5);
        literal <<= 4;
        literal |= value & 0b1111;
        if ((value & 0b10000) == 0) {
            break;
        }
    }
    return literal;
}

packet parse_packet_from_bits(bitstream& bits) {
    packet result{};

    result.version = bits.read_bits(3);
    result.type = to_packet_type(bits.read_bits(3));

    if (result.type == packet_type::literal) {
        result.literal = read_literal(bits);
        return result;

    }

    result.length_type_id = bits.read_bit();
    if (result.length_type_id == 0) {
        auto sub_packets_length = bits.read_bits(15);
        uint64_t start_offset = bits.get_offset();
        while ((bits.get_offset() - start_offset) < sub_packets_length) {
            result.sub_packets.push_back(parse_packet_from_bits(bits));
        }
    } else {
        auto sub_packets_count = bits.read_bits(11);
        for (size_t i = 0; i < sub_packets_count; i++) {
            result.sub_packets.push_back(parse_packet_from_bits(bits));
        }
    }

    return result;
}
