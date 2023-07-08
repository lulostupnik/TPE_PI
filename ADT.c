#include "ADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#define BLOCK 500

typedef struct matrix{
    char * name;
    size_t travelsTo;
    size_t travelsFrom;
}Matrix;
typedef struct station{
char * stationName;
size_t stationId;
size_t travelsByMembers;
size_t Months[12];
}tStation;

typedef struct station * vec;

typedef struct bikeRentingCDT{
vec Stations;
size_t stationQty;
size_t maxId;
Matrix  ** matriz;
size_t firstRead;
}tBikeRentingCDT;

static int  Search(vec arr, int left, int right, size_t target) {
for(int i =0 ;i< right;i++){
if(arr[i].stationId==target)
return i;
}
return -1;
}

static void fillWithZeros(Matrix **m, size_t size) {
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            m[i][j].travelsTo = 0;
            m[i][j].travelsFrom=0;
        }
    }
}

static int compare_Travels(const void *a, const void *b){
    const vec station1 = (const vec )a;
    const vec station2 = (const vec )b;

    return station2->travelsByMembers - station1->travelsByMembers;
}
static int compare_stations(const void *a, const void *b) {
    const vec station1 = (const vec )a;
    const vec station2 = (const vec )b;

    return strcasecmp(station1->stationName, station2->stationName);
}
bikeRentingADT newBikesRenting(void){
   return calloc(1, sizeof(tBikeRentingCDT));
}

void addStation(char *name, int id, bikeRentingADT TAD) {
    if(TAD->stationQty % BLOCK==0){
    TAD->Stations = realloc(TAD->Stations, ((TAD->stationQty)+BLOCK) * sizeof(tStation)); 
    }
    TAD->Stations[TAD->stationQty].stationName = malloc(strlen(name) + 1);
    strcpy(TAD->Stations[TAD->stationQty].stationName, name);
    TAD->Stations[TAD->stationQty].stationId=id;
    if(id > TAD->maxId){
        TAD->maxId=id;
    }
    (TAD->stationQty)++;
}
void processData(bikeRentingADT bikes,int month,int isMember,int idStart,int idEnd){
    if(bikes->firstRead == 0){
         Matrix **matriz = malloc(((bikes->stationQty) + 1) * sizeof(Matrix *));
    for (size_t i = 0; i < bikes->stationQty; i++) {
        matriz[i] = malloc(((bikes->stationQty) + 1) * sizeof(Matrix));
    }
    fillWithZeros(matriz, (bikes->stationQty ));
    orderByName(bikes);
    bikes->Stations = realloc(bikes->Stations, ((bikes->stationQty)) * sizeof(tStation));
    bikes->matriz=matriz;
    bikes->firstRead=1;
    }
    size_t newIdStart = Search(bikes->Stations,0,bikes->stationQty,idStart);
    size_t newIdEnd  = Search(bikes->Stations,0,bikes->stationQty,idEnd);
    if(newIdEnd == -1 || newIdStart == -1 ) {
        return;
    }

    if(isMember == 1){
        bikes->Stations[newIdStart].travelsByMembers++;
    }
    bikes->Stations[newIdStart].Months[month-1]++;;

    if(newIdEnd==newIdStart){
        return;
    }
    char * nameEnd = bikes->Stations[newIdEnd].stationName;
    char * nameStart=bikes->Stations[newIdStart].stationName;

     if(bikes->matriz[newIdStart][newIdEnd].travelsTo==0 && bikes->matriz[newIdStart][newIdEnd].travelsFrom==0){
        bikes->matriz[newIdStart][newIdEnd].name=nameEnd;
     }

     bikes->matriz[newIdStart][newIdEnd].travelsTo++;
     if(bikes->matriz[newIdEnd][newIdStart].travelsFrom==0 && bikes->matriz[newIdEnd][newIdStart].travelsTo==0){
        bikes->matriz[newIdEnd][newIdStart].name=nameStart;
     }
       bikes->matriz[newIdEnd][newIdStart].travelsFrom++;  
}
 void orderByName(bikeRentingADT stations){
    qsort(stations->Stations,stations->stationQty,sizeof(tStation),compare_stations); 
}


void sortTravels(bikeRentingADT stations){
qsort(stations->Stations,stations->stationQty,sizeof(tStation),compare_Travels); 
}

size_t getNumberOfStations(bikeRentingADT stations){
    return stations->stationQty;
}
size_t getTravels(bikeRentingADT stations,int position){
    return stations->Stations[position].travelsByMembers;
}
char * getNameByPosition(bikeRentingADT stations,int position){
    return stations->Stations[position].stationName;
}
size_t getTravelsTo(bikeRentingADT stations,int row,int col){
return stations->matriz[row][col].travelsTo;
}
size_t getTravelsFrom(bikeRentingADT stations,int row,int col){
return stations->matriz[row][col].travelsFrom;
}
void getTravelsByMoth(bikeRentingADT stations,int position,size_t travels[]){
    for(int i=0;i<12;i++){
    travels[i] = stations->Stations[position].Months[i];
    }
}
void freeTad(bikeRentingADT stations){
for(int i=0;i<stations->stationQty;i++){
    free(stations->Stations[i].stationName);
     free(stations->matriz[i]);
}
  free(stations->matriz);
  free(stations->Stations);
  free(stations);
}
