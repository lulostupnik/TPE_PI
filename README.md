# TPE_PROGRAMACION IMPERATIVA 
  - Fecha Final 11/07/2023
    
# GRUPO 
  - Nicolas Bellavitis - 64001
  - Luciano Stupnik - 64233
  - Tomas Balboa - 64237

# COMO GENERAR EJECUTABLES 
**All**
- Si se desea compilar todo el programa, correr en la consola el comando "make all". El mismo genera 2 ejecutables, uno correspondiente a la ciudad de Nueva York (NYC) y otro a Montreal (MON).
  
**Una Ciudad**
- Si se quiere compilar solo la ciudad de Nueva York, correr el comando "make BikeSharingNYC".
- Si se desea compilar solo la ciudad de Montreal, correr el comando "make BikeSharingMON"

**Flag -g**
- Para compilar con el flag -g, correr en la consola el comando "make debug"

**Delete**
- Para borrar todos los archivos generados correr el comando "make clean"
- Para borrar los ejecutables correr el comando "make deleteExecutables"
- Para borrar los queries correr el comando "make deleteQueries"

# EJECUCION
- Para ejecutar el archivo: 
- ./bikeSharingMON "path del archivo csv de bikes MON" "path del archivo csv de estaciones MON"
- ./bikeSharingNYC "path del archivo csv de bikes NYC" "path del archivo csv de estaciones NYC"
**Aclaracion**
- El orden de los archivos y la cantidad de argumentos debe ser la indicada arriba.

