#include <iostream>
#include <assert.h>

#define MAX_SIZE 10000

size_t createSize();

int *createSequence(size_t size);

int findMinForValue(int *sortedSequence, int size, int value);

int findEndIndex(const int *sortedSequence, int size, int value);

void outputValue(const int *sequence, const size_t size);

void freeSequence(int const *sequence);

int main() {
    size_t sizeA = createSize();
    size_t sizeB = createSize();
    int *sortedArray = createSequence(sizeA);
    int *unsortedArray = createSequence(sizeB);

    for (size_t i = 0; i < sizeB; i++) {
        unsortedArray[i] = findMinForValue(sortedArray, (int) sizeA, unsortedArray[i]);
    }

    outputValue(unsortedArray, sizeB);
    freeSequence(sortedArray);
    freeSequence(unsortedArray);

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

int findMinForValue(int *sortedSequence, const int size, int value) {
    int indexOfEnd = findEndIndex(sortedSequence, size, value);
    int indexOfStart = indexOfEnd / 2;

    int middle = 0;
    while (indexOfEnd - indexOfStart > 1) {
        middle = (indexOfStart + indexOfEnd) >> 1;  // (indexOfStart + indexOfEnd) / 2
        if (sortedSequence[middle] >= value)
            indexOfEnd = middle;
        else
            indexOfStart = middle + 1;
    }

    if (sortedSequence[indexOfStart] >= value)
        return indexOfStart;
    else if (sortedSequence[indexOfEnd] >= value)
        return indexOfEnd;
    else
        return size;
}

int findEndIndex(const int *sortedSequence, const int size, int value) {
    int indexOfEnd = 0;
    if (sortedSequence[indexOfEnd] >= value)
        return indexOfEnd;
    indexOfEnd = 1;

    while (indexOfEnd < size && sortedSequence[indexOfEnd] < value) {
        indexOfEnd *= 2;
    }

    if (indexOfEnd >= size) {
        indexOfEnd = size - 1;
    }

    return indexOfEnd;
}

void outputValue(const int *sequence, const size_t size) {
    for (size_t i = 0; i < size; i++) {
        std::cout << sequence[i] << std::endl;
    }
}

void freeSequence(int const *sequence) {
    delete[] sequence;
}
