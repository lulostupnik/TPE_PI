#include "ADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#define BLOCK 500
#define MONTHS 12

typedef struct destino{
    char * name;
    size_t travelsTo;
    size_t travelsFrom;
}tDestino;

typedef struct matrix{
   tDestino* Travels;
   char * name;
   size_t totalTravelsTo;
   size_t totalTravelsFrom;
   
}tMatrix;

typedef struct station{
    char * stationName;
    size_t stationId;
    size_t travelsByMembers;
    size_t Months[12];
}tStation;

typedef struct station * vec;

typedef enum {UNORDERED=0, ID, NAME, TRIPS} tOrder;

typedef struct bikeRentingCDT{
    vec Stations;
    size_t stationQty;
    tMatrix* matriz;
    size_t firstRead;
    tOrder order;
    size_t oldSizeOfMatriz;
}tBikeRentingCDT;

static int SearchForRepeated(vec vec,size_t size, size_t id){
    for( size_t i = 0; i < size; i++){
        if( vec[i].stationId == id){
            return 1;
        }
    }
    return -1;
}


static void  fillWithzeros(tMatrix * matriz, size_t dim,size_t * oldSize){
    for (size_t i = 0; i < *oldSize;i++){
        for (size_t j = *oldSize; j < dim;j++){
            matriz[i].Travels[j].name = NULL;
           matriz[i].Travels[j].travelsFrom = 0;
           matriz[i].Travels[j].travelsTo = 0;
           matriz[j].Travels[i].name = NULL;
           matriz[j].Travels[i].travelsFrom = 0;
           matriz[j].Travels[i].travelsTo = 0;
    }
    }
    for(size_t i=*oldSize;i<dim;i++){
        for(size_t j=*oldSize;j<dim;j++){
            matriz[i].Travels[j].name = NULL;
           matriz[i].Travels[j].travelsFrom = 0;
           matriz[i].Travels[j].travelsTo = 0;
    }
    }

}

static void matrizMalloc(tMatrix * matriz, size_t dim,int flag,size_t * oldSize){
if(flag == 1){
   for( size_t i = 0; i < dim; i++){
        matriz[i].Travels=calloc(dim,sizeof(tDestino));
        matriz[i].name = NULL;
        for( size_t j=0; j < dim; j++){
           matriz[i].Travels[j].name = NULL;
           matriz[i].Travels[j].travelsFrom = 0;
           matriz[i].Travels[j].travelsTo = 0;
        }  
    }
}else{
        for( size_t i=0; i < dim; i++){
        matriz[i].Travels=realloc(matriz[i].Travels,dim * sizeof(tDestino));
        }
        fillWithzeros(matriz,dim,oldSize);
    }
    (*oldSize)=dim;
    }


static int  binarySearch(vec vec, size_t min, size_t max, size_t Id) {
    while (min <= max) {
        size_t mid = min+(max - min)/2;
        if (vec[mid].stationId == Id)
            return mid;
        if (vec[mid].stationId < Id)
            min = mid + 1;
        else
            max = mid - 1;
    }
    return -1;
}

static void startMonthsInZero(size_t months[12]){
    for( size_t i = 0; i < MONTHS; i++){
        months[i] = 0;
    }
}

static int compare_destinos(const void *a, const void *b){
     const tDestino*station1 = (const tDestino *)a;
        const tDestino* station2 = (const tDestino *)b;
       if(station1->name== NULL){
        return 1;
       }
        if(station2->name== NULL){
        return -1;
        }
         return strcasecmp(station1->name, station2->name);
}

static int compare_ids(const void * a, const void * b){
    
    const tStation * station1 = (const tStation *) a;
    const tStation * station2 = (const tStation *) b;
    return (station1->stationId-station2->stationId);
}

static int compare_Travels(const void * a, const void * b){
    int c;
    const vec station1 = (const vec) a;
    const vec station2 = (const vec) b;
 c = station2->travelsByMembers - station1->travelsByMembers;
    if(c == 0){
    return strcasecmp(station1->stationName,station2->stationName);
    }
    return c;
}

static int compare_stations(const void * a, const void * b) {
    const vec station1 = (const vec) a;
    const vec station2 = (const vec) b;
    return strcasecmp(station1->stationName, station2->stationName);
}

static int compare_matrix(const void *a, const void *b) {

    const tMatrix *station1 = (const tMatrix *)a;
    const tMatrix *station2 = (const tMatrix *)b;
    if(  station1->name== NULL ){
        return 1;
    }
        if(  station2->name== NULL ){
        return -1;
        }
    return strcasecmp(station1->name,station2->name);
}

void orderByids(bikeRentingADT ADT){
    qsort(ADT->Stations,ADT->stationQty,sizeof(tStation),compare_ids);
    ADT->order = ID;
}

bikeRentingADT newBikesRenting(void){
   return calloc(1, sizeof(tBikeRentingCDT));
}

