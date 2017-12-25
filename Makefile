#Makefile
#Authors : antoine.montes

CC=gcc -rdynamic

CPPFLAGS =  `pkg-config gtk+-3.0 --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS =
LDLIBS= `pkg-config gtk+-3.0 --libs sdl` -lSDL_image -lm

SRC = main.c ImageTreatment/image_operations.c Tools/tools.c ImageTreatment/segmentation.c GTK/gtk.c NeuralNetwork/neural_network_XOR.c	NeuralNetwork/neural_network.c NeuralNetwork/neural_network_tools.c Detection/detect_char.c
OBJ= $(SRC:.c=.o)
DEP= $(SRC:.c=.d)

all: main

main: $(OBJ)

clean:
		$(RM) $(OBJ) $(OBJ_TESTS) $(DEP) $(DEP_TESTS) main && clear
