//Este es el main de todo el TP para Montreal
#include "readDataADT.h"
#include "query1ADT.h"
#include "errores.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define MAX_LENGHT 256


int main(int argc, char * argv[]){
    int flag;
    if(argc != 3){
        //handleError(INVALID_ARGUMENTS);
        return INVALID_ARGUMENTS;
    }

    size_t  columnsToCopyBikes[] = {0,1,3,4}, columnsToCopyStations[] = {0,1}, dimColsBike = sizeof(columnsToCopyBikes)/sizeof(*columnsToCopyBikes), dimColsStations = sizeof(columnsToCopyStations)/sizeof(*columnsToCopyStations); //Elegimos solo las columnas necesarias para completar los queries solicitados
    
    readDataADT bikesExcel = newRead(argv[1], columnsToCopyBikes, dimColsBike , MAX_LENGHT, &flag);
    if(flag != OK){
        //handleError(flag);
        return flag;
    }
 
    readDataADT stationsExcel = newRead(argv[2], columnsToCopyStations, dimColsStations ,MAX_LENGHT, &flag);
    if(flag != OK){
        //handleError(flag);
        return flag;
    }

    //Como el archivo de MONTREAL se saltea pocos IDs y los mismos empiezan con un nuemro cercano a 0, usamos un vector para "juntar" los IDs con los nombres de las estaciones. El indice del vector sera el ID
    size_t dimRowsStations = getDimRowsExcel(stationsExcel, &flag);
    if(flag != OK){
        //handleError(flag);
        return flag;
    }

    char ** vecNames = NULL;
    size_t maxId = 0;
    for(int i=0; i<dimRowsStations ; i++){
        for(int j=0; j<dimColsStations;j++){
            getDataFromPos()
        }
    }

    
    return OK;
}

