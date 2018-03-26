#include "program.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/* Aqui haremos el init de process */
/*Process * process_init(int pid, char nombre[256]){
    Process *pointer;
    pointer = malloc(sizeof(Process));
    pointer -> pid = pid;
    strncpy(pointer -> nombre, nombre, 256);
    printf("Creando proceso de nombre: %s ", pointer -> nombre);
    printf("y pid: %i \n", pointer -> pid);
}*/

Process* process_init(int pid, char * name, int  start_time, int count, int * lista, LinkedList * pointer_lista){
    Process * pointer;
    pointer = malloc(sizeof(Process));
    pointer -> pid = pid;
    pointer -> name = name;
    pointer -> start_time = start_time;
    pointer -> count = count;
    pointer -> array = lista;
    printf("Create Process called: %s, with pid: %i, it has to start at: %i and have %i elements\n",
     pointer -> name, pointer -> pid, pointer -> start_time, pointer -> count);
    linkedlist_append(pointer_lista, pointer);


}

LinkedList* linkedlist_init()

{
    LinkedList *puntero_ll;
    puntero_ll = malloc(sizeof(LinkedList));
    puntero_ll -> count = 0;
    return puntero_ll;
}
    


/* Aqui leemos el los programas de inputs, la funcion strtol usada para pasarlo a entero*/
void input_read(char *path){
    /* Estas 3 lineas se usan para leer el archivo */
    FILE *fp1;
    char buff[255];             /* En buff almacenaremos la linea */
    fp1= fopen (path, "r");
    /* Fin de las 3 lineas */ 
    char * name;
    char * pch;
    long start_time;
    long n;
    LinkedList * puntero_bodega;
    puntero_bodega = linkedlist_init();
    char *ptr;
    int pid = 1;
    int contador_procesos = 0;
    /* Aqui falta arreglar para el verdadero porte, de alguna manera linkearlo con el n*/
    int * lista_enteros[8];
    /* Fin de lo que hay que arreglar */
    while (fgets(buff, 255, (FILE*)fp1) != NULL){
        contador_procesos += 1;
        int contador= 0;
        printf("%s\n", buff );
        name = strtok(buff," ");
        pch = strtok (NULL, " ");
          while (pch != NULL)
              { 
                if (contador == 0){
                    start_time = strtol(pch, &ptr, 10);       
                }
                else if (contador == 1){
                    n = strtol(pch, &ptr, 10);
                }
                else { 
                    if (pch != NULL){
                    lista_enteros[contador - 2] = pch;
                    }
                }
                contador += 1;
                pch = strtok (NULL, " ");
              }
        pid += 1; 
        process_init(pid, name, start_time, n, lista_enteros, puntero_bodega);  
    }

}

void linkedlist_append(LinkedList* list, Process* process_pointer)
{   
    if (list-> count == 0)
    {
        list -> puntero_inicio = process_pointer;
        list -> puntero_final = process_pointer;
        list -> count += 1;

    }
    else 
    {
        list -> puntero_final -> siguiente = process_pointer;
        list -> puntero_final = process_pointer;
        list -> count += 1;


    }


}
