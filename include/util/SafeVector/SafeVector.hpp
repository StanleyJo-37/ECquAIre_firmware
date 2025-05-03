#pragma once

#include <cstddef>
#include <algorithm>
#include <stdexcept>

using std::copy;
using std::fill_n;
using std::move;
using std::out_of_range;

constexpr bool DEFAULT_STRICT = true;
constexpr size_t DEFAULT_SIZE = 10;
constexpr size_t MAX_CAPACITY = 200;

template<typename T>
class SafeVector {
  public:
    SafeVector(T *data = nullptr, bool strict = DEFAULT_STRICT, size_t size = DEFAULT_SIZE) noexcept;
    ~SafeVector() noexcept;

    SafeVector(const SafeVector<T> &other) noexcept;
    SafeVector<T> &operator=(const SafeVector<T> &other) noexcept;
    SafeVector(SafeVector &&other) noexcept;
    SafeVector<T> &operator=(SafeVector<T> &&other) noexcept;

    T& operator[](size_t index);

    bool add(const T& value) noexcept;
    bool remove(const size_t index, int length = 1) noexcept;
    bool replace(const size_t index, const T &value) noexcept;

    size_t shrink_to_fit() noexcept;

    static void reset(SafeVector &vec) noexcept {
      vec.data = nullptr;
      vec.size = 0;
      vec.capacity = 0;
      vec.strict = false;
    };

    constexpr bool is_strict() const noexcept { return this->strict; };
    T *get_array() const noexcept { return this->data; };
    size_t get_size() const noexcept { return this->size; };
    size_t get_capacity() const noexcept { return this->capacity; };
  private:
    void reserve(size_t newSize) noexcept {
      if (newSize <= size || newSize > MAX_CAPACITY) {
        return;
      }

      T *newData = new T[newSize];
      fill_n(newData, newSize, T{});
      
      copy(data, data + size, newData);

      delete[] this->data;
      this->data = newData;
      this->capacity = newSize;
    };

    T *data = nullptr;
    bool strict = false;
    size_t size;
    size_t capacity;
};

#include "SafeVector.tpp"