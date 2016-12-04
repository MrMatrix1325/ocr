#ifndef RDN_IMAGES
#define RDN_IMAGES

# include <stdlib.h>
# include <SDL.h>

#include "rdn_images.h"
# include "SDL/SDL_image.h"

SDL_Surface* load_image(char *path);

double* computeOutput(char c);

double* computeInput(SDL_Surface *img, unsigned height);

char getCharOutput(struct Network *net);

void binarize(SDL_Surface *img);

void green(SDL_Surface *img);

void green2(SDL_Surface *img);

char* readText(SDL_Surface *img, struct Network* net);

SDL_Surface* decoupe_image(SDL_Surface *img, int *L);

SDL_Surface* Rogne(SDL_Surface *res);

SDL_Surface* SDL_redim(int W2, int H2, SDL_Surface *img);

#endif