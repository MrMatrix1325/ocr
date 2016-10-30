#include <stdio.h>
#include <unistd.h>
#include <time.h>
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




SDL_Surface* SDL_redim(int W2, int H2, SDL_Surface *img)
{
	float W = (float)img->w;
	float H = (float)img->h;
	float RX = (float)W2/W;
	float RY = (float)H2/H;
	for ( int i = 0 ; i < W2 ; ++i )
	{
		for ( int j = 0 ; j < H2 ; ++j )
		{
			float valx = i/RX;
			float valy = j/RY;

			int minx = (int)valx;
			int miny = (int)valy;
			int maxx = minx+1;
			int maxy = miny+1;
			double fx = valx-minx;
			double fy = valy-miny;

			Uint8 r, g, b, r1, g1, b1, r2, g2, b2, r3, g3, b3;
                        Uint8 r4, g4, b4;
			Uint32 pixel = get_pixel(img, i, j);
			SDL_GetRGB(pixel, img->format, &r, &g, &b);

			Uint32 minxminy = get_pixel(img, minx, miny);
			Uint32 minxmaxy = get_pixel(img, minx, maxy);
			Uint32 maxxminy = get_pixel(img, maxx, miny);
			Uint32 maxxmaxy = get_pixel(img, maxx, maxy);

			SDL_GetRGB(minxminy, img->format, &r1, &g1, &b1);
			SDL_GetRGB(maxxminy, img->format, &r2, &g2, &b2);
			SDL_GetRGB(minxmaxy, img->format, &r3, &g3, &b3);
			SDL_GetRGB(maxxmaxy, img->format, &r4, &g4, &b4);


			r = r1 * (1-fx)*(1-fy) + r2 * fx * (1-fy) + r3 * (1-fx)*fy + r4 *fx*fy;
			g = g1 * (1-fx)*(1-fy) + g2 * fx * (1-fy) + g3 * (1-fx)*fy + g4 *fx*fy;
			b = b1 * (1-fx)*(1-fy) + b2 * fx * (1-fy) + b3 * (1-fx)*fy + b4 *fx*fy;

			pixel = SDL_MapRGB(img->format, r, g, b);
			define_pixel(img, i, j, pixel);
		}
	}
	return img;
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
    Uint32 ref_pixel = SDL_MapRGB(img -> format , 42 , 42 , 42);
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

SDL_Surface* superposition(SDL_Surface* img)
{
    int i = 2;
    Uint32 pix1 , pix2 , pixel;
    for (int y = 0 ; y < img -> h ; ++y)
    {
	i = 2;
	for (int x = 0 ; x < (img -> w) - 2 ; ++x)
	{
	    pix1 = get_pixel(img , x , y);
	    pix2 = get_pixel(img , i , y);
	    if (pix1 != pix2)
	    {
		pixel = SDL_MapRGB(img->format , 255 , 255 , 255);
	    }
	    else
	    {
		pixel = SDL_MapRGB(img->format , 0 , 0 , 0);
	    }
	    define_pixel(img , x , y , pixel);
	    ++i;
	}
   }
   return img;
}

int main ()
{
	SDL_Surface* img = IMG_Load("img.bmp");
	//SDL_Surface* yes = SDL_redim(1200 , 700, img);
	SDL_Surface *ttt = grey_lvl(img);
	SDL_Surface* new = black_and_white(ttt);
	SDL_Surface* screen = SDL_SetVideoMode(1200, 700, 0, SDL_SWSURFACE);
	SDL_BlitSurface(new, NULL , screen, NULL);
	SDL_UpdateRect(screen, 0 , 0 , 1200 , 700);
	sleep(3);
	SDL_Surface *image = superposition(new);
	SDL_BlitSurface(image, NULL , screen, NULL);
	SDL_UpdateRect(screen, 0 , 0 , 1200 , 700);
	sleep(3);
	return 0;
}

