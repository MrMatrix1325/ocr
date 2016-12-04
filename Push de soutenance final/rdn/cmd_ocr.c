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

int main()
{
	char cmd[10];
	char arg[10];
	struct Network * net;
	int nbNeurones[3] = { 64, 64, 64 };
	net = newNetwork(nbNeurones, 3);
	while (strcmp(cmd, "quit"))
	{
		printf("<ocr>");
		scanf("%s", cmd);
		if (!strcmp(cmd, "load"))
		{
			printf("path : ");
			scanf("%s", arg);
			net = load(arg);
		}
		if (!strcmp(cmd, "save"))
		{
			printf("path : ");
			scanf("%s", arg);
			save(net, arg);
		}
		if (!strcmp(cmd, "display"))
		{
			display(net);
		}
		if (!strcmp(cmd, "new"))
		{
			int nbNeurones[3] = { 64, 64, 64 };
			net = newNetwork(nbNeurones, 3);
		}
		if (!strcmp(cmd, "teach"))
		{
			double tauxErreur = 1;
			int count = 0;
			while (tauxErreur > 0.54)
			{
				tauxErreur = 0;
				char path[] = "apprentissage/0/A-1.bmp";
				for (char d = '0'; d <= '3'; d++)
				{
					path[14] = d;
					path[17] = '_';
					path[18] = '1';
					for (char c = 'a'; c <= 'z'; c++)
					{
						path[16] = c;
						SDL_Surface *img = load_image(path);
						double *input = computeInput(img, img->w);
						double *output = computeOutput(c);
						double taux = teach(net, input, output);
						if (taux > tauxErreur)
						{
							tauxErreur = taux;
						}
						SDL_FreeSurface(img);
						free(input);
						free(output);
					}
					path[17] = '-';
					for (char c = 'A'; c <= 'Z'; c++)
					{
						path[16] = c;
						SDL_Surface *img = load_image(path);
						double *input = computeInput(img, img->w);
						double *output = computeOutput(c);
						double taux = teach(net, input, output);
						if (taux > tauxErreur)
						{
							tauxErreur = taux;
						}
						SDL_FreeSurface(img);
						free(input);
						free(output);
					}
					for (char c = '0'; c <= '9'; c++)
					{
						path[16] = c;
						SDL_Surface *img = load_image(path);
						double *input = computeInput(img, img->w);
						double *output = computeOutput(c);
						double taux = teach(net, input, output);
						if (taux > tauxErreur)
						{
							tauxErreur = taux;
						}
						SDL_FreeSurface(img);
						free(input);
						free(output);
					}
					path[16] = 'v';
					path[17] = 'i';
					path[18] = 'r';
					SDL_Surface *img = load_image(path);
					double *input = computeInput(img, img->w);
					double *output = computeOutput(',');
					double taux = teach(net, input, output);
					if (taux > tauxErreur)
					{
						tauxErreur = taux;
					}
					SDL_FreeSurface(img);
					free(input);
					free(output);

					path[16] = 'p';
					path[17] = 'o';
					path[18] = 'i';
					SDL_Surface *img2 = load_image(path);
					input = computeInput(img2, img2->w);
					output = computeOutput('.');
					taux = teach(net, input, output);
					if (taux > tauxErreur)
					{
						tauxErreur = taux;
					}
					free(input);
					free(output);
					SDL_FreeSurface(img2);
				}
				count++;
				if (count % 25 == 0)
				{
					printf("%d : %.3f\n", count, tauxErreur);
				}
			}
			printf("%d : %.3f\n", count, tauxErreur);
		}
		if (!strcmp(cmd, "indev"))
		{
			printf("path : ");
			scanf("%s", arg);
			SDL_Surface* img = load_image(arg);

			binarize(img);
			green(img);
			green2(img);
			printf("\n-------------\n%s\n-------------\n", readText(img, net));
			SDL_SaveBMP(img, "Test.bmp");
			SDL_FreeSurface(img);
		}
		if (!strcmp(cmd, "debug"))
		{
			printf("%c\n", 'A' + 32);
		}

	}
	return 0;
}