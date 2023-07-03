#include "readDataADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


int main(){
    int flag;
    size_t columnsToCopy[] = {2,0,3};
    size_t dimColumns = sizeof(columnsToCopy) / sizeof(size_t);
    readDataADT excel1 = newRead("/mnt/c/Users/lulos/Desktop/FINAL PI/EXCELS/stationsMON.csv",columnsToCopy, dimColumns, 256, &flag);

    size_t cols = getDimColsExcel(excel1);
    size_t rows = getDimRowsExcel(excel1);

    char * aux;

    for(int i=0; i<rows; i++){
        for(int j=0; j<cols;j++){
            aux = getDataFromPos(excel1, i, j, &flag); // validacion 
            if(flag == 0){
                printf("%s   ", aux);
                free(aux);
            }
        }
        printf("\n");
    }

   // freeExcelData(excel1);

}