#include "query2.h"
#include "readDataADT.h"

typedef struct nodo{
    size_t departure;
    size_t arrival;
    char * nameDepar;
    char * nameArr;
    size_t travels;
    struct nodo* tail;
}nodo;

typedef struct nodo* List;

typedef struct query2CDT{
    List firstId;
    List firstName;
    size_t columIdDep;
    size_t columIdArr;
    size_t rows;
}query2CDT;

static int cmp(size_t e1,size_t e2){
    if(e1== e2)
    return 0;
    if(e1>e2)
    return 1;
    
    return -1;

}

static List addTravelsRec(List lista, size_t idDepar, size_t idArr, int *flag) {
    char c;
    if (lista == NULL || (c=cmp(lista->departure , idDepar))>0 ) {
        List aux = malloc( sizeof(nodo));
        if (aux == NULL) {
            *flag = NO_MEMORY;
            return NULL;
        }
        aux->departure = idDepar;
        aux->arrival = idArr;
        aux->travels = 1; 
        aux->tail = lista;
        aux->nameArr=NULL;
        aux->nameArr=NULL;
        return aux;
    }
    if (c == 0 && cmp(lista->arrival,idArr) == 0) {
        lista->travels++;
        return lista;
    }
    lista->tail = addTravelsRec(lista->tail, idDepar, idArr, flag);
    return lista;
}


static List addTravels(List lista,readDataADT bikes,int * flag,query2ADT query){
    for(int i=0;i<query->rows;i++){
    char * aux1=getDataFromPos(bikes,i,0,flag);
    char * aux2=getDataFromPos(bikes,i,1,flag);
    size_t idDepar = (size_t) atoi(aux1);
    size_t idArr = (size_t) atoi(aux2);
     free(aux1);
     free(aux2);
        if(idDepar != idArr){
        lista=addTravelsRec(lista,idDepar,idArr,flag);
        }
    }
    return lista;
}

static void print(List lista){
    if(lista == NULL)
        return;
    printf("%ld\t %ld\t %ld\t\n",lista->departure,lista->arrival,lista->travels);
    print(lista->tail);
}


query2ADT newQuery(readDataADT bikes,readDataADT stations,int * flag,size_t  idDep, size_t idArr){
    query2ADT aux=calloc(1,sizeof(*aux));
    if(aux == NULL){
        *flag = NO_MEMORY;
        return NULL;
    }
    aux->columIdArr=idArr;
    aux->columIdDep=idDep;
    aux->rows=getDimRowsExcel(bikes);
    aux->firstId=addTravels(aux->firstId,bikes,flag,aux);
    aux->firstName = NULL;
   //print(aux->firstId);
    return aux;
}
