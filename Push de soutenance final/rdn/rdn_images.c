# include <assert.h>
# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <time.h>
# include <math.h>
# include <SDL.h>

#include "rdn.h"
#include "rdn_images.h"
#include "SDL/SDL_image.h"

static inline Uint8* pixelref(SDL_Surface *surf, unsigned x, unsigned y) {
	int bpp = surf->format->BytesPerPixel;
	return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y) {
	Uint8 *p = pixelref(surface, x, y);
	switch (surface->format->BytesPerPixel) {
	case 1:
		return *p;
	case 2:
		return *(Uint16 *)p;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
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
	switch (surface->format->BytesPerPixel) {
	case 1:
		*p = pixel;
		break;
	case 2:
		*(Uint16 *)p = pixel;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else {
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

SDL_Surface* load_image(char *path) {
	SDL_Surface          *img;
	// Load an image using SDL_image with format detection
	img = IMG_Load(path);
	if (!img)
		// If it fails, die with an error message
		errx(3, "can't load %s: %s", path, IMG_GetError());
	return img;
}

double* computeInput(SDL_Surface *img, unsigned height)
{
	double *input = malloc(height * height * sizeof(double));
	for (unsigned y = 0; y < height; y++)
	{
		for (unsigned x = 0; x < height; x++)
		{
			if (getpixel(img, x, y) == SDL_MapRGB(img->format, 0, 0, 0))
			{
				input[x * height + y] = 0.1;
			}
			else
			{
				input[x * height + y] = 0.9;
			}
		}
	}
	return input;
}

double* computeOutput(char c)
{
	double *output = malloc(64 * sizeof(double));
	for (size_t i = 0; i < 64; i++)
	{
		output[i] = 0.1;
	}
	if ('A' <= c && c <= 'Z')
	{
		output[c - 'A'] = 0.9;
		output[c - 'A' + 26] = 0.3;
	}
	if ('a' <= c && c <= 'z')
	{
		output[c - 'a' + 26] = 0.9;
		output[c - 'a'] = 0.3;
	}
	if ('0' <= c && c <= '9')
	{
		output[c - '0' + 52] = 0.9;
	}
	if (c == ',')
	{
		output[62] = 0.9;
	}
	if (c == '.')
	{
		output[63] = 0.9;
	}
	return output;
}

char getCharOutput(struct Network *net)
{
	double max = 0;
	int maxIndex = 0;
	size_t len_nbN = net->nbLayers;
	for (int i = 0; i < net->nbNeurones[len_nbN - 1]; ++i)
	{
		double neuroneVal = getN(net, len_nbN - 1, i);
		if (neuroneVal > max)
		{
			max = neuroneVal;
			maxIndex = i;
		}
	}
	if (0 <= maxIndex && maxIndex <= 25)
		return 'A' + maxIndex;

	if (26 <= maxIndex && maxIndex <= 51)
		return 'a' + maxIndex - 26;

	if (52 <= maxIndex && maxIndex <= 61)
		return '0' + maxIndex - 52;

	if (maxIndex == 62)
		return ',';

	if (maxIndex == 63)
		return '.';

	return ' ';
}

void binarize(SDL_Surface *img)
{
	for (int y = 0; y < img->h; ++y)
	{
		for (int x = 0; x < img->w; ++x)
		{
			Uint8 r;
			Uint8 g;
			Uint8 b;
			Uint32 pixel = getpixel(img, x, y);
			SDL_GetRGB(pixel, img->format, &r, &g, &b);

			pixel = (r + g + b) / 3;

			if (pixel >= (255 / 2))
			{
				putpixel(img, x, y, SDL_MapRGBA(img->format, 255, 255, 255, 255));
			}
			else
			{
				putpixel(img, x, y, SDL_MapRGBA(img->format, 0, 0, 0, 255));
			}
		}
	}
}

void green(SDL_Surface *img)
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
				putpixel(img, x, y, SDL_MapRGBA(img->format, 0, 255, 0, 255));
			}
		}
	}
}

void green2(SDL_Surface *img)
{
	for (int x = 0; x < img->w; ++x)
	{
		for (int y = 0; y < img->h; ++y)
		{
			while (y < img->h && getpixel(img, x, y) == SDL_MapRGBA(img->format, 0, 255, 0, 255))
			{
				++y;
			}
			int partialx_isfull = 1;
			Uint32 xpixel = SDL_MapRGBA(img->format, 255, 255, 255, 255);
			int y2 = y;
			for (; y < img->h && getpixel(img, x, y) != SDL_MapRGBA(img->format, 0, 255, 0, 255); ++y)
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
					putpixel(img, x, y2, SDL_MapRGBA(img->format, 0, 255, 0, 255));
				}
			}
		}
	}
}

