#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "image_operation.h"
#include <unistd.h>

int main ()
{
	SDL_Surface* img = IMG_Load("img.bmp");
        SDL_Surface* yes = SDL_redim(1200 , 700, img);
	SDL_Surface* ttt = grey_lvl(yes);
	SDL_Surface* new = black_and_white(ttt);
	SDL_Surface* screen = SDL_SetVideoMode(1200, 700, 
0, SDL_SWSURFACE);
	SDL_BlitSurface(new, NULL , screen, NULL);
	SDL_UpdateRect(screen, 0 , 0 , 1200 , 700);
	sleep(3);
	SDL_Surface *image = superposition(new);
	SDL_BlitSurface(image, NULL , screen, NULL);
	SDL_UpdateRect(screen, 0 , 0 , 1200 , 700);
	sleep(3);
	return 0;
}

