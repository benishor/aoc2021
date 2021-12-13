#include "solution.h"

using graph = std::unordered_map<std::string, std::vector<std::string>>;
using path = std::vector<std::string>;

enum class small_cave_visiting_policy {
    once_at_most,
    twice_at_most
};

static bool inline path_contains_route(const path& p, const std::string& r) {
    return std::find(p.begin(), p.end(), r) != p.end();
}

static inline path add_route_to_path(const std::string& r, const path& p) {
    auto new_path = p;
    new_path.push_back(r);
    return new_path;
}

static inline bool is_small_cave(const std::string& s) {
    return s.front() >= 'a';
}

static size_t count_paths(graph& routes, small_cave_visiting_policy type) {
    size_t result{0};
    std::vector<path> paths = {{"start"}};

    std::vector<path> new_paths;
    new_paths.reserve(130000);

    while (!paths.empty()) {
        for (auto& p: paths) {
            for (auto& r: routes[p.back()]) {
                if (r == "start") continue;
                if (r == "end") {
                    result++;
                    continue;
                }

                if (is_small_cave(r)) {
                    if (path_contains_route(p, r)) {
                        if (type == small_cave_visiting_policy::twice_at_most && p[0][0] != '2') {
                            auto new_path = add_route_to_path(r, p);
                            new_path[0][0] = '2';
                            new_paths.emplace_back(new_path);
                        }
                    } else {
                        new_paths.emplace_back(add_route_to_path(r, p));
                    }
                } else {
                    new_paths.emplace_back(add_route_to_path(r, p));
                }
            }
        }
        paths = std::move(new_paths);
    }
    return result;
}

static std::ostream& operator<<(std::ostream& out, const path& p) {
    out << "{";
    for (auto& x: p) {
        out << x << ".";
    }
    out << "}";
    return out;
}


class day12 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {
        graph routes = read_data(in);
        out << count_paths(routes, small_cave_visiting_policy::once_at_most) << std::endl;
        out << count_paths(routes, small_cave_visiting_policy::twice_at_most) << std::endl;
    }

    static graph read_data(std::istream& in) {
        graph routes;
        for (std::string line; std::getline(in, line);) {
            auto pieces = aoc::split_string(line, "-");
            routes[pieces[0]].push_back(pieces[1]);
            routes[pieces[1]].push_back(pieces[0]);
        }
        return routes;
    }
};

int main(int argc, char **argv) {
    day12 solution{};
    return solution.execute(argc, argv);
}
