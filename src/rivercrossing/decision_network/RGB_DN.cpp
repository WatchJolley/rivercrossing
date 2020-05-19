#include "rivercrossing/decision_network/RGB_DN.h"

void RGB_DN::buildNetwork()
{ build(evolved_genome); }

void RGB_DN::buildNetwork(vector<double> genome)
{ build(genome); }

void RGB_DN::build(vector<double> g)
{
    for (int o = 0; o < topology.back(); o++)
        outputs.push_back( 0 );

    DN.buildNetwork(topology, g);
    output_size = topology.back();
}

void RGB_DN::loadInputs(dsWorld &world, vector<double> &inputs, int _object, bool _carrying)
{
    inputs.clear();
    world.getRGB(_object,inputs);
    inputs.push_back( _carrying ? 1 : 0);
}
