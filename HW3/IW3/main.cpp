#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <list>

#define INF 2000000000


class Graph {
    struct Node {
        Node(size_t node = 0, int weight = 0);
        size_t node;
        int weight;
    };
public:
    Graph(size_t count = 0);

    size_t get_count() const;
    const std::vector<std::vector<Node>>& get_graph() const;

    void add_edge(size_t begin, size_t end, int weight);
    void output_graph();

private:
    size_t count_node;
    std::vector<std::vector<Node>> graph;

};

size_t input_count();
Graph insert_data(size_t count_node, size_t count_edge);
int bypass(const Graph& graph, size_t begin, size_t end);

int main() {
    size_t count_node = input_count();
    size_t count_edge = input_count();
    Graph graph = insert_data(count_node, count_edge);

    size_t begin = input_count();
    size_t end = input_count();
    std::cout <<  bypass(graph, begin, end) << std::endl;

//    graph.output_graph();

    return 0;
}

Graph::Node::Node(size_t node, int weight):
node(node),
weight(weight)
{}

Graph::Graph(size_t count):
count_node(count),
graph(count)
{}

size_t Graph::get_count() const {
    return count_node;
}

const std::vector<std::vector<Graph::Node>>& Graph::get_graph() const {
    return graph;
}

void Graph::add_edge(size_t begin, size_t end, int weight) {
    graph[begin].push_back({end, weight});
    graph[end].push_back({begin, weight});

}

void Graph::output_graph() {
    for (size_t i = 0; i < count_node; i++) {
        for (auto &j : graph[i]) {
            std::cout << j.node << '(' << j.weight << ") ";
        }
        std::cout << std::endl;
    }
}

size_t input_count() {
    size_t count;
    std::cin >> count;

    return count;
}

Graph insert_data(size_t count_node, size_t count_edge) {
    Graph result(count_node);
    size_t first_index = 0;
    size_t second_index = 0;
    int weight = 0;
    for (size_t i = 0; i < count_edge; i++) {
        std::cin >> first_index >> second_index >> weight;
        result.add_edge(first_index, second_index, weight);
    }
    return result;

}

int bypass(const Graph& graph, size_t begin, size_t end) {
    std::vector<int> dist(graph.get_count(), INF);
    std::set<std::pair<int, size_t>> q;
    q.emplace(std::make_pair(0, begin));
    dist[begin] = 0;

    size_t cur_node = 0;
    while (!q.empty()) {
        cur_node = (q.begin())->second;
        q.erase(q.begin());

        for (auto &i : graph.get_graph()[cur_node]) {

            if (dist[cur_node] + i.weight < dist[i.node]) {
                if (dist[i.node] != INF) {
                    q.erase(std::make_pair(dist[i.node], i.node));
                }
                dist[i.node] = dist[cur_node] + i.weight;
                q.emplace(std::make_pair(dist[i.node], i.node));
            }
        }
    }

//    for (size_t i = 0; i < count_node; i++) {
//        std:: cout << i << ": " << dist[i] << std::endl;
//    }

    return dist[end];

}