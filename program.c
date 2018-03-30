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
    linkedlist_append(pointer_lista, pointer, 0);
}
/* Termino inicio de procesos*/


LinkedList* create_queue() {
	LinkedList* queue;
	queue -> puntero_inicio = NULL;
	queue -> puntero_final = NULL;
	queue -> count = 0;
	return queue;
}


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

void bajar_prioridad(Process* cur, LinkedList* queue, LinkedList* QueueArray[]) {
	int n = sizeof(QueueArray) / sizeof(QueueArray[0]);
	LinkedList* queue_check = QueueArray[0];
	int y = 0;
	while (queue_check != queue) {
		queue_check = QueueArray[++y];
	}
	linkedlist_remove(cur, queue, 1);
	linkedlist_append(cur, QueueArray[y+1], 1);
	cur -> cur_quantum = -1; /*round robin se encarga de ponerle el correcto*/
	printf("Movimos de queue n° %d a %d\n", y, y+1);
}


void spend_quantum(Process* cur, LinkedList* queue, LinkedList* QueueArray[]) {
    if (cur -> cur_quantum > cur -> cur_burst_value) { /*Si no te vas a gastar todo el quantum que te queda...*/
        cur -> cur_quantum -= cur -> cur_burst_value;
        cur->cur_burst_idx++;
        if (cur -> count > cur -> cur_burst_idx) { /*Hay más bursts*/
            cur -> cur_burst_value = cur -> array[cur->cur_burst_idx];
        } else { /*No hay más bursts*/
            printf("Se terminó proceso de pid %d y nombre %s\n", cur -> pid, cur -> name);
            linkedlist_remove(queue, cur, 1);
        }
    } else {
        cur -> cur_burst_value -= cur -> cur_quantum;
        cur -> cur_quantum = 0;
        cur -> interrups++;
        bajar_prioridad(cur, queue, QueueArray);
    }
}


void round_robin(LinkedList* queue, int quantum, LinkedList* QueueArray[]) {
	printf("En round_robin 0\n");
	Process* cur;
	// printf("%s\n", queue);
	printf("En round_robin 1\n");
	printf("%s\n", queue -> puntero_inicio);
	printf("En round_robin 2\n");
    while (queue -> puntero_inicio != NULL) {
        cur = queue -> puntero_inicio;
        strcpy(cur -> estado, "ru");
        cur -> elegido_cpu++;
        if (cur -> cur_quantum == -1) { /*lo estoy seteando acá, parte siendo -1*/
            cur -> cur_quantum = quantum;
        }
        spend_quantum(cur, queue, QueueArray);
        strcpy(cur -> estado, "re");
        if (cur == queue -> puntero_final) { /*Si estamos al final, volver al principio*/
            cur = queue -> puntero_inicio;
        } else { /*avanzar al siguiente en otro caso*/
            cur = cur -> siguiente_q;
        }
    }
}



/*Funcion que revisa llegadas */
void revisar_llegadas(LinkedList * puntero_bodega, int t, int queues, LinkedList* QueueArray[queues], int quantum)
{ /*Último parámetro es solamente el largo de QueueArray*/
	// printf("ACA0\n");
    int k;
    Process *puntero_actual;
    Process *puntero_siguiente;
    puntero_actual = puntero_bodega -> puntero_inicio;
    for (k = 0; k < puntero_bodega -> count; k++){
    	// printf("ACA1 %d %d\n", k, puntero_bodega -> count);
    	// printf("puntero_actual %s %s\n", puntero_actual -> name, puntero_actual -> siguiente);
    	// printf("puntero_siguiente %s\n", puntero_actual -> siguiente_q -> name);
        puntero_siguiente = puntero_actual -> siguiente;
        // printf("ACA1 post\n");
        if (puntero_actual -> start_time == t){
            printf("Process with name %s, start at %i\n",puntero_actual -> name, puntero_actual -> start_time);
            linkedlist_append(QueueArray[0], puntero_actual, 1); 
        }
        // printf("ACA pre\n");
        puntero_actual = puntero_siguiente;
        // printf("ACA post\n");
    }
    for (int i=0; i < queues; i++) {
    	// printf("ACA2\n");
    	round_robin(QueueArray[i], quantum, QueueArray);
    	// printf("ACA3\n");
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
    

void linkedlist_append(LinkedList* list, Process* process_pointer, int Q)
{   /*Q = 0 es append en bodega, 1 en Queue*/
	if (Q == 0) { /*bodega, se ocupa process -> siguiente*/
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
	} else if (Q == 1) { /*queue, se ocupa process -> siguiente_q*/
	    if (list-> count == 0)
	    {
	        list -> puntero_inicio = process_pointer;
	        list -> puntero_final = process_pointer;
	        list -> count += 1;

	    }
	    else 
	    {
	        list -> puntero_final -> siguiente_q = process_pointer;
	        list -> puntero_final = process_pointer;
	        list -> count += 1;
	    }
	}
    
}

void linkedlist_remove(LinkedList* list, Process* process, int Q) { /*Q = 0 es append en bodega, 1 en Queue*/
	Process* cur = list -> puntero_inicio;
	if (Q == 0) { /*bodega, se ocupa process -> siguiente*/
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
	} else if (Q == 1) { /*queue, se ocupa process -> siguiente_q*/
		if (cur == process) {
			list -> puntero_inicio = process -> siguiente_q;
		} else {
			Process* prev;
			while (cur != process) {
				prev = cur;
				cur -> siguiente_q;
			}
			prev -> siguiente = cur -> siguiente_q;
		}
	}
	
}

/* Termino funciones de linked list */
