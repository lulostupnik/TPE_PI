#include "Query3.h"
#include "readDataADT.h"
#include <stdio.h>
#define MONTHS 12 

typedef struct node{
    size_t stationId; //Id de la estacion 
    int months[MONTHS]; //Vector de meses donde voy a guardar la cant de viajes de cada mes. 
    char * name; //Nombre de la estacion 
    struct node * tail; //Nodo al siguiente 
}node; 

typedef node * List; 

typedef struct Query3CDT{
    List firstStatName; //Puntero al primer nodo de la lista con los nombres de las estaciones 
    List firstStatId; //Puntero al primer nodo de la lista con los id de las estaciones 
    size_t columDepDate; //Tamaño de columna de la fecha de salida 
    size_t columIdDep; //Tamaño columna de ID de partidas 
    size_t rows; //Tamaño de filas 
}Query3CDT;

static List addStatMonths(List list, int month, size_t idDepar, int *flag){
    
}

static List MonthTravel(readDataADT bikes, int *flag, query3ADT query){

    char * date; 
    char * departure; 

    for(size_t i = 1; i < query->rows; i++){
        date = getDataFromPos(bikes,i, query->columDepDate, flag);
            int year, month, day, hour, minute, second; 
            sscanf(date, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second); //Despejo la unica variable que me importa, 
        departure = getDataFromPos(bikes, i, query->columIdDep, flag); 
            size_t idDeparture = atoi(departure); 

        query->firstStatId = addStatMonths(query->firstStatId, month, idDeparture, flag);
    } 
    //RETURN
}

query3ADT newQuery(readDataADT bikes, readDataADT stations, int *flag, size_t idDepar, size_t dateDepar){
    query3ADT aux = calloc(1, sizeof(Query3CDT)); //Hago el espacio     
    if(aux == NULL){
        *flag = NO_MEMORY; 
    }
    aux->columDepDate = dateDepar; 
    aux->columIdDep = idDepar; 
    aux->rows = getDimRowsExcel(bikes, flag); 
    aux->firstStatId = MonthTravel(bikes, flag, aux); 

    return aux; 
}