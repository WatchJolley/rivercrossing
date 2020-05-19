#include "rivercrossing/decisionnetwork.h"
#include <cmath>

void DecisionNetwork::configureType(std::string type)
{
    this->type = type;
}

void DecisionNetwork::buildNetwork(vector<double> &genome)
{
    if (!type.compare("object"))
        object.buildNetwork(genome);
    if (!type.compare("RGB"))
        RGB.buildNetwork(genome);
}

void DecisionNetwork::buildNetwork()
{
    if (!type.compare("object"))
        object.buildNetwork();
    if (!type.compare("RGB"))
        RGB.buildNetwork();
}

void DecisionNetwork::loadInputs(dsWorld &world, vector<double> &inputs, int _object, bool _carrying)
{
    if (!type.compare("object"))
        object.loadInputs(world, inputs, _object, _carrying);
    if (!type.compare("RGB"))
        RGB.loadInputs(world, inputs, _object, _carrying);
}

void DecisionNetwork::update(vector<double> inputs)
{
    if (!type.compare("object"))
    {
        object.update(inputs);
        object.convertOutputs();
    }
    if (!type.compare("RGB"))
        RGB.update(inputs);
}

double DecisionNetwork::getAction(){
    if (!type.compare("object"))
        return object.getAction();
    if (!type.compare("RGB"))
        return RGB.getAction();
}

double DecisionNetwork::getOutput(int pos)
{
    if (!type.compare("object"))
        return object.getOutput(pos);
    if (!type.compare("RGB"))
        return RGB.getOutput(pos);
}

// this is a debug for comparing pre-evoled object DN to RGB
double DecisionNetwork::compareInputs(dsWorld world)
{
    object.buildNetwork();
    vector<double> inputs;
    double totalDiff = 0;

    // while not holding and on grass
    for (int o = 1; o < RIVER::numOfObjects; o++)
    {
        object.loadInputs(world, inputs, 0, false);
        object.update(inputs);
        object.convertOutputs();
        double object_value = object.getOutput(o);

        RGB.loadInputs(world, inputs, o, false);
        RGB.update(inputs);
        double RGB_value = RGB.getOutput(1);

        double difference = std::abs(RGB_value - object_value);
        totalDiff += difference;

//        cout << "On Object 0 , while not holding | " << RGB_value << " vs " << object_value << endl;
    }

    // while holding and on grass
    for (int o = 1; o < RIVER::numOfObjects; o++)
    {
        object.loadInputs(world, inputs, 0, true);
        object.update(inputs);
        object.convertOutputs();
        double object_value = object.getOutput(o);

        RGB.loadInputs(world, inputs, o, true);
        RGB.update(inputs);
        double RGB_value = RGB.getOutput(1);

        double difference = std::abs(RGB_value - object_value);
        totalDiff += difference;

//        cout << "On Object 0 , while holding | " << RGB_value << " vs " << object_value << endl;
    }

    // while not holding and on all objects
    for (int o = 0; o < RIVER::numOfObjects; o++)
    {
        object.loadInputs(world, inputs, o, false);
        object.update(inputs);
        object.convertOutputs();
        double object_value = object.getOutput(0);

        RGB.loadInputs(world, inputs, o, false);
        RGB.update(inputs);
        double RGB_value = RGB.getOutput(0);

        double difference = std::abs(RGB_value - object_value);
        totalDiff += difference;

//        cout << "On Object "<< o << " , while holding | " << RGB_value << " vs " << object_value << endl;
    }

    // while holding and on all objects
    for (int o = 0; o < RIVER::numOfObjects; o++)
    {
        object.loadInputs(world, inputs, o, true);
        object.update(inputs);
        object.convertOutputs();
        double object_value = object.getOutput(0);

        RGB.loadInputs(world, inputs, o, true);
        RGB.update(inputs);
        double RGB_value = RGB.getOutput(0);

        double difference = std::abs(RGB_value - object_value);
        totalDiff += difference;

//        cout << "On Object "<< o << " , while holding | " << RGB_value << " vs " << object_value << endl;
    }

//    cout << "totalDiff : " << -totalDiff << endl;

    return -totalDiff;
}
