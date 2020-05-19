#ifndef ANIMAT_H
#define ANIMAT_H

#include "tools/random.h"

class animat
{
public:
  animat(int y = 0);
  int x, y, age, death;
  bool carrying, debug;
};

#endif // ANIMAT_H
