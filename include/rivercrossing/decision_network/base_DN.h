#ifndef DN_H
#define DN_H

#include "network/network.h"
#include "tools/defines.h"

#include "rivercrossing/dsworld.h"

class base_DN
{
protected:
    Network DN;
    vector<double> outputs;
    int output_size = 0;
    virtual void build(vector<double> g) = 0;
public:
    virtual void buildNetwork() = 0;
    virtual void buildNetwork(vector<double> genome) = 0;
    virtual void loadInputs(dsWorld &world, vector<double> &inputs, int object,  bool _carrying) = 0;

    void update(vector<double> inputs);
    double getAction(){ return outputs.at(0);};
    double getOutput(int pos) { return outputs.at(pos); };
};

#endif // DN_H
