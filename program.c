#include "program.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Inicializar procesos, desde la lectura */
Process* process_init(int pid, char * name, int start_time, int count, int* lista, LinkedList * pointer_lista){
    Process * pointer;
    pointer = malloc(sizeof(Process));
    pointer -> pid = pid;
    strcpy(pointer -> name,name);
    pointer -> start_time = start_time;
    pointer -> cur_quantum = -1;
    pointer -> count = count;
    pointer -> array = lista;
    printf("Create Process object called: %s, with pid: %i, it has to start at: %i and have %i elements\n",
    pointer -> name, pointer -> pid, pointer -> start_time, pointer -> count);
    // printf("Arreglo lista es ");
    // for(int j = 0; j < count; j++) {
    //     printf("%d ", pointer -> array[j]);
    // }
    // printf("\n");
    linkedlist_append(pointer_lista, pointer);
}
/* Termino inicio de procesos

/* Aqui leemos el los programas de inputs, la funcion strtol usada para pasarlo a entero*/
LinkedList * input_read(char *path){
    /* Estas 3 lineas se usan para leer el archivo */
    FILE *fp1;
    char buff[255];                         /* En buff almacenaremos la linea */
    fp1= fopen (path, "r");
    /* Fin de las 3 lineas */ 
    char * name;
    char * pch;
    long start_time;
    long n;
    char *ptr;
    int pid = 1;
    int contador_procesos = 0;
    LinkedList *puntero_bodega;
    puntero_bodega = linkedlist_init();
    /* Aqui falta arreglar para el verdadero porte, de alguna manera linkearlo con el n*/
    int lista_enteros[8];
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
	                    lista_enteros[contador - 2] = atoi(pch);
                    }
                }
                contador += 1;
                pch = strtok (NULL, " ");
              }
        pid += 1; 
        process_init(pid, name, start_time, n, lista_enteros, puntero_bodega); 

    }
    return puntero_bodega;
}
/* Fin lectura archivo */


void spend_quantum(Process* cur, LinkedList* queue) {
    if (cur -> cur_quantum > cur -> cur_burst_value) { /*Si no te vas a gastar todo el quantum que te queda...*/
        cur -> cur_quantum -= cur -> cur_burst_value;
        cur->cur_burst_idx++;
        if (cur -> count > cur -> cur_burst_idx) { /*Hay más bursts*/
            cur -> cur_burst_value = cur -> array[cur->cur_burst_idx];
        } else { /*No hay más bursts*/
            printf("Se terminó proceso de pid %d y nombre %s\n", cur -> pid, cur -> name);
            linkedlist_remove(queue, cur);
        }
    } else {
        cur -> cur_burst_value -= cur -> cur_quantum;
        cur -> cur_quantum = 0;
        // bajar_prioridad();
    }
}


void round_robin(LinkedList* queue, int quantum) {
    while (queue -> puntero_inicio != NULL) {
        Process* cur = queue -> puntero_inicio;
        if (cur -> cur_quantum == -1) { /*lo estoy seteando acá, parte siendo -1*/
            cur -> cur_quantum = quantum;
        }
        spend_quantum(cur, queue);
        if (cur == queue -> puntero_final) { /*Si estamos al final, volver al principio*/
            cur = queue -> puntero_inicio;
        } else { /*avanzar al siguiente en otro caso*/
            cur = cur -> siguiente;
        }
    }
}



/*Funcion que revisa llegadas */
void revisar_llegadas(LinkedList * puntero_bodega, int t)
{
    int k;
    Process *puntero_actual;
    Process *puntero_siguiente;
    puntero_actual = puntero_bodega -> puntero_inicio;
    for (k = 0; k < puntero_bodega -> count; k++){
        puntero_siguiente = puntero_actual -> siguiente;
        if (puntero_actual -> start_time == t){
            printf("Process with name %s, start at %i\n",puntero_actual -> name, puntero_actual -> start_time);
        }
        puntero_actual = puntero_siguiente;
    }
}
/*Fin revisar llegadas */


/*Funciones de linked list */
LinkedList* linkedlist_init()
{
    LinkedList *puntero_ll;
    puntero_ll = malloc(sizeof(LinkedList));
    puntero_ll -> count = 0;
    return puntero_ll;
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

void linkedlist_remove(LinkedList* list, Process* process) {
	Process* cur = list -> puntero_inicio;
	if (cur == process) {
		list -> puntero_inicio = process -> siguiente;
	} else {
		Process* prev;
		while (cur != process) {
			prev = cur;
			cur -> siguiente;
		}
		prev -> siguiente = cur -> siguiente;
	}
}

/* Termino funciones de linked list */
