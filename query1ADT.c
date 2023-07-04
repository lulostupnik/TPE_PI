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
    readDataADT stationExcel;
    readDataADT bikesExcel;
};

static int addTrip(query1ADT q1, size_t stationId);

void printTripsID(query1ADT q1){
 
   printf("Ordenado por ID: \n");
    for(tList l = q1->firstId; l!=NULL; l=l->tail){
        printf("Stration Id %ld, Station name: %s, cantTrips %ld\n",l->stationId, l->name, l->cantTrips);
    }
}


void printTripsNAME(query1ADT q1){
    printf("Ordenado por Nombre: \n");
    for(tList l = q1->firstTrip; l!=NULL; l=l->tail){
        printf("2:Stration Id %ld, Station name: %s, cantTrips %ld\n",l->stationId, l->name, l->cantTrips);
    }
}

query1ADT newQuery1(readDataADT bikesExcel, readDataADT stationExcel,int * flag){
    query1ADT ans = calloc(1, sizeof(*ans));
    if(ans == NULL){
        *flag = NO_MEMORY;
    }else{
        *flag = 0;
    }
    ans->bikesExcel = bikesExcel;
    ans->stationExcel = stationExcel;
    return ans;
}

int processDataQ1(query1ADT q1, size_t bikesIdCol, size_t stationIdCol, size_t stationNameCol){
    int flag = processCantTrips(q1, bikesIdCol);
    if(flag != OK){
        //cosas
        return flag;
    }
    int rowsStation = getDimRowsExcel(q1->stationExcel, &flag);
    if(flag != OK){
        //cosas
        return flag;
    }
    char * auxName, * auxStationId;
    int i;
    for(i=1; i<rowsStation-1; i++){
        auxName = getDataFromPos(q1->stationExcel, i, stationNameCol, &flag);
        if(flag != OK){
        //cosas
        return flag;
        }
        auxStationId = getDataFromPos(q1->stationExcel, i, stationIdCol, &flag);
        if(flag != OK){
        //cosas
        return flag;
        }
        addName(q1, auxName, atoi(auxStationId));
        free(auxStationId); // no le hago free a auxName xq uso ese puntero
    }
    flag = orderByCantTrip(q1);
    if(flag != OK){
        //
    }
    return OK;
}

int processCantTrips(query1ADT q1, size_t bikesIdCol){
    int flag = OK;
    int * flagPtr = &flag;
    /*size_t colsBikes = getDimColsExcel(q1->bikesExcel, flagPtr);
    if(flag != OK){
        return flag;
    }*/
    size_t rowsBikes = getDimRowsExcel(q1->bikesExcel, flagPtr);
    if(flag != OK){
        return flag;
    }
   
    //Agrego todos lo viajes
    char * aux;
    for(int i=1; i<rowsBikes-1; i++){
        aux = getDataFromPos(q1->bikesExcel, i, bikesIdCol, &flag);
       
        if(flag != OK){
            //free de los addtrip y eso
            free(aux);
            return flag;
        }
        flag = addTrip(q1, atoi(aux));
        free(aux);
        if(flag != OK){
            return flag;
        }
    }
   
    return flag;
}

static tList addTripRec(tList list, size_t stationId, int * flag){
    if(list == NULL || stationId < list->stationId){
        tList aux = calloc(1, sizeof(*aux));
        if(aux == NULL){
            *flag = NO_MEMORY;
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



static int addTrip(query1ADT q1, size_t stationId){
    if(q1 == NULL){
        return NULL_POINTER;
    }
    int flag = 0;
  
    q1->firstId = addTripRec(q1->firstId, stationId, &flag);
    return flag;
}


int addNamesFromVec(query1ADT q1, char ** vec, size_t dim){
    int flag = 0;
  
    for(tList l= q1->firstId;l!= NULL; l=l->tail){
            if(l->name == NULL){
                if(l->stationId >= dim){
                    //
                    return INVALID_ID;
                }
                l->name = copyString(vec[l->stationId], &l->len, &flag);
                if(flag != OK){
                    //cosas
                     return flag;
                }
            }else{
                // cosas !
                return ALREADY_ADDED;
            }
        }
    return OK;
}

int addName(query1ADT q1, char * name,/* size_t len,*/ size_t stationId){ 
    if(q1 == NULL){
        return NULL_POINTER;
    }
    for(tList l= q1->firstId;l!= NULL && l->stationId <= stationId ; l=l->tail){
        if(l->stationId == stationId){
            if(l->name == NULL){
                l->name = name;
               /* l->name = malloc(len+1);
                if(l->name == NULL){
                     return NO_MEMORY;
                }
                strcpy(l->name, name);*/
                return OK;
            }else{
                //free(name)?;
                return ALREADY_ADDED;
            }
        }
    }
    return INVALID_ID;
}





static tList addListRec(tList list, char * name, size_t len, size_t stationId, size_t cantTrips, int * flag){
    if(list == NULL || cantTrips > list->cantTrips || (cantTrips == list->cantTrips && strcmp(name, list->name) < 0)){
        tList aux = malloc(sizeof(*aux));
        if(aux == NULL){
            *flag = NO_MEMORY;
            return list;
        }
        aux->stationId = stationId;
        aux->name = name;
        aux->cantTrips = cantTrips;
        aux->len = len;
        aux->tail = list;
        *flag = 0;
        return aux;
    }
    if(cantTrips <= list->cantTrips ){
        list->tail = addListRec(list->tail, name, len, stationId, cantTrips, flag);
    }
    return list;
}



int orderByCantTrip(query1ADT q1){
    int flag = OK;
    for(tList l= q1->firstId;l!= NULL; l=l->tail){
       
        if(l->name == NULL){
            // lo que sea
            //se hubiera accedido a NULL en el strcmp
            return NULL_POINTER;
        }
        q1->firstTrip =addListRec(q1->firstTrip, l->name, l->len, l->stationId, l->cantTrips, &flag); // asumo que ya se agrego el nombre y la len
        
        if(flag == NO_MEMORY){
            //agregar
            return NO_MEMORY;
        }
      
    }
    return flag;
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


