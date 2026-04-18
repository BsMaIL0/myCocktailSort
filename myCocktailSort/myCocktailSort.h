#ifndef MY_COCKTAIL_SORT_H
#define MY_COCKTAIL_SORT_H

#include <utility>

template <typename T, typename Compare = std::less<T>>
void myCocktailSort(T* array, size_t size, Compare comp = Compare()) {
  if (size <= 1) {
    return;
  }

  size_t left = 0;
  size_t right = size - 1;
  bool swapped = true;

  while (left < right && swapped) {
    swapped = false;

    for (size_t i = left; i < right; ++i) {
      if (comp(array[i + 1], array[i])) {
        std::swap(array[i + 1], array[i]);
        swapped = true;
      }
    }
    --right;

    if (!swapped) break;

    swapped = false;

    for (size_t i = right; i > left; --i) {
      if (comp(array[i], array[i - 1])) {
        std::swap(array[i], array[i - 1]);
        swapped = true;
      }
    }
    ++left;
  }
}

#endif  // !MY_COCKTAIL_SORT_H
