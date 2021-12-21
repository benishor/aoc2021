#include "solution.h"

using image = std::vector<std::vector<char>>;

size_t count_lit_pixels(const image& image) {
    size_t width = image[0].size();
    size_t height = image.size();

    size_t result{0};
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (image[y][x] == '#') result++;
        }
    }
    return result;
}

image enhance(const image& input, const std::string& image_enhancing_algo, char infinite_space_char) {
    const int new_width = input[0].size() + 4;
    const int new_height = input.size() + 4;
    const int old_width = input[0].size();
    const int old_height = input.size();

    std::vector<std::vector<char>> result{};

    for (int y = 0; y < new_height; y++) {
        result.push_back({});
        for (int x = 0; x < new_width; x++) {
            int x_in_input = x - 2;
            int y_in_input = y - 2;

            uint16_t index{0};
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int xold = x_in_input + dx;
                    int yold = y_in_input + dy;
                    if (xold >= 0 && xold < old_width &&
                        yold >= 0 && yold < old_height) {
                        if (input[yold][xold] == '#') {
                            uint16_t bit_index = (2 - (dx + 1)) + (2 - (dy + 1)) * 3;
                            index += 1 << bit_index;
                        }
                    } else {
                        if (infinite_space_char == '#') {
                            uint16_t bit_index = (2 - (dx + 1)) + (2 - (dy + 1)) * 3;
                            index += 1 << bit_index;
                        }
                    }
                }
            }
            if (index > 511) {
                std::cerr << "!!! Something fucked up, invalid index!" << index << std::endl;
                exit(1);
            }

            auto c = image_enhancing_algo[index];
            result.back().push_back(c);
        }
    }
    return result;
}

class day20 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {
        std::string image_enhancing_algo;
        std::getline(in, image_enhancing_algo);

        std::vector<std::vector<char>> image;
        for (std::string line; std::getline(in, line);) {
            if (line.empty()) continue;
            std::vector<char> v;
            std::copy(line.begin(), line.end(), std::back_inserter(v));
            image.push_back(v);
        }

        for (int i = 0; i < 50; i++) {
            if (i == 2) {
                out << count_lit_pixels(image) << std::endl;
            }
            image = enhance(image, image_enhancing_algo, (i & 1) == 1 ? '#' : '.');
        }
        out << count_lit_pixels(image) << std::endl;
    }
};

void dump_image(const image& image) {
    size_t width = image[0].size();
    size_t height = image.size();

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            std::cout << image[y][x];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main(int argc, char **argv) {
    day20 solution{};
    return solution.execute(argc, argv);
}
