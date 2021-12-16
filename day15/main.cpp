#include "solution.h"
#include <list>
#include <queue>


using target_cost_pair = std::pair<size_t, size_t>;

struct weighted_graph {
    explicit weighted_graph(size_t node_count);
    void add_edge(size_t from_node, size_t to_node, size_t weight_going, size_t weight_returning) const;
    [[nodiscard]] size_t shortest_path_cost(size_t source_node, size_t destination_node) const;

    const size_t nr_nodes;
    std::list<target_cost_pair> *edges;
};

weighted_graph::weighted_graph(size_t node_count) : nr_nodes{node_count} {
    edges = new std::list<target_cost_pair>[node_count];
}

void weighted_graph::add_edge(size_t from_node, size_t to_node, size_t weight_going, size_t weight_returning) const {
    edges[from_node].emplace_back(to_node, weight_going);
    edges[to_node].emplace_back(from_node, weight_returning);
}

size_t weighted_graph::shortest_path_cost(size_t source_node, size_t destination_node) const {
    std::priority_queue<target_cost_pair, std::vector<target_cost_pair>, std::greater<>> pq;

    std::vector<size_t> distance_to(nr_nodes, std::numeric_limits<size_t>::max());

    pq.push({0, source_node});
    distance_to[source_node] = 0;

    while (!pq.empty()) {
        size_t current_node = pq.top().second;
        pq.pop();

        for (auto& edge: edges[current_node]) {
            size_t next_node = edge.first;
            size_t cost = edge.second;

            if (distance_to[next_node] > distance_to[current_node] + cost) {
                distance_to[next_node] = distance_to[current_node] + cost;
                pq.push({distance_to[next_node], next_node});
            }
        }
    }

    return distance_to[destination_node];
}

class day15 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {
        const int Side = 100;
        auto data = read_data<Side>(in);

        auto g1 = to_weighted_graph<Side>(data);
        out << g1.shortest_path_cost(0, (Side * Side) - 1) << std::endl;

        auto data_stitched = stich_data<Side>(data);

        auto g2 = to_weighted_graph<Side * 5>(data_stitched);
        out << g2.shortest_path_cost(0, (Side * 5 * 5 * Side) - 1);
    }

    static inline uint8_t roll_over(uint8_t data, uint8_t distance) {
        return (data - 1 + distance) % 9 + 1;
    }

    template<std::size_t side>
    static inline std::array<std::array<uint8_t, side * 5>, side * 5>
    stich_data(const std::array<std::array<uint8_t, side>, side>& data) {
        std::array<std::array<uint8_t, side * 5>, side * 5> data_stitched{};
        for (size_t y = 0; y < side * 5; y++) {
            for (size_t x = 0; x < side * 5; x++) {
                data_stitched[y][x] = data[y][x];
                size_t replications_on_x = x / side;
                size_t replications_on_y = y / side;
                size_t replications = replications_on_x + replications_on_y;
                data_stitched[y][x] = roll_over(data[y % side][x % side], replications);
            }
        }
        return data_stitched;
    }

    template<std::size_t side>
    static inline weighted_graph to_weighted_graph(const std::array<std::array<uint8_t, side>, side>& data) {
        weighted_graph g{side * side};
        for (size_t y = 0; y < side; y++) {
            for (size_t x = 0; x < side; x++) {
                if (x < (side - 1)) {
                    auto src_node = y * side + x;
                    auto dst_node = y * side + x + 1;
                    auto cost = data[y][x + 1];
                    auto cost_returning = data[y][x];
                    g.add_edge(src_node, dst_node, cost, cost_returning);
                }
                if (y < (side - 1)) {
                    auto src_node = y * side + x;
                    auto dst_node = (y + 1) * side + x;
                    auto cost = data[y + 1][x];
                    auto cost_returning = data[y][x];
                    g.add_edge(src_node, dst_node, cost, cost_returning);
                }
            }
        }
        return g;
    }

    template<std::size_t side>
    static inline std::array<std::array<uint8_t, side>, side> read_data(std::istream& in) {
        std::array<std::array<uint8_t, side>, side> data{};
        size_t y = 0;
        for (std::string line; std::getline(in, line);) {
            for (size_t x = 0; x < line.size(); x++) {
                data[y][x] = line[x] - '0';
            }
            y++;
        }
        return data;
    }
};

int main(int argc, char **argv) {
    day15 solution{};
    return solution.execute(argc, argv);
}
