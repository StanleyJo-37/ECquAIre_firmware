#pragma once

#include <type_traits>
#include <cstddef>

using std::is_arithmetic_v;

inline int max(int a, int b) {
  return a > b ? a : b;
}

inline int min(int a, int b) {
  return a > b ? b : a;
}

template<typename T, size_t size>
inline int min_elem(T *arr) {
  static_assert(is_arithmetic_v<T>, "T must be an arithmetic type.");

  if (size == 0) {
    return -1;
  }

  int min_idx = 0;
  for (int i = 0; i < size; ++i) {
    if (arr[i] < arr[min_idx]) {
      min_idx = i;
    }
  }

  return min_idx;
}

template<typename T, size_t size>
inline int max_elem(T *arr) {
  static_assert(is_arithmetic_v<T>, "T must be an arithmetic type.");

  if (size == 0) {
    return -1;
  }

  int max_idx = 0;
  for (int i = 0; i < size; ++i) {
    if (arr[i] > arr[max_idx]) {
      max_idx = i;
    }
  }

  return max_idx;
}