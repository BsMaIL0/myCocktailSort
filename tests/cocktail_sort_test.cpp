#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "../myCocktailSort/myCocktailSort.h"

// ==================== HELPER FUNCTIONS ====================

template <typename T>
bool isSorted(const T* array, size_t size) {
  if (size == 0 || size == 1) return true;
  for (size_t i = 0; i < size - 1; ++i) {
    if (array[i] > array[i + 1]) return false;
  }
  return true;
}

template <typename T>
void printArray(const T* array, size_t size, const std::string& title = "") {
  if (!title.empty()) {
    std::cout << title << ": ";
  }
  for (size_t i = 0; i < size; ++i) {
    std::cout << array[i] << " ";
  }
  std::cout << std::endl;
}

// Array generation
enum class ArrayType {
  SORTED,
  REVERSE,
  RANDOM,
  NEARLY_SORTED,
  MANY_DUPLICATES
};

std::vector<int> generateArray(int size, ArrayType type) {
  std::vector<int> arr(size);
  std::random_device rd;
  std::mt19937 gen(rd());

  switch (type) {
    case ArrayType::SORTED:
      for (int i = 0; i < size; ++i) arr[i] = i;
      break;

    case ArrayType::REVERSE:
      for (int i = 0; i < size; ++i) arr[i] = size - i;
      break;

    case ArrayType::RANDOM: {
      std::uniform_int_distribution<> dis(1, size * 10);
      for (int i = 0; i < size; ++i) arr[i] = dis(gen);
    } break;

    case ArrayType::NEARLY_SORTED:
      for (int i = 0; i < size; ++i) arr[i] = i;
      {
        std::uniform_int_distribution<> dis(0, size - 1);
        int swaps = size / 10;
        for (int s = 0; s < swaps; ++s) {
          int i = dis(gen);
          int j = dis(gen);
          std::swap(arr[i], arr[j]);
        }
      }
      break;

    case ArrayType::MANY_DUPLICATES: {
      std::uniform_int_distribution<> dis(1, std::max(1, size / 10));
      for (int i = 0; i < size; ++i) arr[i] = dis(gen);
    } break;
  }
  return arr;
}

// Time measurement
template <typename T>
double measureTime(void (*sortFunc)(T*, size_t, std::less<T>),
                   std::vector<T> arr) {
  auto start = std::chrono::high_resolution_clock::now();
  sortFunc(arr.data(), arr.size(), std::less<T>());
  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  return duration.count() / 1000.0;
}

// ==================== CORRECTNESS TESTS ====================

// 1. Empty array test
TEST(CocktailSortTest, EmptyArray) {
  myCocktailSort<int>(nullptr, 0);
  SUCCEED();
}

// 2. Single element test
TEST(CocktailSortTest, SingleElement) {
  int arr[] = {42};
  myCocktailSort(arr, 1);
  EXPECT_EQ(arr[0], 42);
}

// 3. Two elements sorted test
TEST(CocktailSortTest, TwoElementsSorted) {
  int arr[] = {1, 2};
  int expected[] = {1, 2};
  myCocktailSort(arr, 2);
  EXPECT_EQ(arr[0], expected[0]);
  EXPECT_EQ(arr[1], expected[1]);
}

// 4. Two elements unsorted test
TEST(CocktailSortTest, TwoElementsUnsorted) {
  int arr[] = {2, 1};
  int expected[] = {1, 2};
  myCocktailSort(arr, 2);
  EXPECT_EQ(arr[0], expected[0]);
  EXPECT_EQ(arr[1], expected[1]);
}

// 5. Already sorted array test
TEST(CocktailSortTest, AlreadySorted) {
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  myCocktailSort(arr, 10);
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(arr[i], expected[i]);
  }
}

// 6. Reverse sorted array test
TEST(CocktailSortTest, ReverseSorted) {
  int arr[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  int expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  myCocktailSort(arr, 10);
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(arr[i], expected[i]);
  }
}

// 7. Duplicate elements test
TEST(CocktailSortTest, DuplicateElements) {
  int arr[] = {5, 2, 8, 2, 9, 1, 5, 5, 3};
  int expected[] = {1, 2, 2, 3, 5, 5, 5, 8, 9};
  myCocktailSort(arr, 9);
  for (int i = 0; i < 9; ++i) {
    EXPECT_EQ(arr[i], expected[i]);
  }
}

// 8. Negative numbers test
TEST(CocktailSortTest, NegativeNumbers) {
  int arr[] = {-5, 3, -10, 0, 7, -2, 1};
  int expected[] = {-10, -5, -2, 0, 1, 3, 7};
  myCocktailSort(arr, 7);
  for (int i = 0; i < 7; ++i) {
    EXPECT_EQ(arr[i], expected[i]);
  }
}

// 9. All same elements test
TEST(CocktailSortTest, AllSameElements) {
  int arr[] = {7, 7, 7, 7, 7, 7};
  int expected[] = {7, 7, 7, 7, 7, 7};
  myCocktailSort(arr, 6);
  for (int i = 0; i < 6; ++i) {
    EXPECT_EQ(arr[i], expected[i]);
  }
}

// 10. Even size test
TEST(CocktailSortTest, EvenSize) {
  int arr[] = {4, 3, 2, 1};
  int expected[] = {1, 2, 3, 4};
  myCocktailSort(arr, 4);
  for (int i = 0; i < 4; ++i) {
    EXPECT_EQ(arr[i], expected[i]);
  }
}

