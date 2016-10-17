#include <stdlib.h>
#include <stdio.h>

#include "chained_list.h"

typedef struct Element Element;
struct Element {

	int x ;
        int y ;
        struct Element *nxt;
	
};

typedef struct List List;
struct List{

	Element *first;
};


List* initialisation()
{
	Element *ele = malloc(sizeof(*ele));
        List *list = malloc(sizeof(*list));
        
	ele -> x = 0;
        ele -> y = 0;
        ele -> nxt = NULL;
	list -> first = ele;
        return list;        
}

void add(List *list , int coordx , int coordy)
{
	Element *new = malloc(sizeof(*new));
        new -> x = coordx;
	new -> y = coordy;

	new -> nxt = list -> first;
        list -> first = new;	
}
