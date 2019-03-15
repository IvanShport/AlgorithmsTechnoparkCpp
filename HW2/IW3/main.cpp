#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <utility>
#include <vector>

template <class T> class BinSearchTree {
public:
    explicit BinSearchTree();

    void insert(T key);

    int max_height();

private:
    struct Node {
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        T key;
    };

    std::shared_ptr<Node> head;
};

template <class T> class Treap {
public:
    explicit Treap();

    void insert(T key, int priority);

    int max_height();

private:
    struct Node {
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        int priority;
        T key;
    };

    void split(std::shared_ptr<Node> cur_node, T key, std::shared_ptr<Node> &left,
               std::shared_ptr<Node> &right);

    std::shared_ptr<Node> head;
};

// ------------------------------------------ BinSearchTree ------------------------------------------------------------

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

template <class T> int BinSearchTree<T>::max_height() {
    std::queue<std::pair<std::shared_ptr<Node>, int>> queue_for_bypass;
    int max_height = 0;
    queue_for_bypass.push(std::make_pair(head, 0));

    std::pair<std::shared_ptr<Node>, int> cur_node;
    while (!queue_for_bypass.empty()) {
        cur_node = queue_for_bypass.front();
        queue_for_bypass.pop();

        if (cur_node.second > max_height) {
            max_height = cur_node.second;
        }
        if (cur_node.first->left != nullptr) {
            queue_for_bypass.push(
                    std::make_pair(cur_node.first->left, cur_node.second + 1));
        }
        if (cur_node.first->right != nullptr) {
            queue_for_bypass.push(
                    std::make_pair(cur_node.first->right, cur_node.second + 1));
        }
    }
    return max_height;
}

// ------------------------------------------------- Treap -------------------------------------------------------------

template <class T> Treap<T>::Treap() : head(nullptr) {}

template <class T> void Treap<T>::insert(T key, int priority) {
    auto new_node = std::make_shared<Node>();

    std::shared_ptr<Node> left_node = nullptr;
    std::shared_ptr<Node> right_node = nullptr;

    std::shared_ptr<Node> parent = nullptr;
    std::shared_ptr<Node> cur_node = head;
    while (cur_node != nullptr && priority <= cur_node->priority) {
        parent = cur_node;
        if (key < cur_node->key) {
            cur_node = cur_node->left;
        } else if (key > cur_node->key) {
            cur_node = cur_node->right;
        }
    }

    split(cur_node, key, left_node, right_node);

    new_node->left = left_node;
    new_node->right = right_node;
    new_node->key = key;
    new_node->priority = priority;

    if (parent == nullptr) {
        head = new_node;
    } else if (key < parent->key) {
        parent->left = new_node;
    } else if (key >= parent->key) {
        parent->right = new_node;
    }
}

template <class T>
void Treap<T>::split(std::shared_ptr<Node> cur_node, T key,
                     std::shared_ptr<Node> &left,
                     std::shared_ptr<Node> &right) {
    if (cur_node == nullptr) {
        left = nullptr;
        right = nullptr;
    } else if (key >= cur_node->key) {
        split(cur_node->right, key, cur_node->right, right);
        left = cur_node;
    } else {
        split(cur_node->left, key, left, cur_node->left);
        right = cur_node;
    }
}

template <class T> int Treap<T>::max_height() {
    std::queue<std::pair<std::shared_ptr<Node>, int>> queue_for_bypass;
    int max_height = 0;
    queue_for_bypass.push(std::make_pair(head, 0));

    std::pair<std::shared_ptr<Node>, int> cur_node;
    while (!queue_for_bypass.empty()) {
        cur_node = queue_for_bypass.front();
        queue_for_bypass.pop();

        if (cur_node.second > max_height) {
            max_height = cur_node.second;
        }
        if (cur_node.first->left != nullptr) {
            queue_for_bypass.push(
                    std::make_pair(cur_node.first->left, cur_node.second + 1));
        }
        if (cur_node.first->right != nullptr) {
            queue_for_bypass.push(
                    std::make_pair(cur_node.first->right, cur_node.second + 1));
        }
    }
    return max_height;
}

// ------------------------------------------------- Main --------------------------------------------------------------

size_t input_count();

void input_in_BST(BinSearchTree<int> &BST, Treap<int> &treap, size_t count);

int main() {

    size_t count = input_count();
    BinSearchTree<int> BST;
    Treap<int> treap;
    input_in_BST(BST, treap, count);

    std::cout << abs(BST.max_height() - treap.max_height());

    return 0;
}

size_t input_count() {
    size_t count = 0;
    std::cin >> count;

    return count;
}

void input_in_BST(BinSearchTree<int> &BST, Treap<int> &treap,
                  const size_t count) {
    int temp_value = 0;
    int temp_priority = 0;

    for (size_t i = 0; i < count; i++) {
        std::cin >> temp_value >> temp_priority;
        BST.insert(temp_value);
        treap.insert(temp_value, temp_priority);
    }
}