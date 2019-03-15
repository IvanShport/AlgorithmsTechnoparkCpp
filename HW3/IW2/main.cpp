#include <iostream>
#include <vector>
#include <queue>
#include <list>

size_t input_count();
std::vector<std::list<int>> insert_data(size_t count_node, size_t count_edge);
int bypass(const std::vector<std::list<int>>& matrix, size_t count_node, size_t begin, size_t end);
void output_result(const std::vector<std::list<int>>& matrix, size_t count_node);

int main() {
    size_t count_node = input_count();
    size_t count_edge = input_count();
    std::vector<std::list<int>> matrix = insert_data(count_node, count_edge);

    size_t begin = input_count();
    size_t end = input_count();

    std::cout << bypass(matrix, count_node, begin, end) << std::endl;

//    output_result(matrix, count_node);

    return 0;
}

size_t input_count() {
    size_t count;
    std::cin >> count;

    return count;
}

std::vector<std::list<int>> insert_data(size_t count_node, size_t count_edge) {
    std::vector<std::list<int>> result(count_node);
    size_t first_index = 0;
    size_t second_index = 0;
    for (size_t i = 0; i < count_edge; i++) {
        std::cin >> first_index >> second_index;
        result[first_index].push_back(second_index);
        result[second_index].push_back(first_index);
    }
    return result;

}

int bypass(const std::vector<std::list<int>>& matrix, size_t count_node, size_t begin, size_t end) {
    std::vector<int> dist(count_node, 0);
    std::vector<int> paths(count_node, 0);
    std::queue<int> q;
    q.push(begin);
    paths[begin] = 1;

    int cur_node = 0;
    while (!q.empty()) {
        cur_node = q.front();
        q.pop();

        for (auto i : matrix[cur_node]) {

            if (paths[i] == 0) {
                q.push(i);
                paths[i] = paths[cur_node];
                dist[i] = dist[cur_node] + 1;
            } else if (dist[i] == dist[cur_node] + 1) {
                paths[i] += paths[cur_node];
            }
        }
    }

//    for (size_t i = 0; i < count_node; i++) {
//        std:: cout << i << ": " << dist[i] << std::endl;
//    }
//    std::cout << paths[end] << std::endl;

    return paths[end];

}

void output_result(const std::vector<std::list<int>>& matrix, size_t count_node) {
    for (size_t i = 0; i < count_node; i++) {
        for (auto j : matrix[i]) {
            std::cout << j << ' ';
        }
        std::cout << std::endl;
    }
}