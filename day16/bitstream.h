#pragma once

#include <string>
#include <vector>

static inline unsigned char hex_nibble_to_dec(char hex_char) {
    return (hex_char >= '0' && hex_char <= '9')
           ? hex_char - '0'
           : hex_char - 'A' + 10;
}

class bitstream {
public:
    explicit bitstream(const std::string& hex_string) {
        bytes.reserve(hex_string.size() / 2);

        for (size_t i = 0; i < hex_string.size(); i += 2) {
            bytes.push_back(hex_nibble_to_dec(hex_string[i]) << 4 |
                            hex_nibble_to_dec(hex_string[i + 1]));
        }
    }

    [[nodiscard]] bool available() const {
        return cursor < bytes.size() * 8;
    }

    [[nodiscard]] bool has_available(size_t how_many_bits) const {
        return (bytes.size() * 8 - cursor) >= how_many_bits;
    }

    void reset() {
        cursor = 0;
    }

    inline uint8_t read_bit() {
        size_t byte_index = cursor / 8;
        size_t bit_index = cursor & 7;
        cursor++;
        return (bytes[byte_index] & (1 << (7 - bit_index))) > 0 ? 1 : 0;
    }

    inline uint64_t read_bits(size_t how_many) {
        uint64_t result{0};
        for (size_t i = 0; i < how_many; i++) {
            result |= read_bit() << (how_many - i - 1);
        }
        return result;
    }

    [[nodiscard]] size_t get_offset() const {
        return cursor;
    }

private:

    std::vector<unsigned char> bytes;
    size_t cursor{0};
};
