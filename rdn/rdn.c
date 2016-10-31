# define _XOPEN_SOURCE 500

# include <assert.h>
# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <math.h>

double nbRandom()
{
	static double first = 0;
	if (first == 0)
	{
		srand(time(NULL));
		first = 1;
	}
	int r = rand() % 200;
	return((double)(r - 100) / 100.);
}

struct Network
{
	int nbLayers;			//nombre de couches
	int *nbNeurones;		//nombre de neurone par couche
	double *liaisons;		//valeurs des liaisons
	double *neurones;		//valeurs des neurones
	double *deltas;			//valeurs des deltas
};

struct Network * newNetwork(int* nbNeurones, int nbLayers)
{
	struct Network *net = malloc(sizeof(struct Network));
	net->nbLayers = nbLayers;
	net->nbNeurones = calloc(nbLayers, sizeof(int));

	int nbLiaisons = 0;
	int nbNeuronesTotal = nbNeurones[0];
	net->nbNeurones[0] = nbNeurones[0];
	for (int i = 1; i < nbLayers; i++)
	{
		nbLiaisons += nbNeurones[i] * nbNeurones[i - 1];
		nbNeuronesTotal += nbNeurones[i];
		net->nbNeurones[i] = nbNeurones[i];
	}

	net->liaisons = calloc(nbLiaisons, sizeof(double));
	for (int i = 0; i < nbLiaisons; i++)
	{
		net->liaisons[i] = nbRandom();
	}
	net->neurones = calloc(nbNeuronesTotal, sizeof(double));
	net->deltas = calloc(nbNeuronesTotal, sizeof(double));

	return net;
}

double sigmoide(double x)
{
	double expx = (double)(1.0 / exp(x));
	return (double)(1.0 / (1.0 + expx));
}

double getN(struct Network* net, int couche, int index)
{
	if (index >= net->nbNeurones[couche])
	{
		warnx("Depassement \" get \" neurone sur la couche %d !", couche);
	}
	int i = 0;
	for (int j = 0; j < couche; ++j)
	{
		i += net->nbNeurones[j];
	}
	return net->neurones[i + index];
}

void setN(struct Network* net, int couche, int index, double newVal)
{
	if (index >= net->nbNeurones[couche])
	{
		warnx("Depassement \" get \" neurone sur la couche %d !", couche);
	}
	int i = 0;
	for (int j = 0; j < couche; ++j)
	{
		i += net->nbNeurones[j];
	}
	net->neurones[i + index] = newVal;
}

double getD(struct Network* net, int couche, int index)
{
	if (index >= net->nbNeurones[couche])
	{
		warnx("Depassement \" get \" neurone sur la couche %d !", couche);
	}
	int i = 0;
	for (int j = 0; j < couche; ++j)
	{
		i += net->nbNeurones[j];
	}
	return net->deltas[i + index];
}

void setD(struct Network* net, int couche, int index, double newVal)
{
	if (index >= net->nbNeurones[couche])
	{
		warnx("Depassement \" get \" neurone sur la couche %d !", couche);
	}
	int i = 0;
	for (int j = 0; j < couche; ++j)
	{
		i += net->nbNeurones[j];
	}
	net->deltas[i + index] = newVal;
}

double getL(struct Network* net, int coucheN1, int indexN1, int indexN2)
{
	int couchesPrecedentes = 0;
	if (coucheN1 > 0) 
	{
		for (int i = 0; i < coucheN1; ++i)
		{
			couchesPrecedentes += net->nbNeurones[i] * net->nbNeurones[i + 1];
		}
	}
	return net->liaisons[couchesPrecedentes + net->nbNeurones[coucheN1 + 1] * 
		indexN1 + indexN2];
}

