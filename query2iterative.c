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
static struct nodo* addTravelsIter(struct nodo* lista, size_t idDepar, size_t idArr, int* flag) {
    struct nodo* current = lista;
    struct nodo* prev = NULL;
    
    while (current != NULL && cmp(current->departure, idDepar) <= 0) {
        if (cmp(current->departure, idDepar) == 0 && cmp(current->arrival, idArr) == 0) {
            current->travels++;
            return lista;
        }
        prev = current;
        current = current->tail;
    }

    struct nodo* newNode = malloc(sizeof(nodo));
    if (newNode == NULL) {
        *flag = NO_MEMORY;
        return lista;
    }
    
    newNode->departure = idDepar;
    newNode->arrival = idArr;
    newNode->travels = 1;
    newNode->tail = current;
    
    if (prev != NULL) {
        prev->tail = newNode;
        return lista;
    } else {
        return newNode;
    }
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
    aux->rows = getDimRowsExcel(bikes);
    aux->firstId = addTravels(bikes, flag, aux);
    //aux->firstName = NULL;
    print(aux->firstId);
    freequery2(aux);
    return;
}
