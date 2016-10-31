# include "image_operation.h"
# include <err.h>
# include <stdlib.h>

int main(int argc, char* argv[])
{
    	if ( argc < 2 )
        	errx(1, "must provide an argument");
    	SDL_Surface* img = NULL;
    	init_sdl();
    	img = load_image(argv[1]);
 	display_image(img);
	SDL_Surface* img2 = SDL_redim(1200 , 700, img);
	display_image(img2);
	SDL_Surface* img3 = grey_lvl(img2);
	display_image(img3);
	SDL_Surface* img4 = black_and_white(img3);
	display_image(img4);
	SDL_Surface* img5 = superposition(img4);
	display_image(img5);

	int *T = malloc(4 * sizeof (int));
   	*(T + 0) = 0;
   	*(T + 1) = 0;
  	*(T + 2) = 150;
 	*(T + 3) = 90;
	SDL_Surface* img6= cut_image(img, T);
	SDL_Surface* img7 = green(img6);
	img7 = green2(img7);
	SDL_SaveBMP(img7,"img7.bmp");
	display_image(img7);
	character_block(img7);
	SDL_FreeSurface(img7);
	SDL_FreeSurface(img);
	return 0;
}


