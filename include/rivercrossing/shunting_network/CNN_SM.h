#ifndef CNNSM_H
#define CNNSM_H

#include "network/cnn_filter.h"
#include "rivercrossing/shunting_network/base_SM.h"

class CNN_SM : public base_SM {
private:
  vector<double> evolved_genome { -0.00285367 , 0.221453 , 0.15804 , -0.137879 , -0.0406328 , 0.0632916 , 0.150172 , -0.497353 , 0.469101 };
  vector<CNN_filter> filters;
  double CNN_output[RIVER::x][RIVER::y];
  double getCell(int x, int y);
  double getCell2(int x, int y, double activity[20][20]);
  bool   inRange(int x, int y);
  double getTemp(int x, int y);
public:
  void buildNetwork();
  void buildNetwork(vector<double> genome);
  void activate();
  void printPath();

};

#endif // CNN_SM_H
