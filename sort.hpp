#pragma once
#include <vector>

enum class SortAlgo { Bubble, Selection, Insertion };

class Sort {
public:
  Sort(std::vector<int> &arr) : arr(arr) {}
  virtual ~Sort() {}
  virtual void step() = 0;
  virtual int current() = 0;
  virtual bool done() = 0;
  virtual bool sorted(int idx) = 0;
  virtual void reset() = 0;

protected:
  std::vector<int> &arr;
};
