// tools.h

# ifndef TOOLS_H_
# define TOOLS_H_

# include <stdlib.h>
# include <SDL/SDL.h>
# include <err.h>

Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y);

void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

void wait_for_keypressed(void);

SDL_Surface* display_image(SDL_Surface *img);

SDL_Surface* copy_image(SDL_Surface *img);

void array_swap(int array[], size_t i, size_t j);

void array_select_sort(int array[], size_t len);

void create_matrix_file(SDL_Surface *img, char *filename);

void print_matrix(double mat[], size_t lines, size_t cols);

double* matrixFromFile(char *filename);

double *goalArray(char letter);

double **goalMatrix();

double **lettersMatrix();

double *create_matrix(SDL_Surface *img);

void append(char* s, char c);

char* concat(const char *s1, const char *s2);

# endif
