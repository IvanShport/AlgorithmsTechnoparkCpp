#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <vector>

template <class T> class BinSearchTree {
public:
    explicit BinSearchTree();

    void insert(T key);

    std::vector<T> bypass_level_order();

private:
    struct Node {
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        T key;
    };

    std::shared_ptr<Node> head;
};

template <class T> BinSearchTree<T>::BinSearchTree() : head(nullptr) {}

template <class T> void BinSearchTree<T>::insert(T key) {
    auto new_node = std::make_shared<Node>();

    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->key = key;

    if (head == nullptr) {
        head = new_node;
        return;
    }

    std::shared_ptr<Node> cur_node = head;
    while (key < cur_node->key && cur_node->left != nullptr ||
           key > cur_node->key && cur_node->right != nullptr) {
        if (key < cur_node->key) {
            cur_node = cur_node->left;
        } else if (key > cur_node->key) {
            cur_node = cur_node->right;
        }
    }

    if (key < cur_node->key) {
        cur_node->left = new_node;
    } else if (key > cur_node->key) {
        cur_node->right = new_node;
    }
}

template <class T> std::vector<T> BinSearchTree<T>::bypass_level_order() {
    std::queue<std::shared_ptr<Node>> queue_for_bypass;
    queue_for_bypass.push(head);
    std::vector<T> result;

    std::shared_ptr<Node> cur_node;
    while (!queue_for_bypass.empty()) {
        cur_node = queue_for_bypass.front();
        queue_for_bypass.pop();

        if (cur_node->left != nullptr) {
            queue_for_bypass.push(cur_node->left);
        }
        if (cur_node->right != nullptr) {
            queue_for_bypass.push(cur_node->right);
        }
        result.push_back(cur_node->key);
    }

    return result;
}

size_t input_count();

void input_in_BST(BinSearchTree<int> &BST, size_t count);

void output_result(const std::vector<int> &result);

int main() {

    size_t count = input_count();
    BinSearchTree<int> BST;
    input_in_BST(BST, count);
    auto result = BST.bypass_level_order();
    output_result(result);

    return 0;
}

size_t input_count() {
    size_t count = 0;
    std::cin >> count;

    return count;
}

void input_in_BST(BinSearchTree<int> &BST, const size_t count) {
    int temp_value = 0;

    for (size_t i = 0; i < count; i++) {
        std::cin >> temp_value;
        BST.insert(temp_value);
    }
}

void output_result(const std::vector<int> &result) {
    for (auto &elem : result) {
        std::cout << elem << ' ';
    }
}