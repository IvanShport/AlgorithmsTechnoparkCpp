#include <iostream>
//#include <assert.h>
//#define MAX_SIZE 100000

size_t createSize();

int *createSequence(size_t size);

void findMaxSum(const int *firstSequence, const int *secondSequence, size_t size, size_t &resultI, size_t &resultJ);

void outputResult(size_t resultI, size_t resultJ);

void freeSequence(int const *sequence);

int main() {
    size_t size = createSize();
    int *firstSequence = createSequence(size);
    int *secondSequence = createSequence(size);

    size_t resultI = 0, resultJ = 0;
    findMaxSum(firstSequence, secondSequence, size, resultI, resultJ);

    outputResult(resultI, resultJ);
    freeSequence(firstSequence);
    freeSequence(secondSequence);

    return 0;
}

size_t createSize() {
    size_t size = 0;
    std::cin >> size;

    return size;
}

int *createSequence(size_t size) {
    //assert(size <= MAX_SIZE && size > 1);
    auto array = new int[size];
    for (size_t i = 0; i < size; i++) {
        std::cin >> array[i];
    }

    return array;
}

void
findMaxSum(const int *firstSequence, const int *secondSequence, const size_t size, size_t &resultI, size_t &resultJ) {
    int curSum = 0;
    int prevSum = 0;
    size_t maxI = 0;

    for (size_t i = 1; i < size; i++) {
        curSum = firstSequence[i] + secondSequence[i];
        prevSum = firstSequence[maxI] + secondSequence[i];

        if (curSum > prevSum)
            maxI = i;
        if (prevSum > firstSequence[resultI] + secondSequence[resultJ]) {
            resultI = maxI;
            resultJ = i;
        }
        if (curSum > firstSequence[resultI] + secondSequence[resultJ]) {
            resultI = i;
            resultJ = i;
        }
    }
}

void outputResult(const size_t resultI, const size_t resultJ) {
    std::cout << resultI << ' ' << resultJ << std::endl;
}

void freeSequence(int const *sequence) {
    delete[] sequence;
}
