#ifndef SHUNTINGMODEL_H
#define SHUNTINGMODEL_H

#include <cmath>        // std::abs

#include "tools/config.h"
#include "rivercrossing/shunting_network/CNN_SM.h"
#include "rivercrossing/shunting_network/static_SM.h"

class ShuntingModel {
private:
  CNN_SM CNN;
  static_SM SM;
  std::string type;
public:
  void configureType(std::string type);
  void buildNetwork();
  void buildNetwork(vector<double> &genome);
  void updateIotas(int x, int y, double iota);
  void activate();

  void print(int Cx=-1, int Cy=-1);
  void printNorm();
  void printPath();

  double getActivity(int x, int y);
  double returnLargestActivity();
};


#endif // SHUNTINGMODEL_H
