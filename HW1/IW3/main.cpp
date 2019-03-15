#include <iostream>
#include <cstring>
#include <assert.h>

#define MAX_SIZE 1000000

class Deque {
public:
    Deque();

    ~Deque();

    int popFront();

    int popBack();

    void pushFront(int new_value);

    void pushBack(int new_value);

private:
    int *buffer;
    size_t head;
    size_t tail;
    size_t maxSize;

    void reallocForBuffer();

    size_t nextPos(size_t pos);

    size_t prevPos(size_t pos);
};

Deque::Deque() :
        head(0),
        tail(1),
        maxSize(8) {
    buffer = new int[maxSize];
}

Deque::~Deque() {
    delete[] buffer;
}

int Deque::popFront() {
    if (nextPos(head) == tail)
        return -1;

    head = nextPos(head);
    return buffer[head];
}

int Deque::popBack() {
    if (nextPos(head) == tail)
        return -1;

    tail = prevPos(tail);
    return buffer[tail];
}

void Deque::pushFront(int new_value) {
    if (tail == head) {
        reallocForBuffer();
    }
    buffer[head] = new_value;
    head = prevPos(head);
}

void Deque::pushBack(int new_value) {
    if (tail == head) {
        reallocForBuffer();
    }
    buffer[tail] = new_value;
    tail = nextPos(tail);
}

void Deque::reallocForBuffer() {
    int *newBuffer = new int[maxSize * 2];
    memcpy(newBuffer, buffer, tail * sizeof(int));
    memcpy(&newBuffer[maxSize + head + 1], &buffer[head + 1], (maxSize - head - 1) * sizeof(int));
    delete[] buffer;
    buffer = newBuffer;
    head += maxSize;
    maxSize *= 2;
}

size_t Deque::nextPos(size_t pos) {
    return (pos + 1) % maxSize;
}

size_t Deque::prevPos(size_t pos) {
    return (maxSize + pos - 1) % maxSize;
}

bool workWithDeque(Deque &queue);

int main() {
    Deque queue;

    bool isCorrect = workWithDeque(queue);

    std::cout << (isCorrect ? "YES" : "NO");

    return 0;
}

bool workWithDeque(Deque &queue) {
    size_t countOfCommand = 0;
    std::cin >> countOfCommand;
    assert(countOfCommand <= MAX_SIZE);

    int numberOfCommand = 0;
    unsigned int value = 0;
    bool isCorrect = true;
    for (size_t i = 0; i < countOfCommand; i++) {
        std::cin >> numberOfCommand;
        std::cin >> value;

        switch (numberOfCommand) {
            case 1:
                queue.pushFront(value);
                break;
            case 2:
                if (queue.popFront() != value)
                    isCorrect = false;
                break;
            case 3:
                queue.pushBack(value);
                break;
            case 4:
                if (queue.popBack() != value)
                    isCorrect = false;
                break;
            default:
                isCorrect = false;
                break;
        }

    }

    return isCorrect;
}