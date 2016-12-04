#ifndef RDN_H
#define RDN_H

struct Network
{
	int nbLayers;			//nombre de couches
	int nbLiaisons;

	int *nbNeurones;		//nombre de neurone par couche
	double *liaisons;		//valeurs des liaisons
	double *neurones;		//valeurs des neurones
	double *deltas;			//valeurs des deltas
};

double nbRandom();

double sigmoide(double x);

void display(struct Network* net);

struct Network * newNetwork(int* nbNeurones, int nbLayers);

void compute(struct Network* net, double entrees[]);

double teach(struct Network* net, double entrees[], double sorties[]);

struct Network* load(char *path);

void save(struct Network* net, char *path);

void setL
(struct Network* net, int coucheN1, int indexN1, int indexN2, double newVal);

double getL(struct Network* net, int coucheN1, int indexN1, int indexN2);

void setD(struct Network* net, int couche, int index, double newVal);

double getD(struct Network* net, int couche, int index);

void setN(struct Network* net, int couche, int index, double newVal);

double getN(struct Network* net, int couche, int index);

#endif