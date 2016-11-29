# ifndef IMAGE_OPERATION_H_
# define IMAGE_OPERATION_H_

# include <stdlib.h>
# include <SDL.h>

Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y);
void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void wait_for_keypressed(void);
SDL_Surface* load_image(char *path) ;
SDL_Surface* display_image(SDL_Surface *img);
void init_sdl(void);
SDL_Surface* grey_lvl(SDL_Surface *img);
SDL_Surface* black_and_white(SDL_Surface* img);
SDL_Surface* superposition(SDL_Surface* img);
SDL_Surface* SDL_redim(int W2, int H2, SDL_Surface *img);
void character_block(SDL_Surface *img);
int* Find_min_and_max_pixels(SDL_Surface *img, int i, int j, int *L);
SDL_Surface* cut_image(SDL_Surface *img, int *L);
SDL_Surface* green(SDL_Surface *img);
SDL_Surface* green2(SDL_Surface *img);

# endif
