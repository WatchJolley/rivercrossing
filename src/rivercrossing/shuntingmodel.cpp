#include "rivercrossing/shuntingmodel.h"

void ShuntingModel::configureType(std::string type)
{
    this->type = type;
}

void ShuntingModel::buildNetwork()
{
    if (!type.compare("CNN"))
    {
        CNN.init();
        CNN.buildNetwork();
    }
    if (!type.compare("static"))
    {
        SM.init();
    }
}

void ShuntingModel::buildNetwork(vector<double> &genome)
{
    if (!type.compare("CNN")) CNN.buildNetwork(genome);
}

void ShuntingModel::activate()
{
    if (!type.compare("CNN")) CNN.activate();
    if (!type.compare("static")) SM.activate();
}

void ShuntingModel::print(int Cx, int Cy)
{
    if (!type.compare("CNN")) CNN.print();
    if (!type.compare("static"))SM.print();
}

double ShuntingModel::getActivity(int x, int y)
{
    if (!type.compare("CNN"))
        return CNN.getActivity(x,y);
    if (!type.compare("static"))
        return SM.getActivity(x,y);
}

void ShuntingModel::updateIotas(int x, int y, double iota)
{
    if (!type.compare("CNN"))
    {
        CNN.updateIota( x, y, iota );
        CNN.updateActivity( x, y, iota );
    }
    if (!type.compare("static"))
    {
        SM.updateIota( x, y, iota );
        SM.updateActivity( x, y, iota );
    }
}

double ShuntingModel::returnLargestActivity()
{
    if (!type.compare("CNN")) return CNN.largestActivity;
    if (!type.compare("static")) return SM.largestActivity;
}


