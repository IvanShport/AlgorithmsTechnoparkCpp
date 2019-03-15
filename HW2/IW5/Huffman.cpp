#include "Huffman.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

struct FrequencyOfChar;

typedef std::unordered_map<char, long> encode_dict;
typedef std::shared_ptr<FrequencyOfChar> p_node;
typedef bool bit;

class BitOut {
public:
    BitOut(IOutputStream &stream);

    bool write(bit value);

private:
    IOutputStream &stream;
    byte currentByte;
    char bitsWritten;
};

class BitIn {
public:
    BitIn(IInputStream &stream);

    bool read(bit &value);

private:
    IInputStream &stream;
    byte currentByte;
    char bitsRead;
};

struct FrequencyOfChar {
    std::shared_ptr<FrequencyOfChar> left;
    std::shared_ptr<FrequencyOfChar> right;
    char symbol;
    int freq;

    explicit FrequencyOfChar(char symbol = 0, int freq = 0)
            : symbol(symbol), freq(freq), left(nullptr), right(nullptr) {}
};

struct LessByFreq {
    bool operator()(const p_node &left, const p_node &right) const {
        if (left->freq != right->freq) {
            return left->freq > right->freq;
        } else {
            return left->symbol > right->symbol;
        }
    }
};

BitIn::BitIn(IInputStream &stream) : stream(stream), bitsRead(0) {}

bool BitIn::read(bit &value) {
    if (bitsRead == 0) {
        byte tmp = 0;
        if (!stream.Read(tmp))
            return false;
        else {
            currentByte = 0;
            for (int i = 0; i < 8; i++) {
                currentByte <<= 1;
                currentByte |= 1 & tmp;
                tmp >>= 1;
            }
        }
    }
    value = currentByte & 1;
    currentByte >>= 1;
    bitsRead = (bitsRead + 1) % 8;
    return true;
}

BitOut::BitOut(IOutputStream &stream) : stream(stream), bitsWritten(0) {}

bool BitOut::write(bit value) {
    currentByte <<= 1;
    currentByte |= (value) ? 1 : 0;
    bitsWritten++;
    if (bitsWritten == 8) {
        stream.Write(currentByte);
        bitsWritten = 0;
        return true;
    }
    return false;
}

p_node create_tree(const std::unordered_map<byte, long> &alphabet);

void walk(const p_node &head, long code, encode_dict &dict);

std::unordered_map<byte, char> get_len_of_codes(const encode_dict &dict);

char get_max_len(const std::unordered_map<byte, char> &lens);

void del_empty_left_branch(p_node node);

void decode_inf(IOutputStream &original, const p_node &head,
                const std::string &text, const byte &rest_bits);

void Encode(IInputStream &original, IOutputStream &compressed) {
    std::unordered_map<byte, long> alphabet;
    encode_dict result;

    std::string text;
    byte symbol = 0;
    while (original.Read(symbol)) {
        alphabet[symbol]++;
        text += symbol;
    }

    p_node tree = create_tree(alphabet);
    walk(tree, 1, result);

    std::unordered_map<byte, char> len_of_code = get_len_of_codes(result);
    char max_len = get_max_len(len_of_code);

    size_t encode_size = 0;
    byte alphabet_size = 0;

    for (auto i : alphabet) {
        if (i.second != 0) {
            alphabet_size++;
        }
        encode_size = (encode_size + i.second * len_of_code[i.first]) % 8;
    }
    byte rest_bits = (8 - encode_size) % 8;
    byte bytes_in_code = (max_len % 8 == 0) ? (max_len / 8) : (max_len / 8 + 1);

    BitOut bout(compressed);
    compressed.Write(alphabet_size);
    compressed.Write(rest_bits);
    compressed.Write(bytes_in_code);

    long code = 0;
    int j = 0;
    for (auto i : result) {
        if (alphabet[i.first] != 0) {
            code = i.second;
            compressed.Write(byte(i.first));

            while (code != 1) {
                bout.write(code & 1);
                code >>= 1;
                j++;
            }
            for (; j < bytes_in_code * 8; j++) {
                bout.write(0);
            }
        }
        j = 0;
    }

    for (auto i : text) {
        code = result[i];

        while (code != 1) {
            bout.write(code & 1);
            code >>= 1;
        }
    }
    for (size_t i = 0; i < rest_bits; i++) {
        bout.write(0);
    }
}

