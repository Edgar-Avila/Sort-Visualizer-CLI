#pragma once

#include "sort.hpp"
#include <algorithm>
#include <vector>

class BubbleSort : public Sort {
public:
  BubbleSort(std::vector<int> &arr) : i(0), j(0), Sort(arr) {}

  void step() override {
    if (arr[j] > arr[j + 1]) {
      std::swap(arr[j], arr[j + 1]);
    }
    j++;
    if (j >= arr.size() - i - 1) {
      j = 0;
      i++;
    }
  }

  int current() override { return j; }

  bool done() override { return i >= arr.size() - 1; }

  bool sorted(int idx) override { return (idx > arr.size() - i - 1) || done(); }

  void reset() override {
    i = 0;
    j = 0;
  }

private:
  int i, j;
};
