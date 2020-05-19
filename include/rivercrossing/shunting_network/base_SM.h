#ifndef SM_H
#define SM_H

#include <iostream>
#include <vector>
#include <tools/defines.h>

using namespace std;

class base_SM
{
protected:
  double activity[RIVER::x][RIVER::y];
  double iota[RIVER::x][RIVER::y];
public:
  virtual void activate() = 0;
  void init();

  void print(int Cx=-1, int Cy=-1);
  void printNorm();

  void updateActivity(int x, int y, double value) { activity[x][y] = value; }
  void updateIota(int x, int y, double value) { iota[x][y] = value; }
  double getIota(int x, int y) { return iota[x][y]; }
  double getActivity(int x, int y) { return activity[x][y]; }

  double largestActivity = 0.0;
};

#endif // SM_H
