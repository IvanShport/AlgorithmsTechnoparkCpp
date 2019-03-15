#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <vector>
#include <assert.h>

template <class T> class AVL_Tree {
public:
    explicit AVL_Tree();

    void insert(T key);
    void remove(T key);

    int get_k_stat(int index) const;

    std::vector<T> bypass_level_order() const;

private:
    struct Node {
        Node(T key = T(), std::shared_ptr<Node> left = nullptr,
             std::shared_ptr<Node> right = nullptr, int height = 1,
             int leftSize = 0, int rightSize = 0);

        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        int height;
        int leftSize;
        int rightSize;
        T key;
    };

    std::shared_ptr<Node> balance(std::shared_ptr<Node> node);

    std::shared_ptr<Node> left_rotate(std::shared_ptr<Node> node);
    std::shared_ptr<Node> right_rotate(std::shared_ptr<Node> node);

    int get_height(std::shared_ptr<Node> node) const;
    void set_height_and_size(std::shared_ptr<Node> &node);

    int get_height_difference(std::shared_ptr<Node> node) const;

    std::shared_ptr<Node> find_min(std::shared_ptr<Node> node);
    std::shared_ptr<Node> remove_min(std::shared_ptr<Node> node);

    std::shared_ptr<Node> remove_node(T key, std::shared_ptr<Node> node);
    std::shared_ptr<Node> insert_node(T key, std::shared_ptr<Node> node);

    std::shared_ptr<Node> head;
};

template <class T>
AVL_Tree<T>::Node::Node(T key, std::shared_ptr<AVL_Tree<T>::Node> left,
                        std::shared_ptr<AVL_Tree<T>::Node> right, int height,
                        int leftSize, int rightSize)
        : key(key), left(left), right(right), height(height), leftSize(leftSize),
          rightSize(rightSize) {}

template <class T> AVL_Tree<T>::AVL_Tree() : head(nullptr) {}

template <class T>
int AVL_Tree<T>::get_height(std::shared_ptr<Node> node) const {
    return node != nullptr ? node->height : 0;
}

template <class T>
void AVL_Tree<T>::set_height_and_size(std::shared_ptr<Node> &node) {
    int left_height = get_height(node->left);
    int right_height = get_height(node->right);
    node->height = (right_height > left_height ? right_height : left_height) + 1;
    node->leftSize = node->left == nullptr
                     ? 0
                     : (node->left->leftSize + node->left->rightSize + 1);
    node->rightSize = node->right == nullptr
                      ? 0
                      : (node->right->leftSize + node->right->rightSize + 1);
}

template <class T>
int AVL_Tree<T>::get_height_difference(std::shared_ptr<Node> node) const {
    return get_height(node->right) - get_height(node->left);
}

template <class T>
std::shared_ptr<typename AVL_Tree<T>::Node>
AVL_Tree<T>::left_rotate(std::shared_ptr<Node> node) {
    std::shared_ptr<Node> right_node = node->right;
    node->right = right_node->left;
    right_node->left = node;
    set_height_and_size(node);
    set_height_and_size(right_node);
    return right_node;
}

template <class T>
std::shared_ptr<typename AVL_Tree<T>::Node>
AVL_Tree<T>::right_rotate(std::shared_ptr<Node> node) {
    std::shared_ptr<Node> left_node = node->left;
    node->left = left_node->right;
    left_node->right = node;
    set_height_and_size(node);
    set_height_and_size(left_node);
    return left_node;
}

template <class T>
std::shared_ptr<typename AVL_Tree<T>::Node>
AVL_Tree<T>::balance(std::shared_ptr<Node> node) {
    set_height_and_size(node);
    if (get_height_difference(node) == 2) {
        if (get_height_difference(node->right) < 0) {
            node->right = right_rotate(node->right);
        }
        return left_rotate(node);
    }
    if (get_height_difference(node) == -2) {
        if (get_height_difference(node->left) > 0) {
            node->left = left_rotate(node->left);
        }
        return right_rotate(node);
    }
    return node;
}

template <class T> void AVL_Tree<T>::insert(T key) {
    head = insert_node(key, head);
}

template <class T>
std::shared_ptr<typename AVL_Tree<T>::Node>
AVL_Tree<T>::insert_node(T key, std::shared_ptr<Node> node) {
    if (node == nullptr) {
        auto new_node = std::make_shared<Node>(key);
        return new_node;
    }
    if (key < node->key) {
        node->left = insert_node(key, node->left);
    } else {
        node->right = insert_node(key, node->right);
    }
    return balance(node);
}

template <class T> void AVL_Tree<T>::remove(T key) {
    head = remove_node(key, head);
}

template <class T>
std::shared_ptr<typename AVL_Tree<T>::Node>
AVL_Tree<T>::find_min(std::shared_ptr<Node> node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

template <class T>
std::shared_ptr<typename AVL_Tree<T>::Node>
AVL_Tree<T>::remove_min(std::shared_ptr<Node> node) {
    if (node->left == nullptr) {
        return node->right;
    }
    node->left = remove_min(node->left);
    return balance(node);
}

template <class T>
std::shared_ptr<typename AVL_Tree<T>::Node>
AVL_Tree<T>::remove_node(T key, std::shared_ptr<Node> node) {
    if (node == nullptr) {
        return nullptr;
    }
    if (key < node->key) {
        node->left = remove_node(key, node->left);
    } else if (key > node->key) {
        node->right = remove_node(key, node->right);
    } else {
        std::shared_ptr<Node> left_node = node->left;
        std::shared_ptr<Node> right_node = node->right;
        if (right_node == nullptr) {
            return left_node;
        }
        std::shared_ptr<Node> min_node = find_min(right_node);
        min_node->right = remove_min(right_node);
        min_node->left = left_node;
        return balance(min_node);
    }
    return balance(node);
}

template <class T> std::vector<T> AVL_Tree<T>::bypass_level_order() const {
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

        std::cout << cur_node->key << ' ' << cur_node->height << std::endl;
    }

    return result;
}

template <class T> int AVL_Tree<T>::get_k_stat(int index) const {
    std::shared_ptr<Node> cur_node = head;
    assert(index < (head->leftSize + head->rightSize));
    while (index != cur_node->leftSize) {
        if (index > cur_node->leftSize) {
            index -= cur_node->leftSize + 1;
            cur_node = cur_node->right;
        } else {
            cur_node = cur_node->left;
        }
    }
    return cur_node->key;
}

size_t input_count();

void input_in_AVL_Tree_and_output_k_stat(AVL_Tree<int> &avl_tree, size_t count);

void output_result(const std::vector<int> &result);

int main() {

    size_t count = input_count();
    AVL_Tree<int> avl_tree;
    input_in_AVL_Tree_and_output_k_stat(avl_tree, count);
    //    output_result(result);

    return 0;
}

size_t input_count() {
    size_t count = 0;
    std::cin >> count;

    return count;
}

void input_in_AVL_Tree_and_output_k_stat(AVL_Tree<int> &avl_tree,
                                         size_t count) {
    int temp_value = 0;
    int index = 0;

    for (size_t i = 0; i < count; i++) {
        std::cin >> temp_value >> index;
        if (temp_value >= 0) {
            avl_tree.insert(temp_value);
        } else {
            avl_tree.remove(-temp_value);
        }
        std::cout << avl_tree.get_k_stat(index) << std::endl;
    }
}

void output_result(const std::vector<int> &result) {
    for (auto &elem : result) {
        std::cout << elem << ' ';
    }
}