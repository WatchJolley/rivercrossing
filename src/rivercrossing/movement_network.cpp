#include "rivercrossing/movement_network.h"
#include <algorithm>    // std::min_element, std::max_element

void movement_network::buildNetwork()
{
    for (int o = 0; o < topology.back(); o++)
        outputs.push_back( 0 );

    network.buildNetwork(topology, evolved_genome);
    output_size = topology.back();
}

void movement_network::buildNetwork(vector<double> genome)
{
    for (int o = 0; o < topology.back(); o++)
        outputs.push_back( 0 );

    network.buildNetwork(topology, genome);
    output_size = topology.back();
}

void movement_network::update(vector<double> inputs)
{
    vector<double> results;

    network.feedForward(inputs);
    network.getResults(results);

    for (int o = 0; o < output_size; o++)
        outputs.at(o) = results.at(o);

}

void movement_network::loadInputs(int x, int y, ShuntingModel &SM, vector<double> &inputs, double normActivity)
{
    vector<double> iotas;
    double value = 0.0;

    inputs.clear();

    value = (inRange (x+1) ) ? SM.getActivity(x+1, y)/normActivity : 0;
    iotas.push_back(value);

    value = (inRange (x-1) ) ? SM.getActivity(x-1, y)/normActivity : 0;
    iotas.push_back(value);

    value = (inRange (y+1) ) ? SM.getActivity(x, y+1)/normActivity : 0;
    iotas.push_back(value);

    value = (inRange (y-1) ) ? SM.getActivity(x, y-1)/normActivity : 0;
    iotas.push_back(value);

    if ( (inRange (x-1)) && (inRange (y+1)) )
    { value = SM.getActivity(x-1, y+1)/normActivity; }  else {
      value = 0.0;}
    iotas.push_back(value);

    if ( (inRange (x+1)) && (inRange (y+1)) )
    { value = SM.getActivity(x+1, y+1)/normActivity; }  else {
      value = 0.0;}
    iotas.push_back(value);

    if ( (inRange (x-1)) && (inRange (y-1)) )
    { value = SM.getActivity(x-1, y-1)/normActivity; }  else {
      value = 0.0;}
    iotas.push_back(value);

    if ( (inRange (x+1)) && (inRange (y-1)) )
    { value = SM.getActivity(x+1, y-1)/normActivity; }  else {
      value = 0.0;}
    iotas.push_back(value);


    double highest = -99.999;
    int poitner = -1;
    for(int i = 0; i < iotas.size(); i++) {
        if( iotas.at(i) > highest) {
            poitner = i;
            highest =  iotas.at(i);
        }
    }

    for(int i = 0; i < iotas.size(); i++)
    {
        int input = (i == poitner) ? 1 : 0;
        inputs.push_back(input);
    }

//    // add bias
//    inputs.push_back(1.0);

}

void movement_network::getOutputs(vector<double> &_outputs)
{
    for (int o = 0; o < output_size; o++)
         _outputs.push_back(outputs.at(o));
}
