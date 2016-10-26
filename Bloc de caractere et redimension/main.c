# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include "pixel_operations.h"
# include <err.h>
# include <stdlib.h>

void wait_for_keypressed(void) {
  SDL_Event             event;
  // Infinite loop, waiting for event
  for (;;) {
    // Take an event
    SDL_PollEvent( &event );
    // Switch on event type
    switch (event.type) {
    // Someone pressed a key -> leave the function
    case SDL_KEYDOWN: return;
    default: break;
    }
  // Loop until we got the expected event
  }
}

void init_sdl(void) {
  // Init only the video part
  if( SDL_Init(SDL_INIT_VIDEO)==-1 ) {
    // If it fails, die with an error message
    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
  }
  // We don't really need a function for that ...
}

SDL_Surface* load_image(char *path) {
  SDL_Surface          *img;
  // Load an image using SDL_image with format detection
  img = IMG_Load(path);
  if (!img)
    // If it fails, die with an error message
    errx(3, "can't load %s: %s", path, IMG_GetError());
  return img;
}

SDL_Surface* display_image(SDL_Surface *img) {
  SDL_Surface          *screen;
  // Set the window to the same size as the image
  screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
  if ( screen == NULL ) {
    // error management
    errx(1, "Couldn't set %dx%d video mode: %s\n",
         img->w, img->h, SDL_GetError());
  }
 
  /* Blit onto the screen surface */
  if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
    warnx("BlitSurface error: %s\n", SDL_GetError());
 
  // Update the screen
  SDL_UpdateRect(screen, 0, 0, img->w, img->h);
 
  // wait for a key
  wait_for_keypressed();
 
  // return the screen for further uses
  return screen;
}


int main(int argc, char* argv[])
{
    	if ( argc < 2 )
        	errx(1, "must provide an argument");
    	//SDL_Surface* img = NULL , *ecran;
	SDL_Surface* img = NULL;
    	//init_sdl();
    	img = load_image(argv[1]);
 	//ecran = display_image(img);
	
	//SDL_Surface* img_redim = SDL_redim(100, 100, img);
	//SDL_SaveBMP(img_redim, "img_redim.bmp");
	//ecran = display_image(img_redim);

	int* startx = malloc(sizeof(int));
	int* starty = malloc(sizeof(int));
	*startx = 0;
	*starty = 0;

	/*for(int i = 0; i<2; ++i)
	{
		char* filename = malloc((9) * sizeof(char));
		*(filename + 0) = 'c';
		*(filename + 1) = 'h';
		*(filename + 2) = 'a';
		*(filename + 3) = (char)(i+49);
		*(filename + 4) = '.';
		*(filename + 5) = 'b';
		*(filename + 6) = 'm';
		*(filename + 7) = 'p';
		*(filename + 8) = '\0';
		SDL_Surface* image_decoupee = Bloc_de_caractere(img, startx, starty);
		SDL_SaveBMP(image_decoupee, filename);
		SDL_FreeSurface(image_decoupee);
	}*/

	/*SDL_Surface* img_green = green(img);
	SDL_SaveBMP(img_green, "green.bmp");
	SDL_Surface* img_green2 = green2(img_green);
	SDL_SaveBMP(img_green2, "green2.bmp");

	SDL_FreeSurface(img_green);
	SDL_FreeSurface(img_green2);*/

	//Bloc_de_caractere_ingreenimg(img, startx, starty);


	/*find_next_pixel_notgreen(img, startx, starty);
	int y = *starty;
	int x = *startx;
	int x0 = x;
            
	for (; y < img->h && getpixel(img, x0, y) != SDL_MapRGBA(img->format, 0, 255, 0,255); ++y)
        {
            	for (x = x0; x < img->w && getpixel(img, x, y) != SDL_MapRGBA(img->format, 0, 255, 0,255); ++x)
        	{
			putpixel(img, x, y, SDL_MapRGBA(img->format, 0, 0, 255,255));
	        }
	}
	SDL_SaveBMP(img, "AAAAAAA.bmp");*/




	SDL_FreeSurface(img);
	return 0;
}


