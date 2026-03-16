#pragma once

#include <vector>

using namespace std;

class neuralNetwork
{
public:
  static const int INPUT_N = 5;
  static const int HIDDEN_N = 6;
  static const int OUTPUT_N = 2;

  vector<vector<float>> weights_ih;
  vector<vector<float>> weights_ho;

  vector<float> bias_h;
  vector<float> bias_o;

  neuralNetwork();

  void randomize();

  vector<float> forward(vector<float> inputs);

  vector<float> getWeights();

  void setWeights(vector<float> flat);
};