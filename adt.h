#ifndef adt_h
#define adt_h
typedef struct bikeRentingCDT * bikeRentingADT;
void printDebug(bikeRentingADT adt);
void printMatrix(bikeRentingADT adt);
int addStation(char *name, int id, bikeRentingADT TAD);
void printStations(bikeRentingADT adt);
bikeRentingADT newTad();
void freeAll(bikeRentingADT tad);
void printStationsById(bikeRentingADT adt);
int processData(bikeRentingADT adt,int month,int isMember,int idStart,int idEnd);
#endif