#include "query1ADT.h"
#include <stdio.h>

struct node{
    size_t stationId;
    char * name;
    size_t len;
    size_t cantTrips;
    struct node * tail;
};

typedef struct node * tList;

struct query1CDT{
    tList firstTrip;
    tList iteratorTrip;
    tList firstId;
  
};


void printTrips(query1ADT q1){
    for(tList l = q1->firstId; l!=NULL; l=l->tail){
        printf("Station id: %ld, cantTrips %ld\n", l->stationId, l->cantTrips);
    }
}


query1ADT newQuery1(int * flag){
    query1ADT ans = calloc(1, sizeof(*ans));
    if(ans == NULL){
        *flag = ENOMEM;
    }else{
        *flag = 0;
    }
    return ans;
}


static tList addTripRec(tList list, size_t stationId, int * flag){
    if(list == NULL || stationId < list->stationId){
        tList aux = calloc(1, sizeof(*aux));
        if(aux == NULL){
            *flag = ENOMEM;
            return list;
        }
        //Dejo name y len en 0
        aux->cantTrips = 1;
        aux->stationId = stationId;
        aux->tail = list;
        return aux;
    }
    if(stationId == list->stationId){
        list->cantTrips++;
    }else{
        list->tail = addTripRec(list->tail, stationId, flag);
    } 
    return list;
}



int addTrip(query1ADT q1, size_t stationId){
    if(q1 == NULL){
        return NULL_POINTER;
    }
    int flag = 0;
    q1->firstId = addTripRec(q1->firstId, stationId, &flag);
    return flag;
}

int addName(query1ADT q1, char * name, size_t len, size_t stationId){ 
    if(q1 == NULL){
        return NULL_POINTER;
    }
    for(tList l= q1->firstId;l!= NULL && l->stationId >= stationId ; l=l->tail){
        if(l->stationId == stationId){
            if(l->name == NULL){
                l->name = malloc(len+1);
                if(l->name == NULL){
                     return ENOMEM;
                }
                strcpy(l->name, name);
                return 0;
            }else{
                return ALREADY_ADDED;
            }
        }
    }
    return INVALID_ID;
}



static tList addListRec(tList list, char * name, size_t len, size_t cantTrips, int * flag){
    if(list == NULL || cantTrips < list->cantTrips || (cantTrips == list->cantTrips && strcmp(name, list->name) < 0)){
        tList aux = malloc(sizeof(*aux));
        if(aux == NULL){
            *flag = ENOMEM;
            return list;
        }
        aux->name = name;
        aux->cantTrips = cantTrips;
        aux->len = len;
        aux->tail = list;
        *flag = 0;
        return aux;
    }
    if(cantTrips > list->cantTrips ){
        list->tail = addListRec(list->tail, name, len, cantTrips, flag);
    }
    return list;
}



int orderByCantTrip(query1ADT q1){
    int flag = 0;
    for(tList l= q1->firstId;l!= NULL; l=l->tail){
        q1->firstTrip =addListRec(q1->firstTrip, l->name, l->len, l->cantTrips, &flag); // asumo que ya se agrego el nombre y la len
        free(l);
    }
}



void toBegin(query1ADT query1Adt){
    query1Adt->iteratorTrip= query1Adt->firstTrip;
}

int hasNext(const query1ADT query1Adt){
    return query1Adt->iteratorTrip != NULL;
}

int next(query1ADT query1Adt, char ** name, size_t * len, size_t * cantTrips){
    if(!hasNext(query1Adt)){
        return NULL_POINTER;
    }
    *name = query1Adt->iteratorTrip->name; // Le doy el puntero del ADT. Solo se usara una vez. Be careful
    *len = query1Adt->iteratorTrip->len;
    *cantTrips = query1Adt->iteratorTrip->cantTrips;
    *cantTrips=query1Adt->iteratorTrip->cantTrips;
    query1Adt->iteratorTrip = query1Adt->iteratorTrip->tail;
    return 0;
}



/*
void freeQuery1Resources(query1ADT query1Adt){
    return;
}

query1ADT newQuery1(size_t * flag){
    query1ADT ans = calloc(1, sizeof(*ans));
    if(ans == NULL){
        *flag = ENOMEM;
    }else{
        *flag = 0;
    }
    return ans;
}

static tList addListRec(tList list, char * name, size_t len, size_t cantTrips, int * flag){
    if(list == NULL || cantTrips < list->cantTrips || (cantTrips == list->cantTrips && strcmp(name, list->name) < 0)){
        tList aux = malloc(sizeof(*aux));
        if(aux == NULL){
            *flag = ENOMEM;
            return list;
        }
        aux->name = malloc(len + 1);
        if(aux->name == NULL){
            *flag = ENOMEM;
            return list;
        }
        strcpy(aux->name, name);
        aux->cantTrips = cantTrips;
        aux->len = len;
        aux->tail = list;
        *flag = 0;
        return aux;
    }
    if(cantTrips > list->cantTrips ){
        list->tail = addListRec(list->tail, name, len, cantTrips, flag);
    }
    return list;
}


int addStation(query1ADT query1Adt, char * name, size_t len, size_t cantTrips){
    if(query1Adt == NULL){
        return NULL_POINTER;
    }
    int flag;
    query1Adt->first = addListRec(query1Adt->first, name, len, cantTrips, &flag);
    return flag;
}

}*/