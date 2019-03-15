#include <iostream>
#include <random>

size_t inputSizeOrK();
int *createAndInputSequence(size_t size);
bool comparator(const int &left, const int &right);
template <class T> void getPivot(T *sequence, size_t left, size_t right);
template <class T>
size_t partition(T *sequence, int left, int right,
                 bool (*cmp)(const T &, const T &));
template <class T>
int findOrdinalStatistics(T *sequence, int left, int right, size_t k,
                          bool (*cmp)(const T &, const T &));
void outputResult(int result);

int main() {
  size_t size = inputSizeOrK();
  size_t k = inputSizeOrK();
  int *sequence = createAndInputSequence(size);

  int ordinal_statistics =
      findOrdinalStatistics(sequence, 0, size - 1, k, comparator);

  outputResult(ordinal_statistics);

  return 0;
}

size_t inputSizeOrK() {
  size_t value = 0;
  std::cin >> value;

  return value;
}

int *createAndInputSequence(size_t size) {
  auto sequence = new int[size];

  for (size_t i = 0; i < size; i++) {
    std::cin >> sequence[i];
  }

  return sequence;
}

bool comparator(const int &left, const int &right) { return left > right; }

template <class T>
int findOrdinalStatistics(T *sequence, int left, int right, size_t k,
                          bool (*cmp)(const T &, const T &)) {
  size_t pivot = 0;
  while (left != right) {
    pivot = partition(sequence, left, right, cmp);
    if (pivot == k)
      return sequence[pivot];
    if (k < pivot) {
      right = pivot - 1;
    } else {
      left = pivot + 1;
    }
  }
  return sequence[left];
}

template <class T>
size_t partition(T *sequence, int left, int right,
                 bool (*cmp)(const T &, const T &)) {
  getPivot(sequence, left, right);
  int i = left, j = left;
  while (true) {
    for (; j < right && cmp(sequence[j], sequence[right]); j++) {
    }
    if (j >= right)
      break;
    std::swap(sequence[i], sequence[j]);
    i++;
    j++;
  }
  std::swap(sequence[i], sequence[right]);
  return i;
}

template <class T> void getPivot(T *sequence, size_t left, size_t right) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(left, right);

  std::swap(sequence[dist(gen)], sequence[right]);
}

void outputResult(int result) { std::cout << result; }
