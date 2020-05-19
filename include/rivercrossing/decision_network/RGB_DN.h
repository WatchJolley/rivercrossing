#ifndef DN_RGB_H
#define DN_RGB_H

#include "network/network.h"
#include "tools/defines.h"
#include "tools/config.h"

#include "rivercrossing/dsworld.h"
#include "rivercrossing/decision_network/base_DN.h"

class RGB_DN: public base_DN
{
private:
 vector<unsigned> topology { 4, config::DN_HIDDEN_LAYER, config::DN_HIDDEN_LAYER2, 2 };
 vector<double> evolved_genome { 0.0 };
 void build(vector<double> g);
public:
 void buildNetwork();
 void buildNetwork(vector<double> genome);

 void loadInputs(dsWorld &world, vector<double> &inputs, int _object, bool _carrying);
};

#endif // DN_RGB_H
