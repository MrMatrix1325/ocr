# define _XOPEN_SOURCE 500

# include <assert.h>
# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <math.h>

/* int main()
{
  int nbN[] = {2,3,1};
  double liaisons[9];
  double neurones[6];

  for(int i=0;i<9;i++){
	*(liaisons+i)=2*nbRandom() -1;	
  }
  
  double erreur = 1;
  int count = 0;
  while(erreur > 0.0001){
	erreur = teach(nbN,liaisons,neurones,[1.,.0],[1.]);
	erreur += teach(nbN,liaisons,neurones,[1.,1.],[0.]); 
	erreur += teach(nbN,liaisons,neurones,[0.,1.],[1.]);
	erreur += teach(nbN,liaisons,neurones,[0.,.0],[0.]);
	erreur /= 4;
	if(count%5000==0){
		printf("Erreur");
	}
	count++;
  }
  printf("count = %d \n",count);
  printf("Taux d'erreur = %d \n", erreur);
  
  compute(nbN, liaisons, neurones, (0,1));
  display(nbN, liaisons, neurones);
  compute(nbN, liaisons, neurones, (0, 0));
  display(nbN, liaisons, neurones);
  compute(nbN, liaisons, neurones, (0, 1));
  display(nbN, liaisons, neurones);
  compute(nbN, liaisons, neurones, (1, 0));
  display(nbN, liaisons, neurones);
  
  return 0;
} 
*/

int nbRandom()
{
  static int first = 0;
  if(first==0){
	srand(time (NULL));
	first = 1;
  }
  return(rand());
}

double getN(int nbN[],double liaisons[],double neurones[],int couche,int index)
{
  if(index >= nbN[couche]){
	printf("Depassement \" get \" neurone sur la couche \d !", couche); /***** printf() *****/
  }
  int i = 0 ;
  for(int j = 0; j<couche;++j){
	i += nbN[j];
  }
  return neurones[i+index];
}

void setN(int nbN[],double liaisons[],double neurones[],int couche,int index,double newVal)
{
  if(index >= nbN[couche]){
        printf("Depassement \" get \" neurone sur la couche \d !", couche); /***** printf() *****/
  }
  int i = 0 ;
  for(int j = 0; j<couche;++j){
        i += nbN[j];
  }
  neurones[i+index]=newVal;
}

double getL(int nbN[],double liaisons[],double neurones[],int coucheN1,int indexN1,int indexN2)
{
  int couchesPrecedentes = 0;
  if(coucheN1 > 0){
	for(int i = 0; i < coucheN1;++i){
		couchesPrecedentes += nbN[i]*nbN[i+1];
	}
  }
  return liaisons[couchesPrecedentes + nbN[coucheN1 + 1] * indexN1 + indexN2];
}

void setL(int nbN[],double liaisons[],double neurones[],int coucheN1,int indexN1,int indexN2,double newVal)
{
  int couchesPrecedentes = 0;
  if(coucheN1 > 0){
        for(int i = 0; i < coucheN1;++i){
                couchesPrecedentes += nbN[i]*nbN[i+1];
        }
  }
  liaisons[couchesPrecedentes + nbN[coucheN1 + 1] * indexN1 + indexN2]=newVal;
}

void display(int nbN[],double liaisons[],double neurones[],int displayL)
{
  size_t len_nbN = sizeof(nbN)/sizeof(nbN[0]);
  int couche = 0;
  for(int i;i<len_nbN;++i){  /********** Boucle à printf() **********/
	printf("\n");
	for(int j = 0;j<i;++j){
		printf("( %d )",getN(nbN,liaisons,neurones,couche,j));
		if(couche<len_nbN -1 & displayL==1){
			printf("{");
			for(int k = 0; k<nbN[couche+1]-1;++k){
				printf("%d | ",getL(nbN,liaisons,neurones,couche,j,k));
			}
			printf("%d }",getL(nbN,liaisons,neurones,couche,j,nbN[couche+1]-1));
		}
		printf(" ");
	} 
	printf("\n");
	++couche;
  } /******************* Fin boucle à printf() ********************/
}


double *compute(int nbN[], double liaisons[], double neurones[], double entrees[])
{
  size_t len_nbN = sizeof(nbN)/sizeof(nbN[0]);
  for (int i = 0; i < nbN[0];++i)
  {   
         neurones[i] = entrees[i];
  }
  for(size_t couche = 1;couche < len_nbN ;++couche){
	for(int i=0;i<nbN[couche];++i){
		double sum = 0;
		for( int j=0; j < nbN[couche-1];++j){
			sum+= getN(nbN,liaisons,neurones,couche-1,j)* getL(nbN,liaisons,neurones,                                                                                       couche-1,j,i);
		}
		setN(nbN,liaisons,neurones,couche,i,sigmoide(sum));
	}
  }
  double resultats[nbN[len_nbN-1]];
  for(int k=0;k<nbN[len_nbN-1];++k){
	resultats[k]=getN(nbN,liaisons,neurones,len_nbN-1,k);
  }
  return resultats;
}

double teach(int nbN[], double liaisons[],double neurones[],double entrees[],double sorties[])
{
  double *resluts = compute(nbN,liaisons,neurones,entrees);
  double error = 0;
  size_t len_nbN = sizeof(nbN)/sizeof(nbN[0]);
  for(int i = 0; i< nbN[len_nbN-1];++i){
  	double test = getN(nbN, liaisons, neurones, len_nbN - 1, i) - sorties[i];
	if(test<0){
		error += -(test);
	}else{
		error += test;
	}
  }
  error /= nbN[len_nbN-1];
  double delta[len_nbN];
  for(int j = 0; j<nbN[len_nbN-1];++j){
	setN(nbN,liaisons,delta,len_nbN-1,j,sorties[j]-getN(nbN, liaisons, neurones, len_nbN-1,j));
  }
  for(int k = len_nbN-2;k>0;--k){
	for(int l = 0; l < nbN[k];++l){
		double valeur = getN(nbN,liaisons,neurones,k,l);
		double deltaSum = 0;
		for(int m = 0;m<nbN[k+1];++m){
			double tauxA = 0.3;
			setL(nbN,liaisons,neurones,k,l,m,getL(nbN,liaisons,neurones,k,l,m)+ tauxA*getN(nbN,liaisons,neurones,k,l)*getN(nbN,liaisons,delta,k+1,m));
		}
	}
  }
  return error;
}


double sigmoide(double x)
{
  double expx =  (double)(1.0/exp(x));
  double un = (double)1.0;
  double inverse = (double)(un/(un+expx));
  return (inverse);
}

