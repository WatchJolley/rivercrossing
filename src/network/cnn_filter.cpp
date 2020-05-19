#include "network/cnn_filter.h"

CNN_filter::CNN_filter(int size, vector<double> &genotype, int &count)
{

 for(int x = 0; x < size; x++ ) {
  vector<double> w;
   for(int y = 0; y < size; y++ ) {
    w.push_back( genotype.at( count ) );
    count++;
   }
   weights.push_back( w );
  }
}


int CNN_filter::getSize()
{ return weights.size(); }

double CNN_filter::getWeight(int x, int y)
{ return weights.at(x).at(y); }
