#ifndef CNN_FILTER_H
#define CNN_FILTER_H

#include <vector>

using namespace std;

class CNN_filter
{
private:
 vector< vector<double> > weights;
public:
  CNN_filter(int size, vector<double> &genotype, int &count);
  int getSize();
  double getWeight(int x, int y);
};

#endif // CNN_FILTER_H
