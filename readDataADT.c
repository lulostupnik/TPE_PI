#include "readDataADT.h"


typedef struct excelData{
    char * string;
    size_t len;
}tData;

struct readDataCDT{
    tData * dataVec; // pseudo-matrix
    size_t cols;
    size_t rows;
    size_t * columnsToCopy;
};


static char * copyRow(FILE * fp, size_t maxleght, int * flag);
static tData * separeteRow(char * row, size_t maxLenght, int * flag);
static tData * getData(FILE * fp,size_t * dimRows, size_t * columnsToCopy, size_t dimVecColumns,size_t maxLenght, int * flag);

void printMatrix(readDataADT data){
    for(int i=0; i<data->rows;i++){
        for(int j=0; j<data->cols; j++){
            printf("%s\t", data->dataVec[i*data->cols+j].string);
        }
        printf("\n");
    }
}

int getNewColIdx(readDataADT data, size_t excelIdx, int * flag){
    int i=0;
    *flag = OK;
    for(; i<data->cols; i++){
        if(data->columnsToCopy[i] == excelIdx){
            return i;
        }
    }
    *flag = INVALID_POSITION;
    return -1;  
}

readDataADT newRead(char * tablePath, size_t * columnsToCopy , size_t dimColumns, size_t maxLenght, int * flag){
    *flag = OK;
    readDataADT ans = reallocMem(NULL, sizeof(struct readDataCDT), flag);
   
    if(*flag == NO_MEMORY){
        return NULL;
    }

    FILE * fp = fopen(tablePath, "r");
    if(fp == NULL){
        free(ans);
        *flag = INCORRECT_PATH;
        return NULL;
    }
     //Validar aca que los indices de las columnas esten bien (hacer un copy)
    ans->dataVec = getData(fp,&ans->rows, columnsToCopy, dimColumns, maxLenght, flag); 
    ans->cols = dimColumns;
    ans->columnsToCopy = columnsToCopy; // asumimos que no se vuelve a cambiar (sino hay que hacer una copia)

    if(*flag == NO_MEMORY){
        //nose
        return NULL;
    }
   
    fclose(fp);
    return ans;
}



//hay que hacer validacion aca, o queda en el user?
size_t getDimRowsExcel(readDataADT data, int * flag){
    if(data == NULL){
        *flag = NULL_POINTER;
    }
    *flag = OK;
    return data->rows;
}
size_t getDimColsExcel(readDataADT data, int * flag){
    if(data == NULL){
        *flag = NULL_POINTER;
    }
    *flag = OK;
    return data->cols;
}
char * getDataFromPos(readDataADT data, size_t row, size_t col, int * flag){
    *flag = OK;
    if (row >= data->rows || col >= data->cols) {
        *flag = INVALID_POSITION;
        return NULL;
    }
    char * ans = reallocMem(NULL, data->dataVec[row * data->cols+col].len+1, flag);
    if(ans == NULL){
        *flag = NO_MEMORY;
        return NULL;
    }
 
    return strcpy(ans, data->dataVec[row*data->cols+col].string);
}







static char * copyRow(FILE * fp, size_t maxleght, int * flag){
  char * aux=malloc(maxleght);
  if(aux == NULL){
    *flag = NO_MEMORY;
    return NULL;
  }
  return fgets(aux,maxleght,fp);
}

#define BLOCK_COLUMN 5 

static tData * separeteRow(char * row, size_t maxLenght, int * flag){
    tData * ans=NULL;
    char * token;
    int i=0;
    *flag = OK;
    token=strtok(row,";");
    while(token != NULL){
        if(i%BLOCK_COLUMN == 0){
            ans = realloc(ans, sizeof(tData) * (i+BLOCK_COLUMN));
            if(ans == NULL){
                *flag = NO_MEMORY;
                return NULL;
            }
        }

        ans[i].string = copyString(token, &ans[i].len, flag);
        if(*flag == NO_MEMORY){
            //liberar los anteriores.
            return NULL;
        }
        i++;
    token=strtok(NULL,";");
    }
    return ans;
}
static tData * getData(FILE * fp,size_t * dimRows, size_t * columnsToCopy, size_t dimVecColumns,size_t maxLenght, int * flag){ // generar Matriz
    tData * rta = NULL;
    *flag = OK;
    char * aux;
    tData * separetedRow = NULL;
    *dimRows = 0;

    while((aux = copyRow(fp, maxLenght, flag)) != NULL){
           if(*flag == NO_MEMORY){ // del copyRow
                // liberar recursos del for. 
                return NULL;
           }

           separetedRow = separeteRow(aux, maxLenght, flag); // separa las columnas de una fila
           if(*flag == NO_MEMORY){
            //lpm
            return NULL;
           }

     

           rta = reallocMem(rta, sizeof(tData) * dimVecColumns *(*dimRows+1), flag);
           if(*flag == NO_MEMORY){
            //putra
            return NULL;
           }
           
           for(int j=0; j < dimVecColumns;j++){

                rta[*dimRows * dimVecColumns + j ].string = separetedRow[columnsToCopy[j]].string;
                rta[*dimRows * dimVecColumns + j ].len = separetedRow[columnsToCopy[j]].len; 
                /*
                Esto es al pedo, daba memory leaks o tenia que liberar cada elemento de separatedRows 
                rta[*dimRows * dimVecColumns+j].string = malloc(separetedRow[columnsToCopy[j]].len + 1);
                rta[*dimRows * dimVecColumns+j].len = separetedRow[columnsToCopy[j]].len;
                if(rta[*dimRows * dimVecColumns+j].string == NULL){
                    //posta matame
                    return NULL;
                }
                strcpy(rta[*dimRows * dimVecColumns+j].string, separetedRow[columnsToCopy[j]].string);*/
           }
           (*dimRows)++;
           free(aux);
           free(separetedRow);
    }
    
    return rta;
}
/*static void * reallocMem(void * target, size_t spaces, int * flag){
      void * ptr = realloc(target, spaces);
      if(ptr == NULL){
            free(target);
            *flag = NO_MEMORY;
            return NULL;
      }
      *flag = OK;
      return ptr;
}

#define BLOCK_STR 20

static char * copyString(char * s, size_t * len, int * flag){
	char * rta = NULL;
	int i;
	for(i=0; s[i] != '\n' && s[i] != '\0'; i++){
		if(i%BLOCK_STR == 0){
			rta = realloc(rta, i+BLOCK_STR);
            if(rta == NULL){
                *flag = NO_MEMORY;
                return NULL;
            }
		}
		rta[i] = s[i];
	}
	rta = realloc(rta, i+1); // No deberia fallar ya que se achica el string. (Checkear con la madre tierra).
    rta[i] = 0;
	*len = i;
	return rta;
}
*/