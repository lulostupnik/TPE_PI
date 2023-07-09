#include "ADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#define BLOCK 500
typedef struct destino{
    char * name;
    size_t travelsTo;
    size_t travelsFrom;
}tDestino;
typedef struct matrix{
   tDestino* Travels;
   char * name;
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
Matrix  * matriz;
size_t firstRead;
}tBikeRentingCDT;


 static void matrizMalloc(Matrix * matriz, size_t dim){
    for(size_t i=0;i < dim; i++){

        matriz[i].Travels=calloc(dim,sizeof(tDestino));
        matriz[i].name="NOT TRAVELED";
        for (size_t j=0;j < dim; j++){
           matriz[i].Travels[j].name= "NOT TRAVELED";
        }
        
    }
 }

static int  binarySearch(vec arr, int left, int right, size_t target) {
    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid].stationId == target)
            return mid;

        if (arr[mid].stationId < target)
            left = mid + 1;

        else
            right = mid - 1;
    }
    return -1;
}
static void startMonthsInZero(size_t months[12]){
    for(int i=0;i<12;i++){
        months[i]=0;
    }
}
// static void fillWithZeros(Matrix *m, size_t size) {
//     for (size_t j = 0; j < size; j++) {
//         m[j].name="not traveled";
//         for (size_t i = 0; i < size; i++) {
//             m[j].Travels[i].travelsTo = 0;
//             m[j].Travels[i].travelsFrom = 0;
//             m[j].Travels[i].name = "not traveled";
//         }

// }}

static int compare_destinos(const void *a, const void *b){
     const tDestino*station1 = (const tDestino *)a;
        const tDestino* station2 = (const tDestino *)b;
       if(strcmp(station1->name,"NOT TRAVELED")==0){
        return 1;
       }
        if(strcmp(station2->name,"NOT TRAVELED")==0){
        return -1;
        }
         return strcasecmp(station1->name, station2->name);
}

static int compare_ids(const void *a, const void *b){
    const tStation *station1 = (const tStation *)a;
    const tStation *station2 = (const tStation *)b;
    return (station1->stationId-station2->stationId);
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
static int compare_matrix(const void *a, const void *b) {

    const Matrix *station1 = (const Matrix *)a;
    const Matrix *station2 = (const Matrix *)b;
    if(strcmp(station1->name,"NOT TRAVELED")==0){
        return 1;
    }
        if(strcmp(station2->name,"NOT TRAVELED")==0){
        return -1;
        }
    return strcasecmp(station1->name,station2->name);
}

void orderByids(bikeRentingADT stations){
    qsort(stations->Stations,stations->stationQty,sizeof(tStation),compare_ids);
}

bikeRentingADT newBikesRenting(void){
   return calloc(1, sizeof(tBikeRentingCDT));
}

void addStation(char *name, int id, bikeRentingADT TAD) {
    if(binarySearch(TAD->Stations,0,TAD->stationQty-1,id) == -1){ // si no encuentra la estacion la agrega
    if(TAD->stationQty % BLOCK==0){
    TAD->Stations = realloc(TAD->Stations, ((TAD->stationQty)+BLOCK) * sizeof(tStation)); 
    }
    TAD->Stations[TAD->stationQty].stationName = malloc(strlen(name) + 1);
    strcpy(TAD->Stations[TAD->stationQty].stationName, name);
    TAD->Stations[TAD->stationQty].stationId=id;
    startMonthsInZero(TAD->Stations[TAD->stationQty].Months);
    TAD->Stations[TAD->stationQty].travelsByMembers=0;
    (TAD->stationQty)++;
}}
void processData(bikeRentingADT bikes,int month,int isMember,int idStart,int idEnd){
    if(bikes->firstRead == 0){
         Matrix *matriz = calloc((bikes->stationQty + 1) , sizeof(Matrix));
        matrizMalloc(matriz,bikes->stationQty);
    orderByids(bikes);
    bikes->Stations = realloc(bikes->Stations, ((bikes->stationQty)) * sizeof(tStation));
    bikes->matriz=matriz;
    bikes->firstRead=1;
    }
    size_t newIdStart = binarySearch(bikes->Stations,0,bikes->stationQty-1,idStart);
    size_t newIdEnd  = binarySearch(bikes->Stations,0,bikes->stationQty-1,idEnd);
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

     if(bikes->matriz[newIdStart].Travels[newIdEnd].travelsTo==0 && bikes->matriz[newIdStart].Travels[newIdEnd].travelsFrom==0){
        bikes->matriz[newIdStart].Travels[newIdEnd].name=nameEnd;
         bikes->matriz[newIdStart].name=nameStart;
     }
     bikes->matriz[newIdStart].Travels[newIdEnd].travelsTo++;
     if(bikes->matriz[newIdEnd].Travels[newIdStart].travelsTo==0 && bikes->matriz[newIdEnd].Travels[newIdStart].travelsFrom==0){
       bikes->matriz[newIdEnd].Travels[newIdStart].name=nameStart;
         bikes->matriz[newIdEnd].name=nameEnd;
     }
       bikes->matriz[newIdEnd].Travels[newIdStart].travelsFrom++;
}
 void orderByName(bikeRentingADT stations){
    qsort(stations->Stations,stations->stationQty,sizeof(tStation),compare_stations); 
    qsort(stations->matriz, (stations->stationQty), sizeof(Matrix), compare_matrix);
    for(size_t  i=0;i<stations->stationQty;i++){
        qsort(stations->matriz[i].Travels,stations->stationQty,sizeof(tDestino),compare_destinos);
    }
}


void sortTravels(bikeRentingADT stations){
qsort(stations->Stations,stations->stationQty,sizeof(tStation),compare_stations); 
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
return stations->matriz[row].Travels[col].travelsTo;
}
size_t getTravelsFrom(bikeRentingADT stations,int row,int col){
return stations->matriz[row].Travels[col].travelsFrom;
}
void getTravelsByMoth(bikeRentingADT stations,int position,size_t travels[]){
    for(int i=0;i<12;i++){
    travels[i] = stations->Stations[position].Months[i];
    }
}
char* getNameFromMatrix(bikeRentingADT stations,size_t row,size_t col){
    return stations->matriz[row].Travels[col].name;
}
void freeTad(bikeRentingADT stations) {
    for (size_t i = 0; i < stations->stationQty; i++) {
        free(stations->Stations[i].stationName);
       
            free(stations->matriz[i].Travels);
        
    }
    free(stations->matriz);
    free(stations->Stations);
    free(stations);
}
