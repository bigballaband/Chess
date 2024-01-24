#include "subject.h"

void Subject::notifyObservers(vector<vector<Cell>>& grid, vector<int> wKing, vector<int> bKing) {}

// attach an Observer ptr to observers
void Subject::attach(Observer *o) {
  observers.emplace_back(o);
}

// detach an Observer from observers
void Subject::detach(Observer *o) {
  for (auto it = observers.begin(); it != observers.end();) {
    if (*it == o) {
      it = observers.erase(it);
      break;
    } else {
      ++it;
    }
  }
}

