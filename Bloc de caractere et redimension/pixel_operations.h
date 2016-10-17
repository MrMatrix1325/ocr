// pixel_operations.h
 
# ifndef PIXEL_OPERATIONS_H_
# define PIXEL_OPERATIONS_H_
 
# include <stdlib.h>
# include <SDL.h>
 
Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y);
void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
SDL_Surface* SDL_redim(int W2, int H2, SDL_Surface *img);
SDL_Surface* Bloc_de_caractere(SDL_Surface *img);
int* Find_min_and_max_pixels(SDL_Surface *img, int i, int j, int *L);
SDL_Surface* decoupe_image(SDL_Surface *img, int *L);
 
# endif