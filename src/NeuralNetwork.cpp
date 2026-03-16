#include "NeuralNetwork.h"
#include <cstdlib>
#include <cmath>

NeuralNetwork::NeuralNetwork()
{
  weights_ih.assign(INPUT_N, vector<float>(HIDDEN_N, 0.0f));
  weights_ho.assign(HIDDEN_N, vector<float>(OUTPUT_N, 0.0f));
  bias_h.assign(HIDDEN_N, 0.0f);
  bias_o.assign(OUTPUT_N, 0.0f);
  randomize();
}

void NeuralNetwork::randomize()
{
  for (int i = 0; i < weights_ih.size(); i++)
  {
    for (int j = 0; j < weights_ih[i].size(); j++)
    {
      weights_ih[i][j] = (float)rand() / (float)RAND_MAX * 2.0f - 1.0f;
    }
  }

  for (int i = 0; i < weights_ho.size(); i++)
  {
    for (int j = 0; j < weights_ho[i].size(); j++)
    {
      weights_ho[i][j] = (float)rand() / (float)RAND_MAX * 2.0f - 1.0f;
    }
  }

  for (int i = 0; i < bias_h.size(); i++)
  {
    bias_h[i] = (float)rand() / (float)RAND_MAX * 2.0f - 1.0f;
  }

  for (int i = 0; i < bias_o.size(); i++)
  {
    bias_o[i] = (float)rand() / (float)RAND_MAX * 2.0f - 1.0f;
  }
}

vector<float> NeuralNetwork::forward(vector<float> inputs)
{
  vector<float> hiddenLayer(HIDDEN_N, 0.0f);
  vector<float> outputLayer(OUTPUT_N, 0.0f);

  for (int i = 0; i < HIDDEN_N; i++)
  {
    for (int j = 0; j < INPUT_N; j++)
    {
      hiddenLayer[i] += inputs[j] * weights_ih[j][i];
    }
    hiddenLayer[i] = tanh(hiddenLayer[i] + bias_h[i]);
  }

  for (int i = 0; i < OUTPUT_N; i++)
  {
    for (int j = 0; j < HIDDEN_N; j++)
    {
      outputLayer[i] += hiddenLayer[j] * weights_ho[j][i];
    }
    outputLayer[i] = tanh(outputLayer[i] + bias_o[i]);
  }
  return outputLayer;
}

vector<float> NeuralNetwork::getWeights()
{
  vector<float> weightsFlattened;

  for (int j = 0; j < weights_ih.size(); j++)
  {
    for (int k = 0; k < weights_ih[j].size(); k++)
    {
      weightsFlattened.push_back(weights_ih[j][k]);
    }
  }

  for (int j = 0; j < weights_ho.size(); j++)
  {
    for (int k = 0; k < weights_ho[j].size(); k++)
    {
      weightsFlattened.push_back(weights_ho[j][k]);
    }
  }

  for (auto const &bh : bias_h)
  {
    weightsFlattened.push_back(bh);
  }

  for (auto const &bo : bias_o)
  {
    weightsFlattened.push_back(bo);
  }

  return weightsFlattened;
}

void NeuralNetwork::setWeights(vector<float> flat)
{
  int id{};

  for (int i = 0; i < weights_ih.size(); i++)
  {
    for (int j = 0; j < weights_ih[i].size(); j++)
    {
      weights_ih[i][j] = flat[id++];
    }
  }

  for (int i = 0; i < weights_ho.size(); i++)
  {
    for (int j = 0; j < weights_ho[i].size(); j++)
    {
      weights_ho[i][j] = flat[id++];
    }
  }

  for (int i = 0; i < bias_h.size(); i++)
  {
    bias_h[i] = flat[id++];
  }

  for (int i = 0; i < bias_o.size(); i++)
  {
    bias_o[i] = flat[id++];
  }
}