#include "readExcelTablesADT.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    int flag;
    readExcelTablesADT excel1 = newRead("/mnt/c/Users/lulos/Desktop/FINAL PI/EXCELS/test.csv",&flag);

    size_t cols = getDimColsExcel(excel1);
    size_t rows = getDimRowsExcel(excel1);

    char * aux;

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

    freeExcelData(excel1);

}