#ifndef MOVEMENT_NETWORK_H
#define MOVEMENT_NETWORK_H

#include "network/network.h"
#include "tools/defines.h"

#include "rivercrossing/shuntingmodel.h"

class movement_network
{
private:
    vector<double> outputs;
    int output_size;
    Network network;
    vector<unsigned> topology { 8, 8 };
    vector<double> evolved_genome { 1.0, 0, 0, 0, 0, 1.0, 0, 0, 0, 0, 1.0, 0, 0, 0, 0, 1.0 };

    bool inRange(int location)
    {
      if ( (0 <= location) && (location < RIVER::x ) ) return true;
      return false;
    }

public:
    void buildNetwork();
    void buildNetwork(vector<double> genome);
    void update(vector<double> inputs);

    void loadInputs(int x, int y, ShuntingModel &SM, vector<double> &inputs, double normActivity);
    void getOutputs(vector<double> &_outputs);
};

#endif // MOVEMENT_NETWORK_H
