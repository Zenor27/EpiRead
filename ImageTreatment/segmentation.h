// segmentation.h

# ifndef SEGMENTATION_H_
# define SEGMENTATION_H_

# include <stdlib.h>
# include <SDL/SDL.h>
# include <err.h>
# include "../Tools/tools.h"
# include "image_operations.h"
# include "../Detection/detect_char.h"


SDL_Surface* lineCut(SDL_Surface *img);

void isolateLine(SDL_Surface *img, struct Neural_Network *net);

void cuttedSurface(SDL_Surface *img, int firstCut,
                    int lastCut, struct Neural_Network *net);

void charCut(SDL_Surface *img);

void isolateChar(SDL_Surface *img, struct Neural_Network *net);

int mediumPixelSpacingHorizontal(SDL_Surface *img);

int mediumPixelSpacingVertical(SDL_Surface *img);

void blockDetection_horizontal(SDL_Surface *img);

void blockDetection_vertical(SDL_Surface *img);

# endif
