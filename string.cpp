#include "string.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <ostream>

size_t String::GetMinCapacity(size_t size) { return size + 1; }

size_t String::GetMaxSize(size_t capacity) { return capacity - 1; }

void String::InsertNullTerminator(char* char_sequence, size_t size) {
  char_sequence[size] = '\0';
}

bool String::Empty() const { return (size_ == 0); }

String::String(const char* char_sequence) {
  size_t size_of_sequence = std::strlen(char_sequence);
  if (size_of_sequence == 0) {
    size_ = 0;
    capacity_ = 0;
    string_ = nullptr;
    return;
  }
  size_ = size_of_sequence;
  capacity_ = GetMinCapacity(size_of_sequence);
  string_ = MemoryAllocation(capacity_);
  std::strcpy(string_, char_sequence);
  InsertNullTerminator(string_, size_);
}

String::String(size_t size, char character) : size_(size) {
  if (size == 0) {
    capacity_ = 0;
    string_ = nullptr;
    return;
  }
  capacity_ = GetMinCapacity(size);
  string_ = MemoryAllocation(capacity_);
  std::memset(string_, character, size);
  InsertNullTerminator(string_, size_);
}

String::String(const String& string)
    : size_(string.size_), capacity_(string.capacity_) {
  if (string.capacity_ == 0) {
    string_ = nullptr;
    return;
  }
  string_ = MemoryAllocation(capacity_);
  std::strcpy(string_, string.string_);
}

String::~String() {
  if (capacity_ != 0) {
    delete[] string_;
  }
}

String& String::operator=(const String& string) {
  if (this == &string) {
    return *this;
  }
  String new_string(string);
  Swap(new_string);
  return *this;
}

void String::Clear() {
  size_ = 0;
  InsertNullTerminator(string_, size_);
}

std::ostream& operator<<(std::ostream& os, const String& string) {
  for (size_t i = 0; i < string.Size(); ++i) {
    os << string[i];
  }
  return os;
}

void String::Reserve(size_t new_capacity) {
  if (new_capacity <= capacity_) {
    return;
  }
  ShrinkToCapacity(new_capacity);
}

std::istream& operator>>(std::istream& is, String& string) {
  if (string.Capacity() == 0) {
    size_t new_capacity = 2;
    string.Reserve(new_capacity);
  }
  String new_string;
  std::istream::sentry istream_sentry(is);
  if (istream_sentry) {
    char symbol;
    while (is.get(symbol)) {
      if (std::isspace(static_cast<unsigned char>(symbol)) != 0) {
        break;
      }
      new_string.PushBack(symbol);
    }
  }
  string = new_string;
  return is;
}

bool String::IsStringFull() const {
  assert(size_ <= GetMaxSize(capacity_));
  if (capacity_ == 0) {
    return true;
  }
  return size_ == GetMaxSize(capacity_);
}

bool String::FitInCapacity(size_t size) const {
  return GetMinCapacity(size) <= capacity_;
}

size_t String::GetNewCapacity(size_t old_capacity) const {
  size_t new_capacity = kStringFactor * old_capacity;
  return new_capacity;
}

char* String::MemoryAllocation(size_t capacity) {
  char* new_storage = new char[capacity];
  return new_storage;
}

void String::ShrinkToCapacity(size_t new_capacity) {
  char* new_storage = MemoryAllocation(new_capacity);
  if (new_capacity >= capacity_) {
    for (size_t i = 0; i < size_; ++i) {
      new_storage[i] = string_[i];
    }
  } else {
    size_t new_size = std::min(size_, GetMaxSize(new_capacity));
    for (size_t i = 0; i < new_size; ++i) {
      new_storage[i] = string_[i];
    }
    size_ = new_size;
  }
  InsertNullTerminator(new_storage, size_);
  capacity_ = new_capacity;
  delete[] string_;
  string_ = new_storage;
}

void String::Resize(const size_t kSize) {
  if (FitInCapacity(kSize)) {
    size_ = kSize;
    InsertNullTerminator(string_, size_);
    return;
  }
  size_t new_capacity = GetMinCapacity(kSize);
  char* new_storage = MemoryAllocation(new_capacity);
  InsertNullTerminator(new_storage, kSize);
  for (size_t i = 0; i < size_; ++i) {
    new_storage[i] = string_[i];
  }
  size_ = kSize;
  capacity_ = new_capacity;
  delete[] string_;
  string_ = new_storage;
}

