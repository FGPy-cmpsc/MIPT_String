#pragma once
#include <cassert>
#include <cstddef>
#include <ostream>
#include <vector>

class String {
 public:
  String() = default;

  String(size_t size, char character);

  String(const char* char_sequence);

  String(const String& string);

  ~String();

  String& operator=(const String& string);

  void Clear();

  void PushBack(char character);

  void PopBack();

  void Resize(size_t size);

  void Resize(size_t size, char character);

  void Reserve(size_t new_capacity);

  void ShrinkToFit();

  void Swap(String& other);

  template <std::integral T>
  char& operator[](T index) {
    return string_[index];
  }

  template <std::integral T>
  const char& operator[](T index) const {
    return string_[index];
  }

  const char& Front() const;

  char& Front();

  const char& Back() const;

  char& Back();

  bool Empty() const;

  size_t Size() const;

  size_t Capacity() const;

  const char* Data() const;
  char* Data();

  String& operator+=(const String& added_string);

  template <std::integral T>
  String& operator*=(T count) {
    if (count == 0) {
      *this = String();
      return *this;
    }
    String new_string(*this);
    if (count % 2 == 0) {
      *this += new_string;
      *this *= (count / 2);
    } else {
      *this *= count - 1;
      *this += new_string;
    }
    return *this;
  }

  std::vector<String> Split(const String& delim = " ");

  String Join(const std::vector<String>& strings) const;

 private:
  static size_t GetMinCapacity(size_t size);

  static size_t GetMaxSize(size_t capacity);

  static void InsertNullTerminator(char* char_sequence, size_t size);

  bool IsStringFull() const;

  bool FitInCapacity(size_t size) const;

  size_t GetNewCapacity(size_t old_capacity) const;

  static char* MemoryAllocation(size_t old_capacity);

  void ShrinkToCapacity(size_t new_capacity);

  const int kStringFactor = 2;
  char* string_ = nullptr;
  size_t size_ = 0;
  size_t capacity_ = 0;
};

std::ostream& operator<<(std::ostream& os, const String& string);

std::istream& operator>>(std::istream& is, String& string);

String operator+(const String& first_string, const String& second_string);

bool operator>(const String& first_string, const String& second_string);

bool operator<(const String& first_string, const String& second_string);

bool operator==(const String& first_string, const String& second_string);

bool operator>=(const String& first_string, const String& second_string);

bool operator<=(const String& first_string, const String& second_string);

bool operator!=(const String& first_string, const String& second_string);

template <std::integral T>
String operator*(String string, T count) {
  assert(count >= 0);
  String new_string = string;
  new_string *= count;
  return new_string;
}
