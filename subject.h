#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include <vector>
#include "observer.h"
using namespace std;

class Subject {
 protected:
  std::vector<Observer*> observers;

 public:
  virtual ~Subject() = default;
  void attach(Observer *o);   // attach an Observer ptr to observers
  void detach(Observer *o);   // detach an Observer from observers
  virtual void notifyObservers() = 0;   // notify observers by calling observers' notify()
  virtual void notifyObservers(vector<vector<Cell>>& grid, vector<int> wKing, vector<int> bKing);
};

#endif
