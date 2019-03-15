#include <iostream>
//#include <assert.h>
//#define MAX_SIZE 100000
#include <assert.h>
#include <cstring>

// --------------------------------- VECTOR ----------------------------------------------------------------------------

template<class T>
class Vector {
public:
    Vector(size_t size = 8);

    ~Vector();

    size_t getSize() const;

    void pushBack(const T &ath);

    void popBack();

    const T &operator[](size_t index) const;

    T &operator[](size_t index);

private:
    T *buffer;

    size_t maxSize;
    size_t size;

    void reallocForBuffer();
};

template<class T>
Vector<T>::Vector(size_t size):
        maxSize(size),
        size(0) {
    buffer = new T[maxSize];
}

template<class T>
Vector<T>::~Vector() {
    delete[] buffer;
}

template<class T>
size_t Vector<T>::getSize() const {
    return buffer != nullptr ? size : 0;
}

template<class T>
void Vector<T>::pushBack(const T &ath) {
    if (size == maxSize) {
        reallocForBuffer();
    }
    buffer[size++] = ath;
}

template<class T>
void Vector<T>::popBack() {
    if (size == 0)
        return;
    size--;
}

template<class T>
const T &Vector<T>::operator[](size_t index) const {
    assert(index < size);
    return buffer[index];
}

template<class T>
T &Vector<T>::operator[](size_t index) {
    assert(index < size);
    return buffer[index];
}

template<class T>
void Vector<T>::reallocForBuffer() {
    T *temp = new T[maxSize * 2];
    memcpy(temp, buffer, maxSize * sizeof(T));
    delete[] buffer;
    buffer = temp;
    maxSize *= 2;
}

// -------------------------------- HEAP -------------------------------------------------------------------------------

template<class T>
class Heap {
public:
    Heap(const Vector<T> &sequence, size_t new_size);

    void addValue(const T &new_num);

    void deleteTop();

    const T &getTop() const;

    const size_t getSize() const;

private:
    void SiftDown(size_t index);

    void SiftUp(size_t index);

    Vector<T> heap;
    size_t size;
};

template<class T>
Heap<T>::Heap(const Vector<T> &sequence, const size_t new_size):
        size(new_size),
        heap(8) {
    for (size_t i = 0; i < size; i++) {
        heap.pushBack(sequence[i]);
    }
    for (int i = size / 2 - 1; i >= 0; i--)
        SiftDown(i);

}

template<class T>
void Heap<T>::SiftUp(const size_t index) {
    assert(index < size);
    size_t child = index;
    size_t parent = (child - 1) / 2;

    while (child > 0 && heap[parent] > heap[child]) {
        std::swap(heap[parent], heap[child]);

        child = parent;
        parent = (child - 1) / 2;
    }

}

template<class T>
void Heap<T>::SiftDown(size_t index) {
    assert(index < size);
    size_t largest = index;
    size_t left_child = 0;
    size_t right_child = 0;

    for (;;) {
        left_child = largest * 2 + 1;
        right_child = largest * 2 + 2;

        if (left_child < size && heap[largest] > heap[left_child])
            largest = left_child;
        if (right_child < size && heap[largest] > heap[right_child])
            largest = right_child;
        if (largest == index)
            break;

        std::swap(heap[index], heap[largest]);
        index = largest;
    }
}

template<class T>
void Heap<T>::addValue(const T &new_num) {
    heap.pushBack(new_num);
    size++;
    SiftUp(size - 1);
}

template<class T>
void Heap<T>::deleteTop() {
    heap[0] = heap[size - 1];
    heap.popBack();
    size--;
    if (size) SiftDown(0);
}

template<class T>
const T &Heap<T>::getTop() const {
    return heap[0];
}

template<class T>
const size_t Heap<T>::getSize() const {
    return size;
}

// ------------------------------------ MAIN ---------------------------------------------------------------------------

size_t createSize();

void inputInSequence(Vector<int> &sequence, size_t size);

int calcMinSum(Vector<int> &sequence);

void outputResult(int result);

int main() {
    size_t size = createSize();
    Vector<int> numbersForSum(size);
    inputInSequence(numbersForSum, size);

    int result = calcMinSum(numbersForSum);

    outputResult(result);
    return 0;
}

size_t createSize() {
    size_t size = 0;
    std::cin >> size;

    return size;
}

void inputInSequence(Vector<int> &sequence, size_t size) {
    int number = 0;
    for (size_t i = 0; i < size; i++) {
        std::cin >> number;
        sequence.pushBack(number);
    }
}

int calcMinSum(Vector<int> &sequence) {
    Heap<int> minOfNumbers(sequence, sequence.getSize());
    int result = 0;
    int firstAddend = 0, secondAddend = 0;

    while (minOfNumbers.getSize() != 1) {
        firstAddend = minOfNumbers.getTop();
        minOfNumbers.deleteTop();
        secondAddend = minOfNumbers.getTop();
        minOfNumbers.deleteTop();

        result += firstAddend + secondAddend;
        minOfNumbers.addValue(firstAddend + secondAddend);

    }

    return result;

}

void outputResult(const int result) {
    std::cout << result << std::endl;
}
