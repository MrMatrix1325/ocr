# include "image_operation.h"
# include <err.h>
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>

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

void wait_for_keypressed(void) {
  SDL_Event             event;
  for (;;) {
    SDL_PollEvent( &event );
    switch (event.type) {
    case SDL_KEYDOWN: return;
    default: break;
    }
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


SDL_Surface *grey_lvl(SDL_Surface* img)
{
  Uint8 r , g , b;
  Uint32 pixel, newpixel;
  for (int y = 0 ; y < img -> h ; y++)
  {
    for (int x = 0 ; x < img -> w ; x++)
    {
      pixel = getpixel(img , x , y);
      SDL_GetRGB(pixel , img->format , &r , &g , &b);
      float mid = ((0.3 * r  + 0.59 * g + 0.11 * b) /3);
      newpixel = SDL_MapRGB(img -> format , mid , mid  , mid );
      putpixel(img , x , y , newpixel);
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
      pixel = getpixel(img , x , y);
      if (pixel > ref_pixel)
      pix = SDL_MapRGB(img -> format , 255, 255 , 255);
      else
      pix = SDL_MapRGB(img -> format , 0 , 0 , 0);
      putpixel(img , x , y , pix);
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
	    pix1 = getpixel(img , x , y);
	    pix2 = getpixel(img , i , y);
	    if (pix1 != pix2)
	    {
		pixel = SDL_MapRGB(img->format , 255 , 255 , 255);
	    }
	    else
	    {
		pixel = SDL_MapRGB(img->format , 0 , 0 , 0);
	    }
	    putpixel(img , x , y , pixel);
	    ++i;
	}
   }
   return img;
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

      Uint8 r , g , b , r1 , g1 , b1, r2 , g2 , b2;
      Uint32 pixel = getpixel(img, i, j);
      SDL_GetRGB(pixel, img->format, &r, &g, &b);

      Uint32 minxminy = getpixel(img, minx, miny);
      Uint32 minxmaxy = getpixel(img, minx, maxy);
      Uint32 maxxminy = getpixel(img, maxx, miny);
      Uint32 maxxmaxy = getpixel(img, maxx, maxy);

      Uint8 r3, g3 , b3 , r4, g4, b4;
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

SDL_Surface* green(SDL_Surface *img)
{
  for (int y = 0; y < img->h; ++y)
  {
    int y_isfull = 1;
    Uint32 ypixel = getpixel(img, 0, y);
    for (int x = 0; x < img->w; ++x)
    {
      Uint32 xpixel = getpixel(img, x, y);
      if (xpixel != ypixel)
      {
	y_isfull = 0;
	break;
      }
    }
    if (y_isfull == 1)
    {
      for (int x = 0; x < img->w; ++x)
      {
	putpixel(img, x, y, SDL_MapRGBA(img->format, 0, 255, 0,255));
      }
    }

  }
  return img;
}

SDL_Surface* green2(SDL_Surface *img)
{
  for (int x = 0; x < img->w; ++x)
  {
    for (int y = 0; y < img->h; ++y)
    {
      Uint32 pix = SDL_MapRGBA(img->format, 0, 255, 0,255);
      while(y < img->h && getpixel(img, x, y) == pix)
      {
        ++y;
      }
      int partialx_isfull = 1;
      Uint32 xpixel = SDL_MapRGB(img->format, 0, 0, 0);
      int y2 = y;
      for (;y < img->h && getpixel(img, x, y) != pix; ++y)
      {
        Uint32 ypixel = getpixel(img, x, y);
	if (xpixel != ypixel)
	{
	  partialx_isfull = 0;
	}
      }
      if (partialx_isfull == 1)
      {
	for (; y2 < y; ++y2)
        {
	  putpixel(img, x, y2, SDL_MapRGBA(img->format, 0, 255, 0,255));
	}
      }
    }
  }
  return img;
}

void character_block(SDL_Surface *img)
{
  int *L = malloc(4 * sizeof (int));
  *(L + 0) = 2147483647;
  *(L + 1) = 2147483647;
  *(L + 2) = 0;
  *(L + 3) = 0;

  int startx = 0;
  int starty = 0;

  int is_finish = 0;
  int i = 0;
  Uint32 vert = SDL_MapRGBA(img->format, 0, 255, 0, 255);
  Uint32 blanc = SDL_MapRGBA(img->format, 255, 255, 255, 255);
  while (is_finish == 0)
  {
    is_finish = 1;
    for (int y = 0; y < img->h; ++y)
    {
      for (int x = 0; x < img->w; ++x)
      {
        Uint32 pixel = getpixel(img, x, y);
        if (pixel == blanc && is_finish == 1)
        {
          startx = x;
          starty = y;
          is_finish = 0;
        }
      }
    }
  *(L + 0) = 2147483647;
  *(L + 1) = 2147483647;
  *(L + 2) = 0;
  *(L + 3) = 0;
  L = Find_min_and_max_pixels(img, startx, starty, L);
  SDL_Surface* image_decoupee = cut_image(img, L);
  int x_filename = *(L + 0);
  int y_filename = *(L + 1);
  while(y_filename > 0 && getpixel(img, x_filename, y_filename) != vert)
  {
    --y_filename;
  }
  ++y_filename;

  char* filename = malloc((10) * sizeof(char));
  *(filename + 0) = '(';
  *(filename + 1) = (char)(y_filename);
  *(filename + 2) = ',';
  *(filename + 3) = (char)(x_filename);
  *(filename + 4) = ')';
  *(filename + 5) = '.';
  *(filename + 6) = 'b';
  *(filename + 7) = 'm';
  *(filename + 8) = 'p';
  *(filename + 9) = '\0';
  SDL_SaveBMP(image_decoupee, filename);
  SDL_FreeSurface(image_decoupee);
  ++i;
  }
}

int* Find_min_and_max_pixels(SDL_Surface *img, int x, int y, int *L)
{
  if (x <  *(L + 0))
    *(L + 0) = x;
  if (y <  *(L + 1))
    *(L + 1) = y;
  if (x >  *(L + 2))
    *(L + 2) = x;
  if (y >  *(L + 3))
    *(L + 3) = y;

  putpixel(img, x, y, SDL_MapRGBA(img->format, 255, 255, 254,255));
  Uint32 pix = SDL_MapRGBA(img->format, 255, 255, 255, 255);
  if (y + 1 < img->h && getpixel(img, x, y + 1) == pix)
      	Find_min_and_max_pixels(img, x, y + 1, L);
  if (x + 1 < img->w && getpixel(img, x + 1 , y) == pix)
       	Find_min_and_max_pixels(img, x + 1, y, L);
  if (y - 1 > 0 && getpixel(img, x, y - 1) == pix)
       	Find_min_and_max_pixels(img, x, y - 1, L);
  if (x - 1 > 0 && getpixel(img, x - 1, y) == pix)
       	Find_min_and_max_pixels(img, x - 1, y, L);
  return L;
}

SDL_Surface* cut_image(SDL_Surface *img, int *L)
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

