#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


SDL_Surface* load_img()
{
    	SDL_Surface *img = NULL ;
    	SDL_Init(SDL_INIT_VIDEO);
    	img = IMG_Load("img.bmp");
    	return img ;
}


Uint32 get_pixel(SDL_Surface *img, int x , int y)
{
    	int bpp = img -> format -> BytesPerPixel;
    	Uint8 *pix = (Uint8*)img->pixels + y * img->pitch + x * bpp;
    	switch(bpp)
    	{	
    	case 1:
		return *pix;
	case 2:
		return *(Uint16 *)pix;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return pix[0] << 16 | pix[1] << 8 | pix[2] ;
		else
			return pix[0] | pix[1] << 8 | pix[2] << 16;
	case 4:
		return *(Uint32 *)pix;
	default:
		return 0;
	}
}

void define_pixel(SDL_Surface* img , int x , int y , Uint32 pixel)
{
	int bpp = img->format->BytesPerPixel;
	Uint8 *pi = (Uint8*)img->pixels + y * img->pitch + x * bpp;
	switch(bpp)
	{
	case 1:
		*pi = pixel;
		break;
	case 2:
		*(Uint16*)pi = pixel;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
			pi[0] = (pixel >> 16) & 0xff ;
			pi[1] = (pixel >> 8) & 0xff;
			pi[2] = pixel & 0xff;
		}
		else
		{
			pi[0] = pixel & 0xff;
			pi[1] = (pixel >> 8) & 0xff;
			pi[2] = (pixel >> 16) & 0xff;
		}
		break ;
	case 4:
		*(Uint32 *)pi = pixel;
		break;
	}
}



SDL_Surface* resize_img(SDL_Surface *img)
{
    SDL_Surface *new = NULL;
    int i = 0 , j = 0 , x = 0 , y = 0;
    
}




SDL_Surface *grey_lvl(SDL_Surface* img)
{	
    Uint8 r , g , b;
    Uint32 pixel, newpixel;
    for (int y = 0 ; y < img -> h ; y++)
    {
	for (int x = 0 ; x < img -> w ; x++)
	{
	     pixel = get_pixel(img , x , y);
             SDL_GetRGB(pixel , img->format , &r , &g , &b);
             float mid = ((0.3 * r  + 0.59 * g + 0.11 * b) /3);
	     newpixel = SDL_MapRGB(img -> format , mid , mid  , mid );
             define_pixel(img , x , y , newpixel);
	}
    }
    return img;
}	

SDL_Surface* black_and_white(SDL_Surface* img)
{
    Uint32 pix , pixel;
    Uint32 ref_pixel = SDL_MapRGB(img -> format , 128 , 128 , 128);
    for (int y = 0 ; y < img -> h ; y++)
    {
        for (int x = 0; x < img -> w ; x++)
	{
	    pixel = get_pixel(img , x , y);
	    if (pixel > ref_pixel)
		pix = SDL_MapRGB(img -> format , 255, 255 , 255);
	    else
		pix = SDL_MapRGB(img -> format , 0 , 0 , 0);
	    define_pixel(img , x , y , pix);
	}
    }
    return img ;
}
int main ()
{
	SDL_Surface* img = load_img();
	//SDL_Surface* new = grey_lvl(img);
	SDL_Surface* new = black_and_white(img);
	SDL_Surface* screen = SDL_SetVideoMode(1200, 700, 0, SDL_SWSURFACE);
	SDL_BlitSurface(new, NULL , screen, NULL);
	SDL_UpdateRect(screen, 0 , 0 , 1200 , 700);
	sleep(3);
	return 0;
}

