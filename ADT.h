#include <stddef.h>
#ifndef ADT_h
#define ADT_h

typedef struct bikeRentingCDT * bikeRentingADT;

bikeRentingADT newBikesRenting();

void addStation(char * name,size_t id,bikeRentingADT ADT);
void processData(bikeRentingADT ADT,int month,int isMember,size_t idStart,size_t idEnd);
void orderByName(bikeRentingADT ADT);
void sortTravels(bikeRentingADT ADT);
size_t getNumberOfStations(bikeRentingADT ADT);
size_t getTravels(bikeRentingADT ADT,size_t position);
char * getNameByPosition(bikeRentingADT ADT,size_t position);
size_t getTravelsTo(bikeRentingADT ADT,size_t row,size_t col);
size_t getTravelsFrom(bikeRentingADT ADT,size_t row,size_t col);
void  getTravelsByMoth(bikeRentingADT ADT,size_t position,size_t travels[]);
void freeTad(bikeRentingADT ADT);
char * getNameFromMatrix(bikeRentingADT ADT,size_t row,size_t col);
void imprimir(bikeRentingADT ADT);
size_t getsizeOfMatrix(bikeRentingADT ADT);
#endif
