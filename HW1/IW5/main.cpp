#include <cstring>
#include <iostream>

struct Client {
  Client();

  int begin;
  int end;
  int advCounter;
};

Client::Client() : begin(0), end(0), advCounter(0) {}

size_t createSize();
Client *createAndInputClients(size_t size);
int findMinAdv(Client *allClient, size_t size,
               bool (*cmp)(const Client &, const Client &));
bool comparator(const Client &left, const Client &right);
template <class T>
void mergeSort(T *sequence, size_t size, bool (*cmp)(const T &, const T &));
template <class T>
void merge(const T *leftSequence, size_t left_size, const T *rightSequence,
           size_t right_size, T *tempSequence,
           bool (*cmp)(const T &, const T &));
void outputResult(int result);

int main() {
  size_t size = createSize();
  Client *allClients = createAndInputClients(size);

  int min = findMinAdv(allClients, size, comparator);

  outputResult(min);
  return 0;
}

size_t createSize() {
  size_t size = 0;
  std::cin >> size;

  return size;
}

Client *createAndInputClients(size_t size) {
  auto array = new Client[size];

  int begin = 0, end = 0;
  for (size_t i = 0; i < size; i++) {
    std::cin >> begin >> end;
    array[i].begin = begin;
    array[i].end = end;
  }

  return array;
}

int findMinAdv(Client *allClient, size_t size,
               bool (*cmp)(const Client &, const Client &)) {
  mergeSort(allClient, size, cmp);
  int minAdv = 0;
  int endOfFrontSegment = -1;
  int beginOfFrontSegment = -1;
  for (size_t i = 0; i < size; i++) {
    if (allClient[i].begin > endOfFrontSegment) {
      endOfFrontSegment = allClient[i].end;
      beginOfFrontSegment = endOfFrontSegment - 1;
      minAdv += 2;
    } else if (allClient[i].begin == endOfFrontSegment ||
               ((allClient[i].begin > beginOfFrontSegment) &&
                (allClient[i].end > endOfFrontSegment))) {
      beginOfFrontSegment = endOfFrontSegment;
      endOfFrontSegment = allClient[i].end;
      minAdv++;
    }
  }

  return minAdv;
}

bool comparator(const Client &left, const Client &right) { // <
  if (left.end < right.end)
    return true;
  if (left.end == right.end)
    return left.begin > right.begin;
  return false;
}

template <class T>
void mergeSort(T *sequence, size_t size, bool (*cmp)(const T &, const T &)) {
  if (size <= 1)
    return;
  size_t leftSize = size / 2;
  size_t rightSize = size - leftSize;

  mergeSort(sequence, leftSize, cmp);
  mergeSort(sequence + leftSize, rightSize, cmp);

  T *tempSequence = new T[size];
  merge(sequence, leftSize, sequence + leftSize, rightSize, tempSequence, cmp);
  for (size_t i = 0; i < size; i++) {
    sequence[i] = tempSequence[i];
  }
  delete[] tempSequence;
}

template <class T>
void merge(const T *leftSequence, size_t left_size, const T *rightSequence,
           size_t right_size, T *tempSequence,
           bool (*cmp)(const T &, const T &)) {
  size_t i = 0;
  size_t j = 0;
  for (; i < left_size && j < right_size;) {
    if (cmp(rightSequence[j], leftSequence[i])) {
      tempSequence[i + j] = rightSequence[j];
      j++;
    } else {
      tempSequence[i + j] = leftSequence[i];
      i++;
    }
  }
  if (i == left_size) {
    for (; j < right_size; j++)
      tempSequence[i + j] = rightSequence[j];
  }
  if (j == right_size) {
    for (; i < left_size; i++)
      tempSequence[i + j] = leftSequence[i];
  }
}

void outputResult(const int result) { std::cout << result << std::endl; }
