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

    neuralNetwork()
    {
        weights_ih.assign(INPUT_N, vector<float>(HIDDEN_N, 0.0f));
        weights_ho.assign(HIDDEN_N, vector<float>(OUTPUT_N, 0.0f));
        bias_h.assign(HIDDEN_N, 0.0f);
        bias_o.assign(OUTPUT_N, 0.0f);
    }
};