void String::Resize(const size_t kSize, const char kCharacter) {
  if (FitInCapacity(kSize)) {
    size_ = kSize;
    InsertNullTerminator(string_, size_);
    return;
  }
  const size_t kOldSize = size_;

  const size_t kNewCapacity = GetMinCapacity(kSize);
  char* new_storage = MemoryAllocation(kNewCapacity);
  size_ = kSize;
  InsertNullTerminator(new_storage, size_);
  capacity_ = kNewCapacity;
  for (size_t i = 0; i < kOldSize; ++i) {
    new_storage[i] = string_[i];
  }
  for (size_t i = kOldSize; i < size_; ++i) {
    new_storage[i] = kCharacter;
  }
  delete[] string_;
  string_ = new_storage;
}

void String::PushBack(const char kCharacter) {
  if (IsStringFull()) {
    if (capacity_ == 0) {
      size_t new_capacity = 2;
      Reserve(new_capacity);
    } else {
      size_t new_capacity = GetNewCapacity(capacity_);
      Reserve(new_capacity);
    }
  }
  ++size_;
  string_[size_ - 1] = kCharacter;
  InsertNullTerminator(string_, size_);
}

void String::PopBack() {
  if (Empty()) {
    return;
  }
  --size_;
}

void String::Swap(String& other) {
  std::swap(string_, other.string_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

void String::ShrinkToFit() {
  assert(FitInCapacity(size_));
  if (GetMinCapacity(size_) <= capacity_) {
    capacity_ = GetMinCapacity(size_);
    ShrinkToCapacity(capacity_);
  }
}

const char* String::Data() const { return string_; }

char* String::Data() { return string_; }

char& String::Front() { return string_[0]; }

const char& String::Front() const { return string_[0]; }

char& String::Back() { return string_[size_ - 1]; }

const char& String::Back() const { return string_[size_ - 1]; }

String& String::operator+=(const String& added_string) {
  Reserve(Size() + added_string.Size());
  for (size_t insert_index = 0; insert_index < added_string.Size();
       ++insert_index) {
    PushBack(added_string[insert_index]);
  }
  return *this;
}

String operator+(const String& first_string, const String& second_string) {
  String new_string = first_string;
  new_string += second_string;
  return new_string;
}

size_t String::Size() const { return size_; }

size_t String::Capacity() const { return capacity_ - 1; }

std::vector<String> String::Split(const String& delim) {
  std::vector<String> strings;
  if (size_ == 0) {
    String string = String();
    return {string};
  }
  String new_word;
  String delim_word;
  for (size_t i = 0; i < size_; ++i) {
    if (string_[i] == delim[delim_word.Size()]) {
      delim_word.PushBack(delim[delim_word.Size()]);
    } else {
      new_word += delim_word;
      new_word.PushBack(string_[i]);
      delim_word = String();
    }
    if (delim_word.Size() == delim.Size()) {
      strings.push_back(new_word);
      new_word = String();
      delim_word = String();
    }
  }
  new_word += delim_word;
  strings.push_back(new_word);
  return strings;
}

String String::Join(const std::vector<String>& strings) const {
  String new_string;
  if (strings.empty()) {
    return new_string;
  }
  for (size_t i = 0; i < strings.size() - 1; ++i) {
    new_string += strings[i];
    new_string += *this;
  }
  new_string += strings.back();
  return new_string;
}

bool operator>(const String& first_string, const String& second_string) {
  size_t min_size = std::min(first_string.Size(), second_string.Size());
  for (size_t i = 0; i < min_size; ++i) {
    if (first_string[i] > second_string[i]) {
      return true;
    }
    if (first_string[i] < second_string[i]) {
      return false;
    }
  }
  return first_string.Size() > second_string.Size();
}

bool operator<(const String& first_string, const String& second_string) {
  return second_string > first_string;
}

bool operator==(const String& first_string, const String& second_string) {
  if (first_string.Size() != second_string.Size()) {
    return false;
  }
  for (size_t i = 0; i < first_string.Size(); ++i) {
    if (first_string[i] != second_string[i]) {
      return false;
    }
  }
  return true;
}

bool operator>=(const String& first_string, const String& second_string) {
  return !(second_string > first_string);
}

bool operator<=(const String& first_string, const String& second_string) {
  return !(first_string > second_string);
}

bool operator!=(const String& first_string, const String& second_string) {
  return !(first_string == second_string);
}
