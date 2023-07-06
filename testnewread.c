#include "readDataADT.h"

#include "misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


int main(){
    int flag;
    //newreadADT excel1= newReadstation("../EXCELS/stationsMON.csv",0,1,256,&flag);
    readDataADT excel2=newReadBikes("../EXCELS/bikesMON.csv",1,0,3,4,256,&flag);
    
    int rows = getrows(excel2);

    char * test;
    test = "hola";
    /*for(int i=0; i<rows ; i++){
            for(int j=0; j<4; j++){
                test = getDataFromPosBike(excel2, i, j, &flag);
                printf("%s\n", test);
                free(test);
            }
            printf("\n");
    }*/
    printf("Cant rows %d\n", rows);
    
    
    //freeExcel(excel1);
    freeExcel(excel2);
}

