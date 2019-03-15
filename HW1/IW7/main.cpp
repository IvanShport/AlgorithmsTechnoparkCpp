#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>

#define ALPHABET_COUNT 256

// --------------------------------- String
// ----------------------------------------------------------------------------
class String {
public:
  String();
  String(const String &copied);
  ~String();

  String &operator=(const String &newString);

  size_t get_size() const;
  char *getCString() const;
  void push_back(char symbol);

  char operator[](size_t index) const;

  friend std::ostream &operator<<(std::ostream &os, const String &str);
  friend std::istream &operator>>(std::istream &is, String &str);

private:
  char *string;

  size_t max_size;
  size_t size;

  void realloc_for_string();
};

String::String() : max_size(8), size(0) {
  string = new char[max_size];
  memset(string, 0, max_size);
}

String::String(const String &copied)
    : max_size(copied.max_size), size(copied.size) {
  string = new char[max_size];
  memcpy(string, copied.string, sizeof(char) * (size));
}

String::~String() { delete[] string; }

String &String::operator=(const String &newString) {
  size = newString.size;
  max_size = newString.max_size;
  delete[] string;
  string = new char[max_size];
  memcpy(string, newString.string, size);

  return *this;
}

size_t String::get_size() const { return string != nullptr ? size : 0; }

char *String::getCString() const { return string; }

void String::push_back(char symbol) {
  if (size == max_size) {
    realloc_for_string();
  }
  string[size++] = symbol;
}

void String::realloc_for_string() {
  char *temp = new char[max_size * 2];
  memcpy(temp, string, size * sizeof(char));
  delete[] string;
  string = temp;
  max_size *= 2;
}

char String::operator[](size_t index) const { return string[index]; }

std::ostream &operator<<(std::ostream &os, const String &str) {
  if (str.string)
    for (size_t i = 0; str.string[i] != '\0'; i++)
      std::cout << str.string[i];
  return os;
}

std::istream &operator>>(std::istream &is, String &str) {
  ptrdiff_t cur_size = 1;
  size_t empty_size = str.max_size;

  char *end_string = str.string;

  size_t read_len = 0;
  while (fgets(end_string, empty_size, stdin)) {
    read_len = strlen(end_string);
    str.size += read_len;
    if (end_string[read_len - 1] == '\n') {
      end_string[read_len - 1] = '\0';
      str.size--;
      break;
    }

    empty_size = str.max_size;
    str.max_size *= 2;
    char *temp_string = new char[str.max_size];
    memcpy(temp_string, str.string, empty_size * sizeof(char));
    delete[] str.string;
    str.string = temp_string;
    end_string = str.string + (empty_size - cur_size);
    cur_size++;
  }

  return is;
}

// --------------------------------- VECTOR
// ----------------------------------------------------------------------------

template <class T> class Vector {
public:
  Vector(size_t size = 8);

  Vector(const Vector<T> &copied);

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

template <class T> Vector<T>::Vector(size_t size) : maxSize(size), size(0) {
  buffer = new T[maxSize];
}

template <class T>
Vector<T>::Vector(const Vector<T> &copied)
    : size(copied.size), maxSize(copied.maxSize) {
  buffer = new T[maxSize];

  for (size_t i = 0; i < size; i++) {
    buffer[i] = copied.buffer[i];
  }
}

template <class T> Vector<T>::~Vector() { delete[] buffer; }

template <class T> size_t Vector<T>::getSize() const {
  return buffer != nullptr ? size : 0;
}

template <class T> void Vector<T>::pushBack(const T &ath) {
  if (size == maxSize) {
    reallocForBuffer();
  }
  buffer[size++] = ath;
}

template <class T> void Vector<T>::popBack() {
  if (size == 0)
    return;
  size--;
}

template <class T> const T &Vector<T>::operator[](size_t index) const {
  return buffer[index];
}

template <class T> T &Vector<T>::operator[](size_t index) {
  return buffer[index];
}

template <class T> void Vector<T>::reallocForBuffer() {
  T *temp = new T[maxSize * 2];
  //    memcpy(temp, buffer, maxSize * sizeof(T));
  for (size_t i = 0; i < size; i++) {
    temp[i] = buffer[i];
  }
  delete[] buffer;
  buffer = temp;
  maxSize *= 2;
}

// --------------------------------------- MAIN
// ------------------------------------------------------------------------

Vector<String> createSequenceOfString();

void outputResult(const Vector<String> &result);

void MSDSort(Vector<String> &sequence, size_t left, size_t right, int rank);

void countingSort(Vector<String> &sequence, size_t left, size_t right, int rank,
                  Vector<int> &alphabet);

int main() {
  Vector<String> sequenceOfStrings = createSequenceOfString();
  MSDSort(sequenceOfStrings, 0, sequenceOfStrings.getSize(), 0);
  outputResult(sequenceOfStrings);
  return 0;
}

Vector<String> createSequenceOfString() {
  Vector<String> sequenceOfStrings;

  for (;;) {
    String tempString;
    std::cin >> tempString;
    tempString.push_back('\0');
    if (*tempString.getCString() == std::cin.eof())
      break;
    sequenceOfStrings.pushBack(tempString);
  }

  return sequenceOfStrings;
}

void MSDSort(Vector<String> &sequence, size_t left, size_t right, int rank) {
  if (right - left > 1) {

    Vector<int> alphabet(ALPHABET_COUNT);

    countingSort(sequence, left, right, rank, alphabet);

    for (size_t i = 0; i < ALPHABET_COUNT - 1; i++) {
      if (alphabet[i + 1] - alphabet[i] > 1 &&
          sequence[left + alphabet[i]][rank] != '\0') {
        MSDSort(sequence, left + alphabet[i], left + alphabet[i + 1], rank + 1);
      }
    }
  }
}

void countingSort(Vector<String> &sequence, size_t left, size_t right, int rank,
                  Vector<int> &alphabet) {
  for (int i = 0; i < ALPHABET_COUNT; i++)
    alphabet[i] = 0;

  for (size_t i = left; i < right; i++) {
    alphabet[(int)sequence[i][rank]]++;
  }

  for (size_t i = 1; i < ALPHABET_COUNT; i++) {
    alphabet[i] += alphabet[i - 1];
  }

  Vector<String> sortPocket(right - left);
  for (size_t i = left; i < right; i++) {
    alphabet[(int)sequence[i][rank]]--;
    sortPocket[alphabet[(int)sequence[i][rank]]] = sequence[i];
  }

  for (size_t i = left; i < right; i++) {
    sequence[i] = sortPocket[i - left];
  }
}

void outputResult(const Vector<String> &result) {
  for (size_t i = 0; i < result.getSize(); i++) {
    std::cout << result[i] << '\n';
  }
}
