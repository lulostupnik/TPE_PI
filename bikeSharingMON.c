//Este es el main de todo el TP para Montreal
#include "readDataADT.h"
#include "query1ADT.h"
#include "errores.h"
#include "misc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>


#define MAX_LENGHT 256




static char ** getVecNames(readDataADT stationsExcel, int * dim,size_t dimRowsStations, size_t namesColumn, size_t IdColumn ,int * flag){
    char ** vecNames = NULL;
    char * auxName;
    char * auxIdPtr;
    int i, auxId;
    int maxId = -1;
    for(i=0; i<dimRowsStations ; i++){
      
        auxName = getDataFromPos(stationsExcel, i, namesColumn, flag);
        if(*flag != OK){
            //liberar los anteriores tmb y el q estoy
            return NULL;
        }

        //Para usar atoi, asumo que los datos del Excel son correctos
        auxIdPtr = getDataFromPos(stationsExcel, i, IdColumn, flag);
        auxId = atoi(auxIdPtr);
        free(auxIdPtr);
        if(*flag != OK){
            //liberar los anteriores tmb y el q estoy
            return NULL;
        }
        if(auxId < 0){
            *flag = INVALID_ID;
            //
            return NULL;
        }
        
      
        if(auxId > maxId){
            vecNames = reallocMem(vecNames, sizeof(*vecNames)*(auxId+1), flag);
            if(*flag != OK){
                //completar
                return NULL;
            }
            maxId = auxId;
        }
        vecNames[auxId] = auxName;
    }
    *dim = maxId+1;
    return vecNames;
}



int main(int argc, char * argv[]){
    int flag;
    if(argc != 3){
        //handleError(INVALID_ARGUMENTS);
        return INVALID_ARGUMENTS;
    }

    size_t  columnsToCopyBikes[] = {START_DATE,EMPLACEMENT_PK_START,EMPLACEMENT_PK_END,IS_MEMBER}, columnsToCopyStations[] = {ID,NAMES}, dimColsBike = sizeof(columnsToCopyBikes)/sizeof(*columnsToCopyBikes), dimColsStations = sizeof(columnsToCopyStations)/sizeof(*columnsToCopyStations); //Elegimos solo las columnas necesarias para completar los queries solicitados
    
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

    int dim;
    char ** vecNames = getVecNames(stationsExcel, &dim, dimRowsStations, NAMES, ID, &flag);
    if(flag != OK){
        //handle error
        return flag;
    }   


    //hago los queries
    //FALTA VER MEMEBER !!
    query1ADT q1 = newQuery1(bikesExcel,stationsExcel,&flag); // falta checkear que sea miembro xd
    if(flag != OK){
        //
        return flag;
    }
    
    /*flag = processDataQ1(q1,1, 0, 1);
     if(flag != OK){
        //
        return flag;
    }*/

  
   //Manera alternativa de procesar (da invalid ID...)
   flag = processCantTrips(q1, 1); 
    if(flag != OK){
        //
        return flag;
    }

    flag = addNamesFromVec(q1, vecNames, dim);
    if(flag != OK){
        //
        return flag;
    }
    //NECESARIAMENTE HAY QUE AGREGAR LOS NOMBRES ANTES DE ORDENAR. Sino el flag va a ser NULL_PTR xq se hubiera accedido al mismo.
    flag = orderByCantTrip(q1);
    if(flag != OK){
        //
        return flag;
    }
    



    //Ya puedo iterar
    toBegin(q1);
    char * name;
    size_t len, cantTrips;
    int i=0;

    while(hasNext(q1)){     
        next(q1, &name, &len, &cantTrips);
       i+= cantTrips;
        //funciones HTML
        printf("In iterator: %s, %ld\n", name, cantTrips);
    }
   


    return OK;
}

