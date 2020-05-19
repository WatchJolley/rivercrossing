#ifndef DEFINES_H
#define DEFINES_H

enum CellContent {
  EMPTYCELL = 0,
  RESOURCE = 1,
  STONE = 2,
  WATER = 3,
  TRAP = 4,
  AGENT = 10
};

enum DeathCause {
  NONE = 0,
  AGE = 1,
};

namespace RIVER
{
  const int x = 20;
  const int y = 20;
  const int numOfObjects = 5;
  const double max_iota = 15.00;
}

#endif // DEFINES_H
