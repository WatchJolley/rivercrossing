#include "network/network.h"

#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

double Network::m_recentAverageSmoothingFactor = 100.0; // number of training samples to average over

void Network::getResults(vector<double> &resultVals) const
{
	resultVals.clear();

	for(unsigned n = 0; n < m_layers.back().size(); ++n)
	{
		resultVals.push_back(m_layers.back()[n].getOutputVal());
	}
}

void Network::backProp(const std::vector<double> &targetVals)
{
	// calculate overal Network error (RMS of output neuron errors)

	Layer &outputLayer = m_layers.back();
	m_error = 0.0;

	for(unsigned n = 0; n < outputLayer.size() - 1; ++n)
	{
		double delta = targetVals[n] - outputLayer[n].getOutputVal();
		m_error += delta *delta;
	}
	m_error /= outputLayer.size() - 1; // get average error squared
	m_error = sqrt(m_error); // RMS

	// implement a recent average measurement:

	m_recentAverageError =
			(m_recentAverageError * m_recentAverageSmoothingFactor + m_error)
			/ (m_recentAverageSmoothingFactor + 1.0);
	// calculate output layer gradients

	for(unsigned n = 0; n < outputLayer.size() - 1; ++n)
	{
		outputLayer[n].calcOutputGradients(targetVals[n]);
	}
	// calculate gradients on hidden layers

	for(unsigned layerNum = m_layers.size() - 2; layerNum > 0; --layerNum)
	{
		Layer &hiddenLayer = m_layers[layerNum];
		Layer &nextLayer = m_layers[layerNum + 1];

		for(unsigned n = 0; n < hiddenLayer.size(); ++n)
		{
			hiddenLayer[n].calcHiddenGradients(nextLayer);
		}
	}

	// for all layers from outputs to first hidden layer,
	// update connection weights

	for(unsigned layerNum = m_layers.size() - 1; layerNum > 0; --layerNum)
	{
		Layer &layer = m_layers[layerNum];
		Layer &prevLayer = m_layers[layerNum - 1];

		for(unsigned n = 0; n < layer.size(); n++)
		{
			layer[n].updateInputWeights(prevLayer);
		}
	}
}

void Network::feedForward(const vector<double> &inputVals)
{
	// check the num of inputVals euqal to neuronnum expect bias
	assert(inputVals.size() == m_layers[0].size());

	// assign {latch} the input values into the input neurons
	for(unsigned i = 0; i < inputVals.size(); i++){
		m_layers[0][i].setOutputVal(inputVals[i]);
	}

	// forward propagate
	for(unsigned layerNum = 1; layerNum < m_layers.size(); layerNum++){
		Layer &prevLayer = m_layers[layerNum - 1];
		for(unsigned n = 0; n < m_layers[layerNum].size(); n++){
			m_layers[layerNum][n].feedForward(prevLayer);
		}
	}
}

void Network::buildNetwork(const vector<unsigned> &topology,  vector<double> &genotype)
{
    int count = 0;
    unsigned numLayers = topology.size();
    for(unsigned layerNum = 0; layerNum < numLayers; ++layerNum){
        m_layers.push_back(Layer());
        // numOutputs of layer[i] is the numInputs of layer[i+1]
        // numOutputs of last layer is 0
        unsigned numOutputs = layerNum == topology.size() - 1 ? 0 :topology[layerNum + 1];

        // we have made a new Layer, now fill it ith neurons, and
        // add a bias neuron to the layer:
        for(unsigned neuronNum = 0; neuronNum < topology[layerNum]; neuronNum++){
            Neuron newNeuron = Neuron(numOutputs, neuronNum, genotype, count );
            m_layers.back().push_back(newNeuron);
        }

        // force the bias node's output value to 1.0. It's the last neuron created above
        m_layers.back().back().setOutputVal(1.0);
    }
}
