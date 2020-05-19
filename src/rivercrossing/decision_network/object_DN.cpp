#include "rivercrossing/decision_network/object_DN.h"

void Object_DN::buildNetwork()
{ build(evolved_genome); }

void Object_DN::buildNetwork(vector<double> genome)
{ build(genome); }

void Object_DN::build(vector<double> g)
{
    for (int o = 0; o < topology.back(); o++)
        outputs.push_back( 0 );

    DN.buildNetwork(topology, g);
    output_size = topology.back();
}

void Object_DN::convertOutputs()
{
    for (int n = 0; n < outputs.size(); n++) {
        double result = outputs.at(n);
        if (result >  0.3f) {
            outputs.at(n) =  1;
        } else if (result < -0.3f) {
            outputs.at(n) = -1;
        } else {
            outputs.at(n) =  0;
        }
    }
}

void Object_DN::loadInputs(dsWorld &world, vector<double> &inputs, int object, bool _carrying)
{
    inputs.clear();
    inputs.push_back((object == EMPTYCELL) ? 1 : 0);
    inputs.push_back((object == RESOURCE) ? 1 : 0);
    inputs.push_back((object == STONE)    ? 1 : 0);
    inputs.push_back((object == WATER)    ? 1 : 0);
    inputs.push_back((object == TRAP)     ? 1 : 0);
    inputs.push_back( _carrying ? 1 : 0);
}

