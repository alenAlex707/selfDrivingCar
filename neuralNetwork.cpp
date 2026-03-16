#include <vector>
#include <cstdlib>
#include <cmath>

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

    neuralNetwork()
    {
        weights_ih.assign(INPUT_N, vector<float>(HIDDEN_N, 0.0f));
        weights_ho.assign(HIDDEN_N, vector<float>(OUTPUT_N, 0.0f));
        bias_h.assign(HIDDEN_N, 0.0f);
        bias_o.assign(OUTPUT_N, 0.0f);
    }

    void randomize()
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

    vector<float> forward(vector<float> inputs)
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
};