void Decode(IInputStream &compressed, IOutputStream &original) {
    byte alphabet_size, rest_bits, bytes_in_code;
    compressed.Read(alphabet_size);
    compressed.Read(rest_bits);
    compressed.Read(bytes_in_code);

    auto head = std::make_shared<FrequencyOfChar>(0, 0);
    BitIn bin(compressed);

    byte symbol = 0;
    p_node cur_node = nullptr;
    bit cur_bit;
    for (int i = 0; i < alphabet_size; i++) {
        cur_node = head;
        compressed.Read(symbol);

        for (int j = 0; j < 8 * bytes_in_code; j++) {
            bin.read(cur_bit);
            if (cur_bit == 0) {
                if (cur_node->left == nullptr) {
                    cur_node->left = std::make_shared<FrequencyOfChar>(symbol, 0);
                }
                cur_node = cur_node->left;
            } else {
                if (cur_node->right == nullptr) {
                    cur_node->right = std::make_shared<FrequencyOfChar>(symbol, 0);
                }
                cur_node = cur_node->right;
            }
        }
    }
    del_empty_left_branch(head);

    std::string text;
    while (compressed.Read(symbol)) {
        text += symbol;
    }

    decode_inf(original, head, text, rest_bits);
}

p_node create_tree(const std::unordered_map<byte, long> &alphabet) {
    std::vector<p_node> frequncies;
    for (auto leter : alphabet) {
        if (leter.second != 0) {
            frequncies.push_back(
                    std::make_shared<FrequencyOfChar>(leter.first, leter.second));
        }
    }
    std::priority_queue<p_node, std::vector<p_node>, LessByFreq>
            queue_for_min_freq(frequncies.begin(), frequncies.end());
    while (queue_for_min_freq.size() >= 2) {
        auto first = queue_for_min_freq.top();
        queue_for_min_freq.pop();
        auto second = queue_for_min_freq.top();
        queue_for_min_freq.pop();

        auto new_node =
                std::make_shared<FrequencyOfChar>(0, first->freq + second->freq);
        new_node->left = first;
        new_node->right = second;
        queue_for_min_freq.push(new_node);
    }

    return queue_for_min_freq.top();
}

void walk(const p_node &head, long code, encode_dict &dict) {
    if (head->right == nullptr && head->left == nullptr) {
        long result = 1;
        while (code != 1) {
            result <<= 1;
            result |= code & 1;
            code >>= 1;
        }
        dict[head->symbol] = result != 1 ? result : 2;
        return;
    }
    if (head->left != nullptr) {
        walk(head->left, code << 1, dict);
    }
    if (head->right != nullptr) {
        walk(head->right, (code << 1) + 1, dict);
    }
}

std::unordered_map<byte, char> get_len_of_codes(const encode_dict &dict) {
    std::unordered_map<byte, char> result;
    long code = 0;
    for (auto i : dict) {
        result[i.first] = 0;
        if (i.second != 0) {
            code = i.second;
            while (code != 1) {
                result[i.first]++;
                code >>= 1;
            }
        }
    }
    return result;
}

char get_max_len(const std::unordered_map<byte, char> &lens) {
    char max = 0;
    for (auto i : lens) {
        if (i.second > max) {
            max = i.second;
        }
    }
    return max;
}

void del_empty_left_branch(p_node node) {
    if (node == nullptr) {
        return;
    }

    if (node->left != nullptr && node->right == nullptr) {
        node->left = nullptr;
        return;
    }

    del_empty_left_branch(node->left);
    del_empty_left_branch(node->right);
}

void decode_inf(IOutputStream &original, const p_node &head,
                const std::string &text, const byte &rest_bits) {
    p_node cur_node = head;
    for (int i = 0; i < text.length() - 1; i++) {
        for (int j = 7; j >= 0; j--) {
            if (((text[i] >> j) & 1) == 0) {
                cur_node = cur_node->left;
            } else {
                cur_node = cur_node->right;
            }
            if (cur_node->left == nullptr) {
                original.Write(cur_node->symbol);
                cur_node = head;
            }
        }
    }
    for (int j = 7; j >= rest_bits; j--) {
        if (((text[text.length() - 1] >> j) & 1) == 0) {
            cur_node = cur_node->left;
        } else {
            cur_node = cur_node->right;
        }
        if (cur_node->left == nullptr) {
            original.Write(cur_node->symbol);
            cur_node = head;
        }
    }
}
