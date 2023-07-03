#include "readExcelTablesADT.h"
#include "query1ADT.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    int flag;
    //En realidad, el Q1 no tienee que abrir los excel, sino que recibir el ADT del excel abierto
    readExcelTablesADT excel1 = newRead("/mnt/c/Users/lulos/Desktop/FINAL PI/EXCELS/bikesMON.csv",&flag);
    readExcelTablesADT excelStations = newRead("/mnt/c/Users/lulos/Desktop/FINAL PI/EXCELS/stationsMON.csv",&flag);
    printf("FLAG %d\n", flag);
    query1ADT q1 = newQuery1(&flag);
    
    size_t cols = getDimColsExcel(excel1);
    size_t rows = getDimRowsExcel(excel1);

    char * aux;
    puts("Los datos desorganizados");
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols;j++){
            aux = getDataFromPos(excel1, i, j, &flag);
            if(flag == 0){
                printf("%s\t", aux);
                free(aux);
            }
        }
        printf("\n");
    }
    puts("-");
    puts("-");

    //Agrego todos lo viajes
    
    for(int i=1; i<rows-1; i++){
        aux = getDataFromPos(excel1, i, 1, &flag);
        printf("%d\n", atoi(aux));
        printf("Flag add trip %d\n", addTrip(q1, atoi(aux)));
        //free(aux);
    }

    printTrips(q1);
    printf("\n\n");

    
    size_t colsStation = getDimColsExcel(excelStations);
    int rowsStation = getDimRowsExcel(excelStations);
    char * auxName;
    char * auxStationId;

    printf("Cols station %ld, rows station %d \n", colsStation, rowsStation);

    int flag1, flag2, i;

    //LA funcion getData me esta fallando as que no puedo probar si Add Name, orderByCant trip y las iteradores sirve. Aiuda
    for(i=1; i<rowsStation-1; i++){
        auxName = getDataFromPos(excelStations, i, 1, &flag1); 
        auxStationId = getDataFromPos(excelStations, i, 0, &flag2);
        printf("Name:%s id: %d \n", auxName,atoi(auxStationId));
        addName(q1, auxName, strlen(auxName), atoi(auxStationId));
    }
    printf("\n\n");
    printTrips2(q1);
    printf("\n\n");


    
    orderByCantTrip(q1);
    toBegin(q1);
    char * name;
    size_t len, cantTrips;
    while(hasNext(q1)){
        next(q1, &name, &len, &cantTrips);
        printf("%s, %ld\n", name, cantTrips);
    }

    //faltan frees
    freeExcelData(excel1);
    freeExcelData(excelStations);

}