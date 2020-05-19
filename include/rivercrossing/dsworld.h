#ifndef DSWORLD_H
#define DSWORLD_H

#include <iostream>
#include <vector>

#include "tools/random.h"
#include "tools/defines.h"

using namespace std;

class dsWorld {
private:
  int river = 0;
  void build(int riverWidth);
  double heatmap[RIVER::x][RIVER::y];

public:
  dsWorld();
  dsWorld(int riverWidth);
  dsWorld(int riverWidth, int seed);
  int cell[RIVER::x][RIVER::y];
  int getCell(int x, int y) { return cell[x][y]; };
  int getRiverWidth() { return river; };

  double getIota(vector<double> iota, int x, int y, bool isObject = false);
  void getRGB(int object, vector<double> &RGB);

  void printAnimat(int location_x, int location_y);
  void print() { printAnimat(-1,-1); }

  void updateHeatmap(int x, int y) { heatmap[x][y]+=0.1; };
  double getHeatmapValue(int x, int y) { return heatmap[x][y]; };
  void printHeatmap();

};

#endif // DSWORLD_H
