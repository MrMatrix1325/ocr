// pixel_operations.c
 
// Simple get/put pixel for SDL
// Inspired by code from SDL documentation
// (http://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideo.html)
 
# include "pixel_operations.h"
 
static inline
Uint8* pixelref(SDL_Surface *surf, unsigned x, unsigned y) {
  int bpp = surf->format->BytesPerPixel;
  return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}
 
Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y) {
  Uint8 *p = pixelref(surface, x, y);
  switch(surface->format->BytesPerPixel) {
  case 1:
    return *p;
  case 2:
    return *(Uint16 *)p;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
  case 4:
    return *(Uint32 *)p;
  }
  return 0;
}
 
void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel) {
  Uint8 *p = pixelref(surface, x, y);
  switch(surface->format->BytesPerPixel) {
  case 1:
    *p = pixel;
    break;
  case 2:
    *(Uint16 *)p = pixel;
    break;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      p[0] = (pixel >> 16) & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = pixel & 0xff;
    } else {
      p[0] = pixel & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = (pixel >> 16) & 0xff;
    }
    break;
  case 4:
    *(Uint32 *)p = pixel;
    break;
  }
}

SDL_Surface* SDL_redim(int W2, int H2, SDL_Surface *img) 
{
	SDL_Surface* img_redim = NULL;
	img_redim = SDL_CreateRGBSurface(0, W2, H2, 32, 0, 0, 0, 0);

	float W = (float)img->w;
	float H = (float)img->h;
	float RX = (float)W2/W;
	float RY = (float)H2/H;
	for (int i = 0; i < W2; ++i)
	{
		for (int j = 0; j < H2; ++j)
		{
			float valx = i/RX;
			float valy = j/RY;

			int minx = (int)valx;
			int miny = (int)valy;
			int maxx = minx+1;
			int maxy = miny+1;
			double fx = valx-minx;
			double fy = valy-miny; 
 
			Uint8 r;
			Uint8 g;
			Uint8 b;
			Uint32 pixel = getpixel(img, i, j);
			SDL_GetRGB(pixel, img->format, &r, &g, &b);

			Uint32 minxminy = getpixel(img, minx, miny);
			Uint32 minxmaxy = getpixel(img, minx, maxy);
			Uint32 maxxminy = getpixel(img, maxx, miny);
			Uint32 maxxmaxy = getpixel(img, maxx, maxy);

			Uint8 r1;
			Uint8 g1;
			Uint8 b1;

			Uint8 r2;
			Uint8 g2;
			Uint8 b2;

			Uint8 r3;
			Uint8 g3;
			Uint8 b3;

			Uint8 r4;
			Uint8 g4;
			Uint8 b4;
			
			SDL_GetRGB(minxminy, img->format, &r1, &g1, &b1);
			SDL_GetRGB(maxxminy, img->format, &r2, &g2, &b2);
			SDL_GetRGB(minxmaxy, img->format, &r3, &g3, &b3);
			SDL_GetRGB(maxxmaxy, img->format, &r4, &g4, &b4);
			

			r = r1 * (1-fx)*(1-fy) + r2 * fx * (1-fy) + r3 * (1-fx)*fy + r4 *fx*fy;
			g = g1 * (1-fx)*(1-fy) + g2 * fx * (1-fy) + g3 * (1-fx)*fy + g4 *fx*fy;
			b = b1 * (1-fx)*(1-fy) + b2 * fx * (1-fy) + b3 * (1-fx)*fy + b4 *fx*fy;

			pixel = SDL_MapRGB(img_redim->format, r, g, b);
			putpixel(img_redim, i, j, pixel);
		}
	}
	return img_redim;
}

SDL_Surface* Bloc_de_caractere(SDL_Surface *img)
{
	// (startx, starty) -> the firts white pixel
        int startx = 0;
        int starty = 0;
	int bool = 1;
        for (int y = 0; y < img->h; ++y)
        {
            	for (int x = 0; x < img->w; ++x)
        	{
                	Uint32 pixel = getpixel(img, x, y);
                    	if (pixel == SDL_MapRGBA(img->format, 255, 255, 255, 255) && bool == 1)
                    	{
                        	startx = x;
                        	starty = y;
				bool = 0;
                    	}

                }
	}

	// L -> list of all white pixels already visited from L[2] 
	// L[0] = min_pixel && L[1] = max_pixel 
	
	//We suppose that we have less than 1000 pixels in our bloc
	int *L = malloc(4 * sizeof (int));
	//L[0]
	*(L + 0) = 2147483647;
	*(L + 1) = 2147483647;
	//L[1]
	*(L + 2) = 0;
	*(L + 3) = 0;

	L = Find_min_and_max_pixels(img, startx, starty, L);

	SDL_Surface* image_decoupee = decoupe_image(img, L);
        return image_decoupee;
}

int* Find_min_and_max_pixels(SDL_Surface *img, int x, int y, int *L)
{
	//*(L+ (2 * len_L)) = x;
	//*(L+ (2 * len_L +1)) = y;

	if (x <  *(L + 0))
		*(L + 0) = x;
	if (y <  *(L + 1))
		*(L + 1) = y;
	if (x >  *(L + 2))
		*(L + 2) = x;
	if (y >  *(L + 3))
		*(L + 3) = y;
            
	putpixel(img, x, y, SDL_MapRGBA(img->format, 255, 255, 254,255));

        if (y + 1 < img->h && getpixel(img, x, y + 1) == SDL_MapRGBA(img->format, 255, 255, 255, 255))
            	Find_min_and_max_pixels(img, x, y + 1, L);
        if (x + 1 < img->w && getpixel(img, x + 1 , y) == SDL_MapRGBA(img->format, 255, 255, 255, 255))
        	Find_min_and_max_pixels(img, x + 1, y, L);
        if (y - 1 > 0 && getpixel(img, x, y - 1) == SDL_MapRGBA(img->format, 255, 255, 255, 255))
        	Find_min_and_max_pixels(img, x, y - 1, L);
        if (x - 1 > 0 && getpixel(img, x - 1, y) == SDL_MapRGBA(img->format, 255, 255, 255, 255))
        	Find_min_and_max_pixels(img, x - 1, y, L);
        return L;
}

SDL_Surface* decoupe_image(SDL_Surface *img, int *L)
{
	int x_min = *(L + 0);
	int y_min = *(L + 1);
	int x_max = *(L + 2);
	int y_max = *(L + 3);
	int h = y_max - y_min;
	int w = x_max - x_min;
	SDL_Surface* res = SDL_CreateRGBSurface(0, w+1, h+1, 32, 0, 0, 0, 0);

	int y_img = y_min;
	for (int y_res = 0; y_res < h+1; ++y_res)
	{
		int x_img = x_min;
		for (int x_res = 0; x_res < w+1; ++x_res)
		{
			putpixel(res, x_res, y_res, getpixel(img, x_img, y_img));
			++x_img;
		}
		++y_img;
	}
	return res;
	
}
