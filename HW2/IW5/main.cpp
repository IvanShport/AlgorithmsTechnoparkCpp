//
// Created by ivanshport on 01.12.18.
//

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include "Huffman.h"

class INPUT : public IInputStream {
public:
    INPUT(std::string data): data(std::move(data)) {}

    bool Read(byte& value) override {
        if (curPos == data.size()) {
            return false;
        }
        std::cout << "read: " << data[curPos] << "\n";
//        std::cout << "code: ";
//        byte code = data[curPos];
//        byte mask = 128;
//        while (mask) {
//            std::cout << ((code & mask) != 0);
//            mask >>= 1;
//        }
//        std::cout << "\n";
        value = data[curPos++];
        return true;
    }

private:
    size_t curPos = 0;
    std::string data;
};

class OUTPUT : public IOutputStream {
public:
    void Write(byte value) override {
        byte mask = 128;
        std::cout << "<";
        while (mask) {
            std::cout << ((value & mask) != 0);
            mask >>= 1;
        }
        read.push_back(value);
        std::cout << ">";
        std::cout << " ";
    }

    const std::string& get_read() const {
        return read;
    }

private:
    std::string read;
};

int main() {
    INPUT input = std::string("aaaaaaab");
    OUTPUT output;

    Encode(input, output);
//    INPUT input2 = output.get_read();
//    Decode(input2, output);



//    auto tree = createTreeFormMap(getWordsMap(arr, 2, len));
//
//    printTree(tree, [](word value) {
//        std::cout << " '" << (char) (value) << "'";
//    });
//
//    std::vector<bool> treeStructure;
//    getTreeCodeBits(tree, treeStructure);

    return 0;
}
