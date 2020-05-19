#ifndef STATICSM_H
#define STATICSM_H

#include "rivercrossing/shunting_network/base_SM.h"

class static_SM : public base_SM {
private:
  bool inRange(int location, int xisSize) {
    if ( (0 <= location) && (location < xisSize ) ) return true;
    return false;
  }

  // if in range return activty else zero
  double m(int x, int y) {
    if ( ( inRange(x, RIVER::x) ) && ( inRange(y, RIVER::y) ) ) return (activity[x][y] > 0) ? activity[x][y] : 0;
    return 0;
  }
  
public:
  void buildNetwork();
  void buildNetwork(vector<double> iotas);
  void activate();
};

#endif // STATIC_SM_H
