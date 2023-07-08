#include "adt.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>

#define MONTHS 12

struct node{
    char * name;
    int nameLen;
    int tripsMem;
    int monthTrip[MONTHS];
    struct node * tail;
    int id; //Esto probablemente se puede borrar. 

    //q2;
    int idxL;
};

typedef struct node * tList;

struct idName{
    int id;
    struct node * node;
};



typedef struct bikeRentingCDT{
    struct node * first;
    struct idName * vecName;
    int dimVec;
    int cantStation;
    int flagOrdered;
    int ** matrix;
}bikeRentingCDT;


#define BLOCK 50

bikeRentingADT newTad(){
    return calloc(1, sizeof(bikeRentingCDT));
}

void freeAll(bikeRentingADT tad){
    if(tad == NULL){
        return;
    }
    tList l = tad->first;
    tList aux ;
    for(; l != NULL;){
        free(l->name);
        aux = l->tail;
        free(l);
        l=aux;
    }
    if(tad->matrix != NULL){
        for(int i=0;i<tad->cantStation;i++){
            free(tad->matrix[i]);
        }
        free(tad->matrix);
    }
  
   
    free(tad->vecName);
    free(tad);
}

static struct node * addNode(struct node * list, char * name, int * flag, struct node ** newNode){
    int c=0;
    if (list == NULL || (c=strcasecmp(list->name,name))>0){
        tList aux= malloc(sizeof(*aux));
        aux->tail=list;
        aux->nameLen = strlen(name);
        aux->name = malloc(aux->nameLen+1);
        strcpy(aux->name, name);
        *newNode = aux;
        *flag = 1;
        return aux;
    }
    if(c == 0){
        return list;
    }
    list->tail=addNode(list->tail,name,flag, newNode);
    return list;
}



static int  binarySearch(struct idName * arr, int left, int right, size_t target) {
    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid].id == target)
            return mid;

        if (arr[mid].id < target)
            left = mid + 1;

        else
            right = mid - 1;
    }
    return -1;
}
int addStation(char *name, int id, bikeRentingADT TAD){
    TAD->flagOrdered=0;
    
    if(TAD->cantStation % BLOCK == 0){
        TAD->dimVec+= BLOCK;
        TAD->vecName = realloc(TAD->vecName, sizeof(struct idName) * TAD->dimVec);
    }
    
    int flag = 0;
    TAD->first = addNode(TAD->first, name, &flag, &TAD->vecName[TAD->cantStation].node);
    if(flag == 1){
        TAD->vecName[TAD->cantStation].id = id;
        TAD->vecName[TAD->cantStation].node->id = id; // me da fiaca cambiar la f rec. 
        TAD->cantStation++;
    }/*else{
        printf("Not added %s\n", name);
    }*/
    return 1;
}

void printMatrix(bikeRentingADT adt){
    char * nameStart, * nameEnd;
    tList list_i = adt->first;
    tList list_j = adt->first;
    for(int i=0; i<adt->cantStation && list_i != NULL;i++){
        for(int j=0; j<adt->cantStation && list_j != NULL;j++){
            if(adt->matrix[i][j] != 0 || adt->matrix[j][i] != 0){
                nameStart = list_i->name;
                nameEnd = list_j->name;
                if(nameStart != NULL && nameEnd != NULL){
                    printf("%s --> %s: Cant: %d            %s --> %s: Cant: %d\n", nameStart, nameEnd, adt->matrix[i][j], nameEnd, nameStart, adt->matrix[j][i]);
                }
            }
            list_j = list_j->tail;
        }
        list_j=adt->first;
        list_i = list_i->tail;
    }
}

void printDebug(bikeRentingADT adt){
   for(int i=0; i<adt->cantStation;i++){
    for(int j=0; j<adt->cantStation;j++){
        printf("%d ",adt->matrix[i][j]);
    }
    printf("\n\n");
   }
    printMatrix(adt);
    /*printf("%s\n", adt->vecName[0].node->name);
    printf("%s\n", adt->vecName[1].node->name);
    printf("%d\n", adt->matrix[0][1]);*/
}

void printStationsById(bikeRentingADT adt){
    for(int i=0; i<adt->cantStation; i++){
        printf("ID: %d  %s\n", adt->vecName[i].id, adt->vecName[i].node->name);
    }
}

void printStations(bikeRentingADT adt){
    tList l= adt->first;
    for(;l!= NULL; l=l->tail){
        printf("%s\n", l->name);
    }
}



static int cmpfunc(const void* a, const void* b) {
    const struct idName* idNameA = (const struct idName*)a;
    const struct idName* idNameB = (const struct idName*)b;

    if (idNameA->id < idNameB->id)
        return -1;
    else if (idNameA->id > idNameB->id)
        return 1;
    else
        return 0;
}


static void orderStations(bikeRentingADT adt){
     qsort(adt->vecName,adt->cantStation,sizeof(*adt->vecName),cmpfunc);
}


static int createMatrix(bikeRentingADT adt){
    if(adt->matrix != NULL){
        return -1; // ya se creo la matrix, el front no deberia agregar estaciones.
    }

    adt->matrix = calloc(1, sizeof(int *) * adt->cantStation);
    for(int i =0; i<adt->cantStation;i++){
        adt->matrix[i] = calloc(1, sizeof(int)*adt->cantStation);
    }
    return 1;
}


static int addToMatrix(bikeRentingADT adt, int idxStart, int idxEnd){
    adt->matrix[idxStart][idxEnd]++;
    return 1;
}

int processData(bikeRentingADT adt,int month,int isMember,int idStart,int idEnd){
    if(month < 1 || month > 12){
        return -1;
    }

    if(adt->flagOrdered == 0){ // Se termino de cargar las estaciones. 
        orderStations(adt);
        createMatrix(adt);
        int i=0;
        for(struct node * l=adt->first; l!= NULL;l=l->tail){
            l->idxL=i;
            i++;
        }
        adt->flagOrdered=1;
    }
    
    int idxStart = binarySearch(adt->vecName, 0, adt->cantStation, idStart);
    int idxEnd = binarySearch(adt->vecName, 0, adt->cantStation, idEnd);
  
    if(idxStart != -1 && idxEnd != -1){
        if(isMember){
            adt->vecName[idxStart].node->tripsMem++;
        }
        adt->vecName[idxStart].node->monthTrip[month-1]++;
        if(idxEnd != idxStart){
            addToMatrix(adt, adt->vecName[idxStart].node->idxL, adt->vecName[idxEnd].node->idxL);
        }
    }
    return 1;
}


