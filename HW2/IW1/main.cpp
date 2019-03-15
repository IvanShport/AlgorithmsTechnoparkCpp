#include <iostream>
#include <string>
#include <vector>

#define MIN_START_SIZE 8
#define A_CONST 0.6180339887

struct Element {
    std::string key;
    bool isDelete = false;
};

class HashTable {
public:
    explicit HashTable(size_t (*hash_func)(const std::string &,
                                           size_t) = nullptr);

    bool Has(const std::string &key) const;
    bool Add(const std::string &key);
    bool Remove(const std::string &key);

private:
    size_t next_pos(size_t index, size_t iteration, size_t prev_index) const;
    void grow();

    size_t (*hash_func)(const std::string &, size_t);
    std::vector<Element> table;
    size_t real_size;
};

HashTable::HashTable(size_t (*hash_func)(const std::string &, size_t))
        : table(MIN_START_SIZE), real_size(0) {
    this->hash_func = hash_func;
}

bool HashTable::Has(const std::string &key) const {
    size_t hash = hash_func(key, table.size());
    size_t count_of_iteration = 0;
    size_t prev_index = 0;
    size_t i = hash;

    for (; count_of_iteration < table.size() &&
           (!table[i].key.empty() || table[i].isDelete);
           i = next_pos(hash, count_of_iteration + 1, prev_index),
                   count_of_iteration++) {
        if (table[i].key == key) {
            return true;
        }
        prev_index += count_of_iteration;
    }
    return false;
}

bool HashTable::Add(const std::string &key) {
    size_t hash = hash_func(key, table.size());
    int del_elem = -1;
    size_t count_of_iteration = 0;
    size_t prev_index = 0;
    size_t i = hash;

    for (; count_of_iteration < table.size() &&
           (!table[i].key.empty() || table[i].isDelete);
           i = next_pos(hash, count_of_iteration + 1, prev_index),
                   count_of_iteration++) {

        if (table[i].key == key) {
            return false;
        }
        if (del_elem == -1 && table[i].isDelete) {
            del_elem = i;
        }
        prev_index += count_of_iteration;
    }

    if (del_elem != -1) {
        table[del_elem].key = key;
        table[del_elem].isDelete = false;
    } else {
        table[i].key = key;
    }

    real_size++;

    if (double(real_size) >= 0.75 * double(table.size())) {
        grow();
    }

    return true;
}

bool HashTable::Remove(const std::string &key) {
    size_t hash = hash_func(key, table.size());
    size_t count_of_iteration = 0;
    size_t prev_index = 0;
    size_t i = hash;

    for (; count_of_iteration < table.size() &&
           (!table[i].key.empty() || table[i].isDelete);
           i = next_pos(hash, count_of_iteration + 1, prev_index),
                   count_of_iteration++) {

        if (table[i].key == key) {
            table[i].key = "";
            table[i].isDelete = true;
            real_size--;
            return true;
        }
        prev_index += count_of_iteration;
    }
    return false;
}

size_t HashTable::next_pos(size_t index, size_t iteration,
                           size_t prev_index) const {
    return (index + iteration + prev_index) % table.size();
}

void HashTable::grow() {
    size_t new_size = table.size() * 2;
    std::vector<Element> new_table(new_size);
    std::vector<Element> prev_table = table;
    table = new_table;

    for (auto &elem : prev_table) {
        if (!elem.key.empty() && !elem.isDelete) {
            Add(elem.key);
        }
    }
}

size_t hash_function(const std::string &str, size_t size);

int main() {
    HashTable set(hash_function);
    char command = 0;
    std::string key;
    while (std::cin >> command >> key) {
        switch (command) {
            case '?':
                std::cout << (set.Has(key) ? "OK" : "FAIL") << std::endl;
                break;
            case '+':
                std::cout << (set.Add(key) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (set.Remove(key) ? "OK" : "FAIL") << std::endl;
                break;
            default:
                std::cout << "FAIL" << std::endl;
        }
    }

    return 0;
}

size_t hash_function(const std::string &str, size_t size) {
    size_t hash = 0;

    for (auto symb : str) {
        hash = int(hash * A_CONST + symb) % size;
    }
    return hash;
}