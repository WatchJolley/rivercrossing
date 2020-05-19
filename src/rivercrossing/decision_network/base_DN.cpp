#include "rivercrossing/decision_network/base_DN.h"

void base_DN::update(vector<double> inputs)
{
    vector<double> results;

    DN.feedForward(inputs);
    DN.getResults(results);

    for (int o = 0; o < output_size; o++)
        outputs.at(o) = results.at(o);

}
