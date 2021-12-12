#include "solution.h"

class day12 : public aoc::solution {
protected:


    static bool is_small_cave(const std::string& s) {
        return s.front() >= 'a' && s.front() <= 'z';
    }

    void run(std::istream& in, std::ostream& out) override {

        // read data
        std::unordered_map<std::string, std::vector<std::string>> routes;
        for (std::string line; std::getline(in, line);) {
            auto pieces = aoc::split_string(line, "-");
            routes[pieces[0]].push_back(pieces[1]);
            routes[pieces[1]].push_back(pieces[0]);
        }

        // solve part1
        using path = std::vector<std::string>;
        std::vector<path> final_routes;
        std::vector<path> paths;
        std::vector<path> new_paths;

        path first;
        first.push_back("start");
        paths.push_back(first);

        while (!paths.empty()) {
            new_paths.clear();
            for (auto& p: paths) {
                if (p.back() == "end") {
                    final_routes.push_back(p);
                } else {
                    std::cout << "Evaluating path: ";
                    for (auto& x : p) {
                        std::cout << x << ".";
                    }
                    std::cout << std::endl;

                    std::cout << "\tavailable routes: ";
                    for (auto& x : routes[p.back()]) {
                        std::cout << x << ", ";
                    }
                    std::cout << std::endl;

                    for (auto& r: routes[p.back()]) {
                        if (!is_small_cave(r) || std::find(p.begin(), p.end(), r) == p.end()) {
                            auto new_p = p;
                            new_p.push_back(r);
                            new_paths.push_back(new_p);

                            std::cout << "\tadding path: ";
                            for (auto& x : new_p) {
                                std::cout << x << ".";
                            }
                            std::cout << std::endl;

                        } else {
                            std::cout << "Ending candidate route: ";
                            for (auto& x : p) {
                                std::cout << x << ".";
                            }
                            std::cout << std::endl;
                        }
                    }
                }
            }
            paths = new_paths;
        }

        out << final_routes.size() << std::endl;

    }
};

int main(int argc, char **argv) {
    day12 solution{};
    return solution.execute(argc, argv);
}