void addStation(char *name,size_t id,bikeRentingADT ADT) {
    ADT->firstRead=1;
    if(SearchForRepeated(ADT->Stations,ADT->stationQty,id) == -1){   // si no encuentra la estacion la agrega                   
        // if(ADT->stationQty % BLOCK == 0){
        //     ADT->Stations = realloc(ADT->Stations,(ADT->stationQty+1) * sizeof(tStation)); 
        // }
        ADT->Stations = realloc(ADT->Stations,(ADT->stationQty+1) * sizeof(tStation)); 
        ADT->Stations[ADT->stationQty].stationName = malloc(strlen(name) + 1);
        strcpy(ADT->Stations[ADT->stationQty].stationName, name);
        ADT->Stations[ADT->stationQty].stationId = id;
        startMonthsInZero(ADT->Stations[ADT->stationQty].Months);
        ADT->Stations[ADT->stationQty].travelsByMembers = 0;
        ADT->stationQty++;
    }
}
void processData(bikeRentingADT ADT,int month,int isMember,size_t idStart,size_t idEnd){
    int flag;
    if(ADT->order != ID){
         orderByids(ADT);
    }
   size_t newIdStart = binarySearch(ADT->Stations,0,ADT->stationQty-1,idStart);
    size_t newIdEnd  = binarySearch(ADT->Stations,0,ADT->stationQty-1,idEnd);
    if( newIdEnd == -1 || newIdStart == -1 ) {
        return;
    }

    if( ADT->firstRead == 1){
        if(ADT->matriz == NULL){
            flag=1;
            }else{
            flag=0;
            }
        ADT->matriz = realloc( ADT->matriz,ADT->stationQty * sizeof(tMatrix));
        matrizMalloc(ADT->matriz,ADT->stationQty,flag,&(ADT->oldSizeOfMatriz));
       
        //ADT->Stations = realloc(ADT->Stations, (ADT->stationQty) * sizeof(tStation));
        ADT->firstRead = 0;
    }
 
    if( isMember == 1){
        ADT->Stations[newIdStart].travelsByMembers++;
    }
        ADT->Stations[newIdStart].Months[month-1]++;;

    if( newIdEnd == newIdStart){
        return;
    }
    char * nameEnd = ADT->Stations[newIdEnd].stationName;
    char * nameStart = ADT->Stations[newIdStart].stationName;
    if( ADT->matriz[newIdStart].Travels[newIdEnd].travelsTo==0 && ADT->matriz[newIdStart].Travels[newIdEnd].travelsFrom == 0){
        ADT->matriz[newIdStart].Travels[newIdEnd].name=nameEnd;
        ADT->matriz[newIdStart].name=nameStart;
        ADT->matriz[newIdStart].totalTravelsTo++;
        ADT->matriz[newIdEnd].Travels[newIdStart].name = nameStart;
        ADT->matriz[newIdEnd].name = nameEnd;
    }
    ADT->matriz[newIdStart].Travels[newIdEnd].travelsTo++;
    ADT->matriz[newIdEnd].Travels[newIdStart].travelsFrom++;
}

void orderByName(bikeRentingADT ADT){
    qsort(ADT->Stations,ADT->stationQty,sizeof(tStation),compare_stations);
    qsort(ADT->matriz,ADT->oldSizeOfMatriz,sizeof(tMatrix),compare_matrix);
       for( size_t i = 0; i < ADT->oldSizeOfMatriz; i++){
        qsort(ADT->matriz[i].Travels,ADT->oldSizeOfMatriz,sizeof(tDestino),compare_destinos);
        }
    ADT->order = NAME;
}



void sortTravels(bikeRentingADT ADT){
    qsort(ADT->Stations,ADT->stationQty,sizeof(tStation),compare_Travels); 
    ADT->order = TRIPS;
}

size_t getNumberOfStations(bikeRentingADT ADT){
    return ADT->stationQty;
}

size_t getTravels(bikeRentingADT ADT,size_t position){
    return ADT->Stations[position].travelsByMembers;
}

char * getNameByPosition(bikeRentingADT ADT,size_t position){
    return ADT->Stations[position].stationName;
}

size_t getTravelsTo(bikeRentingADT ADT,size_t row,size_t col){
    return ADT->matriz[row].Travels[col].travelsTo;
}

size_t getTravelsFrom(bikeRentingADT ADT,size_t row,size_t col){
    return ADT->matriz[row].Travels[col].travelsFrom;
}

void getTravelsByMoth(bikeRentingADT ADT,size_t position,size_t travels[]){
    for( size_t i = 0; i < MONTHS; i++){
        travels[i] = ADT->Stations[position].Months[i];
    }
}

char* getNameFromMatrix(bikeRentingADT ADT,size_t row,size_t col){
    return ADT->matriz[row].Travels[col].name;
}

size_t getsizeOfMatrix(bikeRentingADT ADT){
    return ADT->oldSizeOfMatriz;
}
void freeTad(bikeRentingADT ADT) {
    for( size_t i = 0; i < ADT->stationQty; i++) {
        free(ADT->Stations[i].stationName);   
    }
    for(size_t i = 0 ;i <ADT->oldSizeOfMatriz;i++){
        free(ADT->matriz[i].Travels);
    }
    free(ADT->matriz);
    free(ADT->Stations);
    free(ADT);
}
void imprimir(bikeRentingADT ADT){
    printf("%ld\n",ADT->stationQty);
}