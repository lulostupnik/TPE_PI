#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>

char * getField(FILE * fp, char * limits, char * foundLimit, int * flag);
char ** getData(FILE * fp, char * limits, size_t * rows, size_t * cols ,int * flag);
void * reallocMem(void * target, size_t spaces, int * flag);



int main(void)
{
   FILE *fp;

   fp = fopen("/mnt/c/Users/lulos/Desktop/FINAL PI/EXCELS/bikesMON.csv", "r");
   if (fp == NULL)
   {
      printf("No se pudo abrir el archivo\n");
      return 1;
   }
   char limits[]={';', '\n', EOF};
   int flag;
 
   size_t rows=1, cols;   
   char ** matrix = getData(fp, limits, &rows, &cols,&flag);
   printf("Hay %ld rows y %ld cols\n\n", rows, cols);
   fclose(fp);
}

#define BLOCK_STR 20



void * reallocMem(void * target, size_t spaces, int * flag){
      void * ptr = NULL;
      ptr = realloc(target, spaces);
      if(ptr == NULL){
            free(target);
            *flag = ENOMEM;
            return NULL;
      }
     
      *flag = 0;
      return ptr;
}

char * getField(FILE * fp, char * limits, char * foundLimit, int * flag){
      char * rta = NULL, * search, c;
      int i;

      for(i=0, c=fgetc(fp); (search = strchr(limits, c)) == NULL; c=fgetc(fp), i++){
            if(i%BLOCK_STR == 0){
               rta = reallocMem(rta, i+BLOCK_STR, flag);
               if(*flag == ENOMEM){
                  return NULL;
               }
            }
            rta[i] = c;
      }
      rta = reallocMem(rta, 1+i, flag);
      if(*flag == ENOMEM){
            return NULL;
      } 
      rta[i] = '\0';
      *foundLimit = *search;
      return rta;
}

#define BLOCK_COLS 5
//No checkeo por ENOMEM. La funcion devulve una pseudo matrix con los datos. 
char ** getData(FILE * fp, char * limits, size_t * rows, size_t * cols ,int * flag){
      char limit = '0';
      char ** rta = NULL;
      int cant_cols = 0;
      for(cant_cols = 0; limit != '\n' && limit != EOF; cant_cols++){
            if(cant_cols % BLOCK_COLS == 0){
                  rta = reallocMem(rta, (cant_cols+BLOCK_COLS) * sizeof(char*), flag);
            }
            rta[cant_cols] = getField(fp, limits, &limit, flag);
      }
      rta = reallocMem(rta, cant_cols * sizeof(char *), flag);
      *cols = cant_cols;
      int i;
      for(i=1; limit != EOF ;i++){
            rta = reallocMem(rta, (cant_cols*(i+1))*sizeof(char *), flag);
            for(int j = 0; j<cant_cols;j++){
                  rta[cant_cols*i+j] = getField(fp, limits, &limit, flag);
            }
      }
   
      *rows = i;
      return rta;
}