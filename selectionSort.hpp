#pragma once

#include "sort.hpp"
#include <algorithm>
#include <vector>

class SelectionSort : public Sort {
public:
  SelectionSort(std::vector<int> &arr) : i(0), j(0), min(0), Sort(arr) {}

  void step() override {
    if (arr[j] < arr[min]) {
      min = j;
    }
    j++;
    if (j >= arr.size()) {
      std::swap(arr[i], arr[min]);
      i++;
      j = i;
      min = i;
    }
  }

  int current() override { return j; }

  bool done() override { return i >= arr.size() - 1; }

  bool sorted(int idx) override { return (idx < i) || done(); }

  void reset() override {
    i = 0;
    j = 0;
    min = 0;
  }

private:
  int i, j, min;
};