// 11. Odd size test
TEST(CocktailSortTest, OddSize) {
  int arr[] = {5, 3, 4, 1, 2};
  int expected[] = {1, 2, 3, 4, 5};
  myCocktailSort(arr, 5);
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(arr[i], expected[i]);
  }
}

// 12. Camels in tail test
TEST(CocktailSortTest, CamelsInTail) {
  int arr[] = {2, 3, 4, 5, 6, 7, 8, 9, 1};
  int expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  myCocktailSort(arr, 9);
  for (int i = 0; i < 9; ++i) {
    EXPECT_EQ(arr[i], expected[i]);
  }
}

// 13. Large numbers test
TEST(CocktailSortTest, LargeNumbers) {
  int arr[] = {1000000, 500, 999999, 1, 10000000};
  int expected[] = {1, 500, 999999, 1000000, 10000000};
  myCocktailSort(arr, 5);
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(arr[i], expected[i]);
  }
}

// 14. Sorted property test
TEST(CocktailSortTest, SortedProperty) {
  int arr[] = {9, 7, 5, 3, 1, 8, 6, 4, 2, 0};
  myCocktailSort(arr, 10);
  EXPECT_TRUE(isSorted(arr, 10));
}

// ==================== TESTS WITH DIFFERENT DATA TYPES ====================

// 15. Double array test
TEST(CocktailSortTest, DoubleArray) {
  double arr[] = {3.14, 1.41, 2.71, 1.0, 2.5};
  double expected[] = {1.0, 1.41, 2.5, 2.71, 3.14};
  myCocktailSort(arr, 5);
  for (int i = 0; i < 5; ++i) {
    EXPECT_DOUBLE_EQ(arr[i], expected[i]);
  }
}

// 16. String array test
TEST(CocktailSortTest, StringArray) {
  std::string arr[] = {"banana", "apple", "cherry", "date"};
  std::string expected[] = {"apple", "banana", "cherry", "date"};
  myCocktailSort(arr, 4);
  for (int i = 0; i < 4; ++i) {
    EXPECT_EQ(arr[i], expected[i]);
  }
}

// 17. Custom comparator test (descending order)
TEST(CocktailSortTest, CustomComparator) {
  int arr[] = {1, 2, 3, 4, 5};
  int expected[] = {5, 4, 3, 2, 1};
  // Sort in descending order
  myCocktailSort(arr, 5, [](int a, int b) { return a > b; });
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(arr[i], expected[i]);
  }
}

// ==================== PARAMETERIZED TESTS ====================

class CocktailSortSizeTest : public ::testing::TestWithParam<int> {};

TEST_P(CocktailSortSizeTest, DifferentSizes) {
  int size = GetParam();
  if (size == 0) {
    // For size 0 just check that function doesn't crash
    myCocktailSort<int>(nullptr, 0);
    SUCCEED();
    return;
  }

  std::vector<int> arr(size);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 100);

  for (int i = 0; i < size; ++i) {
    arr[i] = dis(gen);
  }

  std::vector<int> expected = arr;
  std::sort(expected.begin(), expected.end());

  myCocktailSort(arr.data(), size);

  for (int i = 0; i < size; ++i) {
    EXPECT_EQ(arr[i], expected[i]);
  }
}

INSTANTIATE_TEST_SUITE_P(Sizes, CocktailSortSizeTest,
                         ::testing::Values(0, 1, 2, 3, 5, 10, 20, 50, 100,
                                           200));

// ==================== SPEED TESTS ====================

class SpeedTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::cout << "\nCOCKTAIL SORT SPEED MEASUREMENTS" << std::endl;
  }
};

TEST_F(SpeedTest, RandomArrays) {
  std::vector<int> sizes = {100, 500, 1000, 2000, 5000, 10000};

  std::cout << "\nRANDOM ARRAYS" << std::endl;
  std::cout << std::string(50, '-') << std::endl;
  std::cout << std::left << std::setw(20) << "Array size" << std::setw(20)
            << "Time (ms)" << std::setw(15) << "Comparisons" << std::endl;
  std::cout << std::string(50, '-') << std::endl;

  for (int size : sizes) {
    auto arr = generateArray(size, ArrayType::RANDOM);
    double timeMs = measureTime<int>(myCocktailSort, arr);

    std::cout << std::left << std::setw(20) << size << std::setw(20)
              << std::fixed << std::setprecision(3) << timeMs << std::setw(15)
              << (size * size) << std::endl;
  }
}

TEST_F(SpeedTest, BestVsWorstCase) {
  std::vector<int> sizes = {1000, 2000, 5000, 10000};

  std::cout << "\nBEST CASE VS WORST CASE COMPARISON" << std::endl;
  std::cout << std::string(70, '-') << std::endl;
  std::cout << std::left << std::setw(20) << "Size" << std::setw(20)
            << "Best case (ms)" << std::setw(20) << "Worst case (ms)"
            << std::setw(15) << "Ratio" << std::endl;
  std::cout << std::string(70, '-') << std::endl;

  for (int size : sizes) {
    auto bestArr = generateArray(size, ArrayType::SORTED);
    auto worstArr = generateArray(size, ArrayType::REVERSE);

    double bestTime = measureTime<int>(myCocktailSort, bestArr);
    double worstTime = measureTime<int>(myCocktailSort, worstArr);
    double ratio = worstTime / bestTime;

    std::cout << std::left << std::setw(20) << size << std::setw(20)
              << std::fixed << std::setprecision(3) << bestTime << std::setw(20)
              << worstTime << std::setw(15) << std::setprecision(1) << ratio
              << "x" << std::endl;
  }
}

// ==================== MAIN FUNCTION ====================

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}