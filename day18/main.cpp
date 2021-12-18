#include "solution.h"

enum class node_type {
    pair,
    literal
};

struct node {
    node_type type;
    int64_t value;
    struct node *left;
    struct node *right;
    struct node *parent;

    static node *literal_node(int64_t v) {
        node *result = new node();
        result->type = node_type::literal;
        result->value = v;
        result->parent = nullptr;
        return result;
    }

    static node *pair_node(node *left, node *right) {
        node *result = new node();
        result->type = node_type::pair;
        result->left = left;
        result->right = right;
        result->parent = nullptr;
        return result;
    }
};

node *deep_clone(node *n) {
    if (n->type == node_type::literal) {
        return node::literal_node(n->value);
    } else {
        return node::pair_node(deep_clone(n->left), deep_clone(n->right));
    }
}


void dump_number(node *nr, std::ostream& out) {
    if (nr->type == node_type::pair) {
        out << "[";
        dump_number(nr->left, out);
        out << ",";
        dump_number(nr->right, out);
        out << "]";
    } else {
        out << nr->value;
    }
}

node *parse_number(std::string text) {
    std::stack<node *> nodes;

    for (size_t i = 0; i < text.size(); i++) {
        char c = text[i];
        switch (c) {
            case '[':
                // nothing to do
                break;
            case ']': {
                node *right = nodes.top();
                nodes.pop();
                node *left = nodes.top();
                nodes.pop();
                node *parent = node::pair_node(left, right);
                left->parent = parent;
                right->parent = parent;
                nodes.push(parent);
                break;
            }
            case ',':
                // nothing to do
                break;
            default:
                size_t nr_len = {1};
                while (std::isdigit(text[i + nr_len])) {
                    nr_len++;
                }
                nodes.push(node::literal_node(std::stoi(text.substr(i, nr_len))));
                i += nr_len - 1;
                break;
        }
    }
    node *result = nodes.top();
    nodes.pop();
    return result;
}

node *add(node *a, node *b) {
    return node::pair_node(a, b);
}

node *first_to_explode(node *nr, int& nesting) {
    if (nr->type == node_type::pair) {
        nesting++;
        if (nesting == 5) {
            return nr;
        }

        auto result = first_to_explode(nr->left, nesting);
        if (result != nullptr) return result;

        result = first_to_explode(nr->right, nesting);
        if (result != nullptr) return result;

        nesting--;
    }
    return nullptr;
}

void get_all_literals(node *number, node *pair_to_keep, std::vector<node *>& nodes) {
    if (number->type == node_type::literal || pair_to_keep == number) {
        nodes.push_back(number);
        return;
    }
    get_all_literals(number->left, pair_to_keep, nodes);
    get_all_literals(number->right, pair_to_keep, nodes);
}

node *first_to_split(node *nr) {
    std::vector<node *> nodes;
    get_all_literals(nr, nullptr, nodes);
    for (auto& l: nodes) {
        if (l->value >= 10) {
            return l;
        }
    }
    return nullptr;
}

node *find_first_reduceable(node *nr, int& nesting) {
    if (nr->type == node_type::pair) {
        nesting++;
        if (nesting == 5) {
            return nr;
        }

        if (nr->left->type == node_type::literal) {
            if (nr->left->value >= 10) return nr->left;
        } else {
            auto result = find_first_reduceable(nr->left, nesting);
            if (result != nullptr) return result;
        }

        if (nr->right->type == node_type::literal) {
            if (nr->right->value >= 10) return nr->right;
        } else {
            auto result = find_first_reduceable(nr->right, nesting);
            if (result != nullptr) return result;
        }

        nesting--;
    }
    return nullptr;
}


void explode(node *pair, node *number) {
    std::vector<node *> literal_nodes;
    get_all_literals(number, pair, literal_nodes);

    for (size_t i = 0; i < literal_nodes.size(); i++) {
        if (literal_nodes[i] == pair) {
            if (i > 0) {
                literal_nodes[i - 1]->value += pair->left->value;
            }
            if (i < (literal_nodes.size() - 1)) {
                literal_nodes[i + 1]->value += pair->right->value;
            }
            break;
        }
    }

    // Then, the entire exploding pair is replaced with the regular number 0.
    pair->type = node_type::literal;
    pair->value = 0;
    delete pair->left;
    delete pair->right;
    pair->left = pair->right = nullptr;
}

void split(node *literal) {
    // To split a regular number, replace it with a pair; the left element of the pair should be the regular number divided by two and
    // rounded down, while the right element of the pair should be the regular number divided by two and rounded up.
    // For example, 10 becomes [5,5], 11 becomes [5,6], 12 becomes [6,6], and so on.
    literal->type = node_type::pair;
    literal->left = node::literal_node(floor(literal->value / 2.0));
    literal->left->parent = literal;
    literal->right = node::literal_node(ceil(literal->value / 2.0));
    literal->right->parent = literal;
}

node *reduce(node *number) {
    int nesting{0};
    node *to_reduce;
    while (true) {
        bool action_taken{false};
        if ((to_reduce = first_to_explode(number, nesting)) != nullptr) {
            explode(to_reduce, number);
            action_taken = true;
            nesting = 0;
        }
        if (action_taken) continue;
        if ((to_reduce = first_to_split(number)) != nullptr) {
            split(to_reduce);
            action_taken = true;
        }
        if (!action_taken) break;
    }
    return number;
}

int64_t magnitude(node *nr) {
    if (nr->type == node_type::literal) {
        return nr->value;
    } else {
        return magnitude(nr->left) * 3 + magnitude(nr->right) * 2;
    }
}

class day18 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {

        std::vector<std::string> lines;
        std::deque<node *> numbers;
        for (std::string line; std::getline(in, line);) {
            numbers.push_back(parse_number(line));
            lines.push_back(line);
        }

        while (numbers.size() > 1) {
            node *a = numbers.front();
            numbers.pop_front();

            node *b = numbers.front();
            numbers.pop_front();

            numbers.push_front(reduce(add(a, b)));
        }
        out << magnitude(numbers.front()) << std::endl;

        int64_t largest_magnitude{0};
        for (size_t i = 0; i < (lines.size() - 1); i++) {
            for (size_t j = i; j < lines.size(); j++) {
                auto m1 = magnitude(reduce(add(parse_number(lines[i]), parse_number(lines[j]))));
                auto m2 = magnitude(reduce(add(parse_number(lines[j]), parse_number(lines[i]))));
                largest_magnitude = std::max(largest_magnitude, m1);
                largest_magnitude = std::max(largest_magnitude, m2);
            }
        }
        out << largest_magnitude << std::endl;
    }
};

int main(int argc, char **argv) {
    day18 solution{};
    return solution.execute(argc, argv);
}