char* readText(SDL_Surface *img, struct Network* net)
{
	//char path[] = "AAA.bmp";

	int firstChar = 1;
	int maj = 1;
	int lastX = -1;
	int moy = 0;
	char* text = malloc(50000 * sizeof(char));
	size_t indexText = 0;
	Uint32 vert = SDL_MapRGBA(img->format, 0, 255, 0, 255);

	for (int y = 0; y < img->h; ++y)
	{
		firstChar = 1;
		for (int x = 0; x < img->w; ++x)
		{
			if (getpixel(img, x, y) != vert)
			{
				if (firstChar)
				{
					firstChar = 0;
					moy = 0;
					int largeur = 0;
					int nbSpaces = 0;
					for (int spaces = x; spaces < img->w - 1; ++spaces)
					{
						if (getpixel(img, spaces, y) == vert)
						{
							largeur++;
							if (getpixel(img, spaces + 1, y) != vert)
							{
								nbSpaces++;
								moy = (moy  * (nbSpaces - 1) + largeur) / nbSpaces;
								largeur = 0;
							}
						}
					}
					moy *= 5;
				}

				int x2 = x;
				int y2 = y;
				for (; x2 < img->w && getpixel(img, x2, y) != vert; x2++);
				for (; y2 < img->h && getpixel(img, x, y2) != vert; y2++);

				y2--;
				x2--;

				int* L = malloc(4 * sizeof(int));
				L[0] = x;
				L[1] = y;
				L[2] = x2;
				L[3] = y2;
				SDL_Surface *image_decoupee = decoupe_image(img, L);

				image_decoupee = Rogne(image_decoupee);
				image_decoupee = SDL_redim(10, 10, image_decoupee);
				binarize(image_decoupee);

				double *input = computeInput(image_decoupee, image_decoupee->w);
				compute(net, input);
				char c = getCharOutput(net);

				if ((x < lastX || (x - lastX) > moy) && lastX != -1 && (c != '.' && c != ','))
				{
					text[indexText] = ' ';
					indexText++;
				}
				if (maj && 'a' <= c && c <= 'z')
				{
					c -= 32;
				}
				if (!maj && 'A' <= c && c <= 'Z')
				{
					c += 32;
				}
				lastX = x2;
				text[indexText] = c;
				indexText++;
				maj = c == '.';
				//path[2] = indexText % ('Z' - 'A') + 'A';
				//if (path[2] == 'A')
				//	path[1]++;
				//if (path[1] == 'Z' + 1)
				//{
				//	path[0]++;
				//	path[1] = 'A';
				//}
				//SDL_SaveBMP(image_decoupee, path);

				SDL_FreeSurface(image_decoupee);

				for (int i = x; i <= x2; i++)
				{
					for (int j = y; j <= y2; j++)
					{
						putpixel(img, i, j, vert);
					}
				}
				text[indexText] = '\0';
			}
		}
	}
	return text;
}

SDL_Surface* decoupe_image(SDL_Surface *img, int *L)
{
	int x_min = *(L + 0);
	int y_min = *(L + 1);
	int x_max = *(L + 2);
	int y_max = *(L + 3);
	int h = y_max - y_min;
	int w = x_max - x_min;
	SDL_Surface* res = SDL_CreateRGBSurface(0, w + 1, h + 1, 32, 0, 0, 0, 0);

	int y_img = y_min;
	for (int y_res = 0; y_res < h + 1; ++y_res)
	{
		int x_img = x_min;
		for (int x_res = 0; x_res < w + 1; ++x_res)
		{
			putpixel(res, x_res, y_res, getpixel(img, x_img, y_img));
			++x_img;
		}
		++y_img;
	}
	return res;
}

SDL_Surface* Rogne(SDL_Surface *res)
{
	int xmin = 0;
	int ymin = 0;
	int xmax = 0;
	int ymax = 0;
	Uint32 color = SDL_MapRGBA(res->format, 255, 255, 255, 255);

	int isfull = 1;
	for (int y = 0; y < res->h && isfull; y++)
	{
		for (int x = 0; x < res->w; x++)
		{
			if (getpixel(res, x, y) != color)
			{
				isfull = 0;
			}
		}
		if (!isfull)
		{
			ymin = y;
			y = res->h;
		}
	}
	isfull = 1;
	for (int x = 0; x < res->w && isfull; x++)
	{
		for (int y = 0; y < res->h; y++)
		{
			if (getpixel(res, x, y) != color)
				isfull = 0;
		}
		if (!isfull)
		{
			xmin = x;
			x = res->w;
		}
	}
	isfull = 1;
	for (int y = res->h - 1; y > -1 && isfull; y--)
	{
		for (int x = res->w - 1; x > -1; x--)
		{
			if (getpixel(res, x, y) != color)
			{
				isfull = 0;
			}
		}
		if (!isfull)
		{
			ymax = y;
			y = -1;
		}
	}
	isfull = 1;
	for (int x = res->w - 1; x > -1 && isfull; x--)
	{
		for (int y = res->h - 1; y > -1; y--)
		{
			if (getpixel(res, x, y) != color)
			{
				isfull = 0;
			}
		}
		if (!isfull)
		{
			xmax = x;
			x = -1;
		}
	}

	int *L = malloc(4 * sizeof(int));
	L[0] = xmin;
	L[1] = ymin;
	L[2] = xmax;
	L[3] = ymax;
	return decoupe_image(res, L);
}

SDL_Surface* SDL_redim(int W2, int H2, SDL_Surface *img)
{
	SDL_Surface* img_redim = NULL;
	img_redim = SDL_CreateRGBSurface(0, W2, H2, 32, 0, 0, 0, 0);

	float W = (float)img->w;
	float H = (float)img->h;
	float RX = (float)W2 / W;
	float RY = (float)H2 / H;
	for (int i = 0; i < W2; ++i)
	{
		for (int j = 0; j < H2; ++j)
		{
			float valx = i / RX;
			float valy = j / RY;

			int minx = (int)valx;
			int miny = (int)valy;
			int maxx = minx + 1;
			int maxy = miny + 1;
			double fx = valx - minx;
			double fy = valy - miny;

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


			r = r1 * (1 - fx)*(1 - fy) + r2 * fx * (1 - fy) + r3 * (1 - fx)*fy + r4 *fx*fy;
			g = g1 * (1 - fx)*(1 - fy) + g2 * fx * (1 - fy) + g3 * (1 - fx)*fy + g4 *fx*fy;
			b = b1 * (1 - fx)*(1 - fy) + b2 * fx * (1 - fy) + b3 * (1 - fx)*fy + b4 *fx*fy;

			pixel = SDL_MapRGB(img_redim->format, r, g, b);
			putpixel(img_redim, i, j, pixel);
		}
	}
	return img_redim;
}