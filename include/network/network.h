#ifndef Network_H
#define Network_H

#include "network/neuron.h"
#include <iostream>
#include <vector>

using namespace std;

typedef vector<Neuron> Layer;
typedef vector<double> weights;

class Network
{
public:
    Network() {};
    void buildNetwork(const vector<unsigned> &topology,  vector<double> &genotype);
	void feedForward(const vector<double> &inputVals);
	void backProp(const vector<double> &targetVals);
	void getResults(vector<double> &resultVals) const;
	double getRecentAverageError(void) const { return m_recentAverageError; }

private:
	vector<Layer> m_layers; //m_layers[layerNum][neuronNum]
	double m_error;
	double m_recentAverageError;
	static double m_recentAverageSmoothingFactor;
};

#endif // Network_H
