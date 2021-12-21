#include "solution.h"

struct position {
    int64_t x, y, z;

    void min() {
        x = y = z = std::numeric_limits<int64_t>::min();
    }

    void max() {
        x = y = z = std::numeric_limits<int64_t>::max();
    }
};

bool operator<(const position& a, const position& b) {
    if (a.x < b.x) return true;
    if (a.x == b.x && a.y < b.y) return true;
    if (a.x == b.x && a.y == b.y && a.z < b.z) return true;
    return false;
}

bool operator==(const position& a, const position& b) {
    if (a.z != b.z) return false;
    if (a.y != b.y) return false;
    if (a.x != b.x) return false;
    return true;
}


position operator-(const position& a, const position& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

std::ostream& operator<<(std::ostream& out, const position& p) {
    out << p.x << "," << p.y << "," << p.z;
    return out;
}


position transform(position p, uint8_t coordinate_system) {
    // facing +Z. and rolls
    if (coordinate_system == 0) {
        return p;
    } else if (coordinate_system == 1) {
        return position{p.y, -p.x, p.z};
    } else if (coordinate_system == 2) {
        return position{-p.x, -p.y, p.z};
    } else if (coordinate_system == 3) {
        return position{-p.y, p.x, p.z};
    }

    // facing -Z. and rolls
    if (coordinate_system == 4) {
        return position{-p.x, p.y, -p.z};
    } else if (coordinate_system == 5) {
        return position{-p.y, -p.x, -p.z};
    } else if (coordinate_system == 6) {
        return position{p.x, -p.y, -p.z};
    } else if (coordinate_system == 7) {
        return position{p.y, p.x, -p.z};
    }

    // facing +X. and rolls
    if (coordinate_system == 8) {
        return position{p.z, p.y, -p.x};
    } else if (coordinate_system == 9) {
        return position{p.z, -p.x, -p.y};
    } else if (coordinate_system == 10) {
        return position{p.z, -p.y, p.x};
    } else if (coordinate_system == 11) {
        return position{p.z, p.x, p.y};
    }

    // facing -X. and rolls
    if (coordinate_system == 12) {
        return position{-p.z, p.y, p.x};
    } else if (coordinate_system == 13) {
        return position{-p.z, -p.x, p.y};
    } else if (coordinate_system == 14) {
        return position{-p.z, -p.y, -p.x};
    } else if (coordinate_system == 15) {
        return position{-p.z, p.x, -p.y};
    }

    // facing +Y. and rolls
    if (coordinate_system == 16) {
        return position{p.x, p.z, -p.y};
    } else if (coordinate_system == 17) {
        return position{p.y, p.z, p.x};
    } else if (coordinate_system == 18) {
        return position{-p.x, p.z, p.y};
    } else if (coordinate_system == 19) {
        return position{-p.y, p.z, -p.x};
    }

    // facing -Y. and rolls
    if (coordinate_system == 20) {
        return position{p.x, -p.z, p.y};
    } else if (coordinate_system == 21) {
        return position{p.y, -p.z, -p.x};
    } else if (coordinate_system == 22) {
        return position{-p.x, -p.z, -p.y};
    } else if (coordinate_system == 23) {
        return position{-p.y, -p.z, p.x};
    }

    // shouldn't get here
    return p;
}


std::pair<position, position> find_bounds(std::vector<position>& vector) {
    position min, max;
    min.max();
    max.min();

    for (auto& p: vector) {
        if (p.x < min.x) min.x = p.x;
        if (p.y < min.y) min.y = p.y;
        if (p.z < min.z) min.z = p.z;

        if (p.x > max.x) max.x = p.x;
        if (p.y > max.y) max.y = p.y;
        if (p.z > max.z) max.z = p.z;
    }

    return {min, max};
}

std::vector<position> combine(const std::vector<position>& a, const std::vector<position>& b, position translation) {
    std::set<position> unique;
    for (auto& p: a) {
        unique.insert(p);
    }
    for (auto& p: b) {
        unique.insert(p - translation);
    }

    std::vector<position> v;
    for (auto& p: unique) {
        v.push_back(p);
    }
    return v;

//    std::set<std::string> unique_positions;
//    for (auto& p: a) {
//        std::stringstream s;
//        s << p;
//        unique_positions.insert(s.str());
//    }
//    for (auto& p: b) {
//        std::stringstream s;
//        s << (p - translation);
//        unique_positions.insert(s.str());
//    }
//
//    std::vector<position> result;
//    for (auto& p: unique_positions) {
//        auto pieces = aoc::split_string(p, ",");
//        result.push_back(position{
//                std::stoi(pieces[0]),
//                std::stoi(pieces[1]),
//                std::stoi(pieces[2])
//        });
//    }
//    return result;
}

size_t count_matches(const std::vector<position>& a, const std::vector<position>& b, const position& translation) {
    std::set<position> unique;
    for (auto& p: a) {
        unique.insert(p);
    }
    for (auto& p: b) {
        unique.insert(p - translation);
    }
    return a.size() + b.size() - unique.size();

//    std::set<std::string> unique_positions;
//    for (auto& p: p1) {
//        std::stringstream s;
//        s << p;
//        unique_positions.insert(s.str());
//    }
//    for (auto& p: p2) {
//        std::stringstream s;
//        s << (p - difference);
//        unique_positions.insert(s.str());
//    }
//
//    return p1.size() + p2.size() - unique_positions.size();
}

class day19 : public aoc::solution {
protected:


    void run(std::istream& in, std::ostream& out) override {

        std::vector<std::vector<position>> data;

        int current_scanner = -1;
        for (std::string line; std::getline(in, line);) {
            if (line.empty()) continue;

            if (line.substr(0, 3) == "---") {
                current_scanner++;
                data.emplace_back();
                continue;
            }

            auto coords = aoc::split_string(line, ",");
            data[current_scanner].emplace_back(
                    position{std::stoi(coords[0]),
                             std::stoi(coords[1]),
                             std::stoi(coords[2])});
        }


        auto first_scanner = data[0];
        std::deque<int> scanners_to_solve;
        for (size_t i = 1; i < data.size(); i++) scanners_to_solve.push_back(i);

        std::vector<position> scanner_positions = {{0, 0, 0}};
        while (!scanners_to_solve.empty()) {

            size_t scanner_index = scanners_to_solve.front();
            scanners_to_solve.pop_front();
            auto& second_scanner = data[scanner_index];

            std::cout << "Analyzing scanner " << scanner_index << std::endl;

//            auto bounds_1 = find_bounds(first_scanner);
//            std::cout << "bb1: " << bounds_1.first << " -> " << bounds_1.second << std::endl;

            bool match_found = false;
            for (auto xyz = 0; xyz < 24; xyz++) {
                if (match_found) break;
                std::cout << "\ttrying orientation " << xyz << std::endl;

                std::vector<position> transformed_positions{};
                transformed_positions.reserve(second_scanner.size());
                for (const auto& p: second_scanner) {
                    transformed_positions.push_back(transform(p, xyz));
                }

                for (auto& p1: first_scanner) {
                    for (auto& p2: transformed_positions) {
                        auto matches = count_matches(first_scanner, transformed_positions, p2 - p1);
                        if (matches > 1) {
                            std::cout << "\t\t- found " << matches << " matches at xyz=" << xyz << " and difference "
                                      << (p2 - p1) << std::endl;
                        }
                        if (matches >= 12) {
                            scanner_positions.push_back((p2 - p1));
                            std::cout << "\t\t- found " << matches << " matches at xyz=" << xyz << " and difference "
                                      << (p2 - p1) << std::endl;
                            match_found = true;
                            first_scanner = combine(first_scanner, transformed_positions, p2 - p1);
                            std::cout << "\t\t- size after combining: " << first_scanner.size() << std::endl;
                            break;
                        }
                    }
                    if (match_found) break;
                }
            }
            if (!match_found) {
                scanners_to_solve.push_back(scanner_index);
            }
        }
        std::cout << first_scanner.size() << std::endl;

        // part 2
        uint64_t max_distance{0};
        for (size_t i = 0; i < (scanner_positions.size() - 1); i++) {
            for (size_t j = i + 1; j < scanner_positions.size(); j++) {
                auto& p = scanner_positions[i];
                auto& q = scanner_positions[j];
                uint64_t current_distance = abs(q.x - p.x) + abs(q.y - p.y) + abs(q.z - p.z);
                if (current_distance > max_distance) {
                    max_distance = current_distance;
                }
            }
        }
        std::cout << max_distance << std::endl;
    }
};

int main(int argc, char **argv) {
    day19 solution{};
    return solution.execute(argc, argv);
}
