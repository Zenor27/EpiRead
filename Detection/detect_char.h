# ifndef DETECT_CHAR_H_
# define DETECT_CHAR_H_

# include <stdlib.h>
# include <stdio.h>

#include "../NeuralNetwork/neural_network_tools.h"

char DetectText(struct Neural_Network *net, double* letter);

# endif
