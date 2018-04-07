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
    pointer -> cur_burst_value = pointer -> array[0];
    pointer -> cur_burst_idx = 0;
    printf("Create Process object called: %s, with pid: %i, it has to start at: %li and have %li elements\n",
    pointer -> name, pointer -> pid, pointer -> start_time, pointer -> count);
    linkedlist_append(pointer_lista, pointer, 0);
    return pointer;
}
/* Termino inicio de procesos*/


LinkedList* create_queue() {
	LinkedList* queue = malloc(sizeof(LinkedList*));
	Process* aux = malloc(sizeof(Process*));
    aux -> pid = 0;
    aux -> name[255] = NULL;
    aux -> start_time = 0;
    aux -> count = 0;
    aux -> array = NULL;
    aux -> cur_burst_idx = 0;                  /*Índice de burst que toca*/
    aux -> cur_burst_value = 0;                /*Pero si ya lleva un poco de la burst actual, aquí decimos cuánto queda*/
    aux -> cur_quantum = 0;
    aux -> siguiente = NULL;         /*Usado para bodega inicial*/
    aux -> siguiente_q = NULL;       /*Usado para queue*/
    aux -> estado[2] = NULL;                     /*estados ru, re, fi */
    aux -> elegido_cpu = 0;
    aux -> interrups = 0;
    aux -> turnaround_t = 0;
    aux -> response_t = 0;
    aux -> waiting_t = 0;

	queue -> puntero_inicio = aux;
	queue -> puntero_final = aux;
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
    /* Fin de lo que hay que arreglar */
    while (fgets(buff, 255, (FILE*)fp1) != NULL){
        int *lista_enteros = (int*)calloc(8, sizeof(int));
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

void bajar_prioridad(Process* in_cpu, LinkedList* queue, LinkedList* QueueArray[], int quantum) {
	int n = sizeof(QueueArray) / sizeof(QueueArray[0]);
	LinkedList* queue_check = QueueArray[0];
	int y = 0;
	while (queue_check != queue) {
		queue_check = QueueArray[++y];
	}
	linkedlist_remove(queue, in_cpu, 1);
	linkedlist_append(QueueArray[y+1], in_cpu, 1);
	in_cpu -> cur_quantum = quantum;
	printf("Movimos de queue n° %d a %d\n", y, y+1);
}



Process* wrapper_rr(int queues, LinkedList* QueueArray[queues], int quantum, Process* in_cpu, int t){
      for (int i=0; i < queues; i++) {
        // printf("ACA, llamando a round_robin en queue %d, que tiene %d elemento(s)\n", i, QueueArray[i] -> count);
        if (QueueArray[i] -> count > 0) {
            printf("Llamando a round_robin en queue %d, la primera con al menos un elemento\n", i);
            Process* in_cpu_2 = round_robin(QueueArray[i], quantum, QueueArray, in_cpu, t);
            return in_cpu_2;
        }
        
    }

    return (Process*) NULL;
} 


Process* encontrar_siguiente_proceso(Process* in_cpu, LinkedList* queue, LinkedList* QueueArray[], int quantum, int t) {
    Process* otro;
    if (queue -> count > 0) {                   
        if (in_cpu == queue -> puntero_final) {    /*Caso 2-a: tenemos que volver a principio queue*/
            otro = queue -> puntero_inicio;
        } else {                                /*Caso 2-b: seguimos con el siguiente*/
            otro = in_cpu -> siguiente_q;
        }
        in_cpu -> interrups++;                     /*Sumamos 1 a interrupciones del proceso*/
        strcpy(in_cpu -> estado, "re");
        strcpy(otro -> estado, "ru");
        return otro;
    } else {  
        printf("Buscando en una lista de abajo\n");                                  /*3er caso: hay que ir a la siguiente queue*/
        return wrapper_rr(sizeof(QueueArray) / sizeof(QueueArray[0]), QueueArray, quantum, in_cpu, t);
    }
}



Process* round_robin(LinkedList* queue, int quantum, LinkedList* QueueArray[], Process* in_cpu, int t) {

    if (in_cpu && in_cpu -> cur_quantum == -1) {
        in_cpu -> cur_quantum = quantum; 
    }

    /*Caso 0: no hay nadie en CPU y tiene que entrar alguien*/
    if (in_cpu == NULL) {
        printf("Caso 0\n");
        in_cpu = queue -> puntero_inicio;
        strcpy(in_cpu -> estado, "ru");
        return in_cpu;
    }

    if (in_cpu -> cur_quantum > 0 && (in_cpu -> cur_burst_value > 0 || in_cpu -> cur_burst_idx < in_cpu -> count - 1)) {
    /*1er caso: todavía queda quantum y queda algún burst, sigue el mismo en CPU ()*/
        printf("En caso 1\n");
        if (in_cpu -> cur_burst_value == 0) {   /*No queda nada en este burst*/
            in_cpu -> cur_burst_idx++;         /*Siguiente burst*/
            in_cpu -> cur_burst_value = in_cpu -> array[in_cpu -> cur_burst_idx];
        }
        in_cpu -> cur_burst_value--;
        in_cpu -> cur_quantum--;
        //sumar a tiempo de procesamiento 1
        printf("cur_quantum: %d; cur_burst_value: %d\n", in_cpu -> cur_quantum, in_cpu -> cur_burst_value);
        return in_cpu;
    }

    /*3er caso: se me acaba proceso*/
    if (in_cpu -> cur_burst_value == 0 && in_cpu -> cur_burst_idx == in_cpu -> count - 1) {
        in_cpu -> turnaround_t = t - in_cpu -> start_time;
        linkedlist_remove(queue, in_cpu, 1);
        strcpy(in_cpu -> estado, "fi");
        printf("cur_quantum: %d; cur_burst_value: %d\n", in_cpu -> cur_quantum, in_cpu -> cur_burst_value);
        return encontrar_siguiente_proceso(in_cpu, queue, QueueArray, quantum, t);
    }

    /*2do caso: se me acaba quantum*/
    if (in_cpu -> cur_quantum == 0) {
        if (in_cpu -> cur_burst_value == 0) {
            in_cpu -> cur_burst_idx++;
            in_cpu -> cur_burst_value = in_cpu -> array[in_cpu -> cur_burst_idx];
        }

        bajar_prioridad(in_cpu, queue, QueueArray, quantum);
        printf("cur_quantum: %d; cur_burst_value: %d\n", in_cpu -> cur_quantum, in_cpu -> cur_burst_value);
        sleep(1);
        in_cpu = encontrar_siguiente_proceso(in_cpu, queue, QueueArray, quantum, t);
        return in_cpu;
    }

    
    
    return (Process*) NULL;
}



/*Funcion que revisa llegadas */
void revisar_llegadas(LinkedList * puntero_bodega, int t, LinkedList* queue)
{ 

    int k;
    Process *puntero_actual;
    Process *puntero_siguiente;
    puntero_actual = puntero_bodega -> puntero_inicio;
    for (k = 0; k < puntero_bodega -> count; k++){
        puntero_siguiente = puntero_actual -> siguiente;
        if (puntero_actual -> start_time == t){
            printf("Process with name %s, start at %li\n",puntero_actual -> name, puntero_actual -> start_time);
            linkedlist_append(queue, puntero_actual, 1); 
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

void linkedlist_remove(LinkedList* list, Process* process, int Q) { /*Q = 0 es remove en bodega, 1 en Queue*/
	Process* cur = list -> puntero_inicio;
	if (Q == 0) { /*bodega, se ocupa process -> siguiente*/
		if (cur == process) {
			list -> puntero_inicio = process -> siguiente;
		} else {
			Process* prev;
			while (cur != process) {
				prev = cur;
				cur = cur -> siguiente;
			}
			prev -> siguiente = cur -> siguiente;
            if (cur == list -> puntero_final) {
                list -> puntero_final = prev;
            }
		}
	} else if (Q == 1) { /*queue, se ocupa process -> siguiente_q*/
		if (cur == process) {
			list -> puntero_inicio = process -> siguiente_q;
			if (list -> count == 1) {
				list -> puntero_final = list -> puntero_inicio;
			}
		} else {
			Process* prev;
			while (cur != process) {
				prev = cur;
				cur = cur -> siguiente_q;
			}
			prev -> siguiente = cur -> siguiente_q;
            if (cur == list -> puntero_final) {
                list -> puntero_final = prev;
            }
		}
	}
	list -> count--;
}

/* Termino funciones de linked list */
