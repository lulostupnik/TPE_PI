#include "readDataADT.h"
#include "query2.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


int main(){
    int flag;
    size_t columnsToCopy[] = {0,1};
    size_t columsToCopy2[] ={1,3}; // bikes
    size_t dimColumns = sizeof(columnsToCopy) / sizeof(size_t);
    size_t dimColumns2 = sizeof(columsToCopy2) / sizeof(size_t);
    readDataADT excel1 = newRead("./stationsMON.csv",columnsToCopy, dimColumns, 256, &flag);
    readDataADT excel2 = newRead("./test.csv",columsToCopy2, dimColumns2, 256, &flag);
    size_t id1=0;
    size_t id2=1;
    query2ADT test=newQuery(excel2,excel1,&flag,id1,id2);
    
    
    
    
    
    
    
    // size_t cols = getDimColsExcel(excel1);
    // size_t rows = getDimRowsExcel(excel1);

    // char * aux;

    // for(int i=0; i<rows; i++){
    //     for(int j=0; j<cols;j++){
    //         aux = getDataFromPos(excel1, i, j, &flag); // validacion 
    //         if(flag == 0){
    //             printf("%s   ", aux);
    //             free(aux);
    //         }
    //     }
    //     printf("\n");
    // }

   // freeExcelData(excel1);

}