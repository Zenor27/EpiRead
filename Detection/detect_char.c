#include "detect_char.h"


//The classical forward pass
static void ForwardPass(struct Neural_Network *net)
{
  double sum, weight, output, bias;

  //Calculate Output for Hidden neurons
  for (int h = 0; h < net -> nbHidden; h++)
  {
    sum = 0.0;
    for (int i = 0; i < net -> nbInput; i++)
    {
      weight = net -> WeightIH[i][h];
      output = net -> OutputI[i];

      sum += weight * output;
    }
    bias = net -> BiasH[h];
    net -> OutputH[h] = Sigmoid(sum + bias);
  }

  //Calculate Output for Output neurons
  for (int o = 0; o < net -> nbOutput; o++)
  {
    sum = 0.0;
    for (int h = 0; h < net -> nbHidden; h++)
    {
      weight = net -> WeightHO[h][o];
      output = net -> OutputH[h];

      sum += weight * output;
    }
    bias = net -> BiasO[o];
    net -> OutputO[o] = Sigmoid(sum + bias);
  }
}

char DetectText(struct Neural_Network *net, double* letter)
{
    for(int i = 0; i < net -> nbInput; i++)
    {
        net -> OutputI[i] = letter[i];
    }

    ForwardPass(net);

    //Retrieve the character detected
    int pos = RetrievePos(net);
    char c = RetrieveChar(pos);

    return c;
}
