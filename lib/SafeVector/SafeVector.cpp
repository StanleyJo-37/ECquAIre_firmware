#include "SafeVector.h"

template <typename T>
inline SafeVector<T>::SafeVector(size_t size, T *data, bool strict) noexcept
  : size(size), data(data), strict(strict), capacity(size)
{
  reserve(size);
}

template <typename T>
SafeVector<T>::~SafeVector() noexcept
{
  delete[] data;
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
  if (index > this->capacity) {
    return false;
  }

  copy(&this->data[index + length], &this->data[this->size], &this->data[index]);

  this->size -= 1;

  return true;
}

template <typename T>
bool SafeVector<T>::replace(const size_t index, const T &value) noexcept
{
  if (index < capacity) {
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

  T *newData = new T[this->size]();
  copy(this->data, this->data + this->size, newData);

  delete[] this->data;
  this->data = newData;
  this->capacity = this->size;

  return old_capacity - this->size;
}
