#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include <vector>
using namespace std;

class Cell;

class Observer {
 public:
  virtual void notify(Cell &c) = 0;
  virtual void notify(int rol, int col, int destRow, int destCol, vector<vector<Cell>>& grid, vector<int> wKing, vector<int> bKing) {}
  virtual ~Observer() = default;
};

#endif
