#pragma once

#include "sort.hpp"

class InsertionSort : public Sort {
public:
  InsertionSort(std::vector<int> &arr) : Sort(arr) {
    i = 1;
    j = 0;
    key = arr[i];
  }

  void step() override {
    if (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j--;
    } else {
      arr[j + 1] = key;
      i++;
      j = i - 1;
      key = arr[i];
    }
  }

  int current() override { return j; }

  bool done() override { return i >= arr.size(); }

  bool sorted(int idx) override { return idx < i; }

  void reset() override {
    i = 1;
    j = 0;
    key = arr[i];
  }

private:
  int i, j, key;
};
