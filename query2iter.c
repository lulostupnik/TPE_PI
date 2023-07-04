#include <stdio.h>
#include <stdlib.h>
#include "query2.h"
#include "readDataADT.h"



typedef struct nodo {
    size_t departure;
    size_t arrival;
    //char* nameDepar;
   //char* nameArr;
    size_t travels;
    struct nodo* tail;
} nodo;

typedef struct nodo* List;

typedef struct query2CDT {
    struct nodo * firstId;
    //List firstName;
    size_t columIdDep;
    size_t columIdArr;
    size_t rows;
} query2CDT;

static int cmp(size_t e1, size_t e2) {
    if (e1 == e2)
        return 0;
    if (e1 > e2)
        return 1;

    return -1;
}
static struct nodo* addTravelsIter(struct nodo* list, size_t idDepar, size_t idArr, int* flag) {
    struct nodo* current = list;
    struct nodo* prev = NULL;
    char c;

    while (current != NULL) {
        c = cmp(current->departure, idDepar);
        if (c == 0 && cmp(current->arrival, idArr) == 0) {
            current->travels++;
            return list;
        }
        if (c > 0) {
            break;  
        }
        prev = current;
        current = current->tail;
    }

    struct nodo* aux = malloc(sizeof(struct nodo));
    if (aux == NULL) {
        *flag = NO_MEMORY;
        return list;
    }

    aux->departure = idDepar;
    aux->arrival = idArr;
    aux->travels = 1;
    aux->tail = current;

    if (prev != NULL) {
        prev->tail = aux;
    } else {
        list = aux;  
    }

    return list;
}

static List addTravels(readDataADT bikes, int* flag, query2ADT query) {
    char * aux1;
    char * aux2;
   
    for (size_t i = 1; i <query->rows; i++) {
        aux1 = getDataFromPos(bikes, i, query->columIdDep, flag);
        aux2 = getDataFromPos(bikes, i, query->columIdArr, flag);
        size_t idDepar = atoi(aux1);
        size_t idArr = atoi(aux2);
        
        if (idDepar != idArr) {
            query->firstId = addTravelsIter(query->firstId, idDepar, idArr, flag);
            
        }
        free(aux1);
        free(aux2);
    }
    
    return query->firstId;
}

static void print(List lista) {
    if (lista == NULL)
        return;
    printf("%ld\t %ld\t %ld\t\n", lista->departure, lista->arrival, lista->travels);
    print(lista->tail);
}

static void freequery2rec(List lista){
    if(lista == NULL)
        return ;
    freequery2rec(lista->tail);
    free(lista);
}
static void freequery2(query2ADT query){
    freequery2rec(query->firstId);
    free(query);
}


void newQuery(readDataADT bikes, readDataADT stations, int* flag, size_t idDep, size_t idArr) {
    query2ADT aux = calloc(1, sizeof(query2CDT));
    if (aux == NULL) {
        *flag = NO_MEMORY;
        return;
    }
    aux->columIdArr = idArr;
    aux->columIdDep = idDep;
    aux->rows = getDimRowsExcel(bikes, flag);
    aux->firstId = addTravels(bikes, flag, aux);
    //aux->firstName = NULL;
    print(aux->firstId);
    freequery2(aux);
    return;
}
