#include "SafeVector.hpp"

template <typename T>
inline SafeVector<T>::SafeVector(T *data, bool strict, size_t size) noexcept
  : data(data), strict(strict), size(size), capacity(size)
{
  reserve(size);
  if (data != nullptr) {
    copy(data, data + size, this->data);
  }
  else {
    fill_n(this->data, size, T{});
  }

}

template <typename T>
SafeVector<T>::~SafeVector() noexcept
{
  delete[] data;
}

template <typename T>
SafeVector<T>::SafeVector(const SafeVector<T> &other) noexcept
  : strict(other.strict), size(other.size), capacity(other.capacity)
{
  this->data = new T[other.capacity];
  copy(other.data, other.data + other.size, this->data);
}

template <typename T>
SafeVector<T>::SafeVector(SafeVector<T> &&other) noexcept
  : strict(other.strict), size(other.size), capacity(other.capacity), data(other.data)
{
  reset(other);
}

template <typename T>
SafeVector<T> &SafeVector<T>::operator=(const SafeVector<T> &other) noexcept
{
  if (this != &other) {
    delete[] this->data;
    this->data = new T[other.capacity];
    copy(other.data, other.data + other.size, this->data);

    this->size = other.size;
    this->capacity = other.capacity;
    this->strict = other.strict;
  }

  return *this;
}

template <typename T>
SafeVector<T> &SafeVector<T>::operator=(SafeVector<T> &&other) noexcept
{
  if (this != &other) {
    delete[] this->data;

    this->data = other.data;
    this->size = other.size;
    this->capacity = other.capacity;
    this->strict = other.strict;

    reset(other);
  }

  return *this;
}

template <typename T>
T& SafeVector<T>::operator[](size_t index)
{
  if (!data || index >= size) {
    throw out_of_range("Accessing out of range index.");
  } 
  return this->data[index];
}

template <typename T>
bool SafeVector<T>::add(const T &value) noexcept
{
  if (this->size < this->capacity) {
    this->data[size++] = value;
    return true;
  }

  if (!this->strict && this->capacity < MAX_CAPACITY) {
    reserve(this->capacity == 0 ? 1 : this->capacity * 2);
    this->data[size++] = value;
    return true;
  }

  return false;
}

template <typename T>
bool SafeVector<T>::remove(const size_t index, int length) noexcept
{
  if (index > this->size || index + length > this->size) {
    return false;
  }

  copy(&this->data[index + length], &this->data[this->size], &this->data[index]);

  this->size -= length;

  return true;
}

template <typename T>
bool SafeVector<T>::replace(const size_t index, const T &value) noexcept
{
  if (index < size) {
    this->data[index] = value;
    return true;
  }
  return false;
}

template <typename T>
size_t SafeVector<T>::shrink_to_fit() noexcept
{
  if (this->size == this->capacity || this->strict) return 0;

  int old_capacity = this->capacity;

  T *newData = new T[this->size];
  copy(this->data, this->data + this->size, newData);

  delete[] this->data;
  this->data = newData;
  this->capacity = this->size;

  return old_capacity - this->size;
}
