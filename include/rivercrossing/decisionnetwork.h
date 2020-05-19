#ifndef DECISIONNETWORK_H
#define DECISIONNETWORK_H

#include "network/network.h"
#include "tools/defines.h"
#include "tools/config.h"

#include "rivercrossing/dsworld.h"
#include "rivercrossing/decision_network/object_DN.h"
#include "rivercrossing/decision_network/RGB_DN.h"

class DecisionNetwork
{
private:
 Object_DN object;
 RGB_DN RGB;
 std::string type;
public:
    void configureType(std::string type);
    void buildNetwork();
    void buildNetwork(vector<double> &genome);
    void loadInputs(dsWorld &world, vector<double> &inputs, int _object, bool _carrying);

    void      update(vector<double> inputs);
    double    getAction();
    double    getOutput(int pos);
    double    compareInputs(dsWorld worlds);
};

#endif // DECISIONNETWORK_H