void setL
(struct Network* net, int coucheN1, int indexN1, int indexN2, double newVal)
{
	int couchesPrecedentes = 0;
	if (coucheN1 > 0) 
	{
		for (int i = 0; i < coucheN1; ++i)
		{
			couchesPrecedentes += net->nbNeurones[i] * net->nbNeurones[i + 1];
		}
	}
	net->liaisons[couchesPrecedentes + 
		net->nbNeurones[coucheN1 + 1] * indexN1 + indexN2] = newVal;
}

void display(struct Network* net)
{
	size_t len_nbN = net->nbLayers;
	printf("---------\n");
	for (int i = 0; i < net->nbNeurones[0]; i++)
	{
		printf("\033[33m(%.3f)\033[0m", getN(net, 0, i));
		printf("{");
		for (int j = 0; j < net->nbNeurones[1] - 1; ++j)
		{
			printf("%.3f | ", getL(net, 0, i, j));
		}
		printf("%.3f}", getL(net, 0, i, net->nbNeurones[1] - 1));

		printf("   ");
	}
	printf("\n\n");
	for (size_t i = 1; i < len_nbN - 1; ++i)
	{
		int nbCouche = net->nbNeurones[i];
		for (int j = 0; j < nbCouche; ++j)
		{
			printf("(%.3f)", getN(net, i, j));
			printf("{");
			for (int k = 0; k < net->nbNeurones[i + 1] - 1; ++k)
			{
				printf("%.3f | ", getL(net, i, j, k));
			}
			printf("%.3f}", getL(net, i, j, net->nbNeurones[i + 1] - 1));

			printf("   ");
		}
		printf("\n\n");
	}
	for (int i = 0; i < net->nbNeurones[len_nbN - 1]; i++)
	{
		printf("\033[36m(%.3f)\033[0m   ", getN(net, len_nbN - 1, i));
	}
	printf("\n---------\n");
}

void compute(struct Network* net, double entrees[])
{
	size_t len_nbN = net->nbLayers;
	for (int i = 0; i < net->nbNeurones[0]; ++i)
	{
		net->neurones[i] = entrees[i];
	}
	for (size_t couche = 1; couche < len_nbN; ++couche)
	{
		for (int i = 0; i < net->nbNeurones[couche]; ++i)
		{
			double sum = 0;
			for (int j = 0; j < net->nbNeurones[couche - 1]; ++j)
			{
				sum += getN(net, couche - 1, j)* getL(net, couche - 1, j, i);
			}
			setN(net, couche, i, sigmoide(sum));
		}
	}
	return;
}

double teach(struct Network* net, double entrees[], double sorties[])
{
	compute(net, entrees);
	double error = 0;
	size_t len_nbN = net->nbLayers;
	for (int i = 0; i < net->nbNeurones[len_nbN - 1]; ++i)
	{
		double neuroneVal = getN(net, len_nbN - 1, i);
		double nextError = fabs(neuroneVal - sorties[i]);
		if (nextError > error)
		{
			error = nextError;
		}
	}

	for (int i = 0; i < net->nbNeurones[len_nbN - 1]; ++i)
	{
		setD(net, len_nbN - 1, i, sorties[i] - getN(net, len_nbN - 1, i));
	}

	for (size_t i = len_nbN - 2; i > 0; --i)
	{
		for (int j = 0; j < net->nbNeurones[i]; ++j)
		{
			double valeur = getN(net, i, j);
			double deltaSum = 0;
			for (int k = 0; k < net->nbNeurones[i + 1]; ++k)
			{
				deltaSum += getD(net, i + 1, k) * getL(net, i, j, k);
			}
			setD(net, i, j, valeur * (1 - valeur) * deltaSum);
		}
	}
	for (size_t i = 0; i < len_nbN - 1; i++)
	{
		for (int j = 0; j < net->nbNeurones[i]; j++)
		{
			for (int k = 0; k < net->nbNeurones[i + 1]; k++)
			{
				double tauxA = 0.1;
				setL(net, i, j, k, getL(net, i, j, k) + tauxA*getN(net, i, j)*
					getD(net, i + 1, k));
			}
		}
	}
	return error;
}

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