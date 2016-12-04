# define _XOPEN_SOURCE 500

# include <assert.h>
# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <math.h>

#include "rdn.h"

int main()
{
	int nbN[] = { 2,3,1 };
	struct Network *net = newNetwork(nbN, 3);

	double E1[] = { 1.0, 0.0 };
	double S1[] = { 1.0 };
	double E2[] = { 1.0,1.0 };
	double S2[] = { 0.0 };
	double E3[] = { 0.0,1.0 };
	double S3[] = { 1.0 };
	double E4[] = { 0.0,0.0 };
	double S4[] = { 0.0 };

	double erreur = 1.;
	double result = 0.;
	int count = 0;

	while (erreur > 0.4d)
	{
		erreur = teach(net, E1, S1);
		result = teach(net, E2, S2);
		if (result > erreur)
		{
			erreur = result;
		}
		result = teach(net, E3, S3);
		if (result > erreur)
		{
			erreur = result;
		}
		result = teach(net, E4, S4);
		if (result > erreur)
		{
			erreur = result;
		}
		count++;
		if (count % 50000 == 0)
		{
			printf("Taux d'erreur de l'iteration %d = %f \n", count, erreur);
		}
	}
	printf("\nNombre d'iterration = %d \n", count);
	printf("Taux d'erreur final = %f \n\n", erreur);

	compute(net, E1);
	display(net);
	compute(net, E2);
	display(net);
	compute(net, E3);
	display(net);
	compute(net, E4);
	display(net);

	return 0;
}