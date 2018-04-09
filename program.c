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
    pointer -> response_t = -1;
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
    // aux -> name[255] = NULL;
    aux -> start_time = 0;
    aux -> count = 0;
    aux -> array = NULL;
    aux -> cur_burst_idx = 0;                  /*Índice de burst que toca*/
    aux -> cur_burst_value = 0;                /*Pero si ya lleva un poco de la burst actual, aquí decimos cuánto queda*/
    aux -> cur_quantum = 0;
    aux -> siguiente = NULL;         /*Usado para bodega inicial*/
    aux -> siguiente_q = NULL;       /*Usado para queue*/
    // aux -> estado[2] = NULL;                     /*estados ru, re, fi */
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

int buscar_idx(LinkedList* queue, int queues, LinkedList* QueueArray[queues]);

void bajar_prioridad(Process* in_cpu, LinkedList* queue, LinkedList* QueueArray[], int quantum, int queues, int v3) {
	LinkedList* queue_check = QueueArray[0];
	int y = 0;
	while (queue_check != queue) {
		queue_check = QueueArray[++y];
	}
    if (y < queues - 1) {
        linkedlist_remove(queue, in_cpu, 1);
        linkedlist_append(QueueArray[y+1], in_cpu, 1);
        if (v3) {
            // printf("buscar_idx retorna %d\n", buscar_idx(QueueArray[y+1], queues, QueueArray));
            in_cpu -> cur_quantum = quantum * (buscar_idx(QueueArray[y+1], queues, QueueArray) + 1); 
        } else {
            in_cpu -> cur_quantum = quantum;
        }
        printf("Movimos el proceso %s de queue n° %d a %d\n", in_cpu -> name, y, y+1);
    } else {
        if (v3) {
            in_cpu -> cur_quantum = quantum * (buscar_idx(queue, queues, QueueArray) + 1); 
        } else {
            in_cpu -> cur_quantum = quantum;
        }
        printf("Última cola, no bajamos prioridad\n");
    }
}

Process* wrapper_rr(int queues, LinkedList* QueueArray[queues], int quantum, Process* in_cpu, int t, int v3){
      for (int i=0; i < queues; i++) {
        if (QueueArray[i] -> count > 0) {
            printf("Llamando a round_robin en queue %d, la primera con al menos un elemento\n", i);
            Process* in_cpu_2 = round_robin(QueueArray[i], quantum, QueueArray, in_cpu, t, queues, v3);
            return in_cpu_2;
        }
    }
    return (Process*) NULL;
} 

int buscar_idx(LinkedList* queue, int queues, LinkedList* QueueArray[queues]) {
    for (int i=0; i < queues; i++) {
        if (QueueArray[i] == queue) {
            return i;
        }
    }
    printf("CUIDADO\n");
    return -1;
}

int conteo_procesos(LinkedList* QueueArray[], int queues) {
    int cont = 0;
    for (int i=0; i < queues; i++) {
        cont += QueueArray[i] -> count;
    }
    return cont;
}

Process* encontrar_siguiente_proceso(Process* in_cpu, LinkedList* queue, LinkedList* QueueArray[], int quantum, int t, int queues) {
    Process* otro;
    if (queue -> count > 1) {
        if (in_cpu == queue -> puntero_final) {    /*Caso 2-a: tenemos que volver a principio queue*/
            otro = queue -> puntero_inicio;
        } else {                                /*Caso 2-b: seguimos con el siguiente*/
            otro = in_cpu -> siguiente_q;
        }
        strcpy(in_cpu -> estado, "re");
        strcpy(otro -> estado, "ru");
        return otro;
    } 
    else {  
         /*3er caso: hay que ir a la siguiente queue*/
        int idx_queue = buscar_idx(queue, queues, QueueArray);
        if (conteo_procesos(QueueArray, queues) == 1) {
            return in_cpu;
        }
        for (int i=idx_queue+1; i < queues; i++) {
            if (QueueArray[i] -> count > 0) {
                Process* in_cpu_2 = QueueArray[i] -> puntero_inicio;
                return in_cpu_2;
            }   
        }
    }
}

void concat(LinkedList* pegante, LinkedList* pegado) {
    if (pegante -> count == 0) {
        if (pegado -> count != 0) {
            pegante -> puntero_inicio = pegado -> puntero_inicio;
            pegante -> puntero_final = pegado -> puntero_final;
        }
    } else {
        if (pegado -> count != 0) { /*Caso normal, ninguno es null*/
            pegante -> puntero_final -> siguiente_q = pegado -> puntero_inicio;
            pegante -> puntero_final = pegado -> puntero_final;
        }         
    }
    pegante -> count += pegado -> count;    
}

void linkedlist_imprimir(LinkedList * list);

void subir_prioridades(int queues, LinkedList* QueueArray[queues], Process* in_cpu) {
    for (int i=1; i < queues; i++) {
        concat(QueueArray[0], QueueArray[i]);
    }
}

        
LinkedList* queue_anterior(LinkedList* queue, LinkedList* QueueArray[]) {
    LinkedList * prev;
    LinkedList* cur = QueueArray[0];
    int i = 0;
    while (cur != queue) {
        i++;
        prev = cur;
        cur  = QueueArray[i];
    }
    return prev;
}


Process* round_robin(LinkedList* queue, int quantum, LinkedList* QueueArray[], Process* in_cpu, int t, int queues, int v3) {

    if (in_cpu && in_cpu -> cur_quantum == -1) {
        if (v3) {
            in_cpu -> cur_quantum = quantum * (buscar_idx(queue, queues, QueueArray) + 1); 
        } else {
            in_cpu -> cur_quantum = quantum; 
        }
    }

    /*Caso 0: no hay nadie en CPU y tiene que entrar alguien*/
    if (in_cpu == NULL) {
        printf("Caso 0\n");
        in_cpu = queue -> puntero_inicio;
        if (in_cpu -> response_t == -1) { /*Nunca ha entrado*/
            in_cpu -> response_t = t - in_cpu -> start_time;
            in_cpu -> elegido_cpu++;
        }
        strcpy(in_cpu -> estado, "ru");
        return in_cpu;
    }
    else {
        in_cpu -> cur_burst_value--;
        in_cpu -> cur_quantum--;
        in_cpu -> processing_t++;
        printf("cur_quantum: %d; cur_burst_value: %d\n", in_cpu -> cur_quantum, in_cpu -> cur_burst_value);
    }

    
    /*Caso se me acaba proceso*/
    if (in_cpu -> cur_burst_value == 0 && in_cpu -> cur_burst_idx == in_cpu -> count - 1) {
        printf("Tiempo término %d %s\n", t, in_cpu -> name);
        in_cpu -> turnaround_t = t - in_cpu -> start_time;
        in_cpu -> waiting_t = in_cpu -> turnaround_t - in_cpu -> processing_t;
        Process* in_cpu2 = encontrar_siguiente_proceso(in_cpu, queue, QueueArray, quantum, t, queues);
        if (in_cpu != in_cpu2) {
            in_cpu2 -> elegido_cpu++;
        }
        strcpy(in_cpu -> estado, "fi");
        linkedlist_remove(queue, in_cpu, 1);
        return in_cpu2;
    }


    /*Caso sigo o se acaba burst*/
    if (in_cpu -> cur_quantum > 0) {
        if (in_cpu -> cur_burst_value == 0) {
            in_cpu -> cur_burst_idx++;         /*Siguiente burst*/
            in_cpu -> cur_burst_value = in_cpu -> array[in_cpu -> cur_burst_idx];
            in_cpu -> interrups++;
            Process* in_cpu2 = encontrar_siguiente_proceso(in_cpu, queue, QueueArray, quantum, t, queues);
            in_cpu2 -> elegido_cpu++;
            if (in_cpu2 -> response_t == -1) { /*Nunca ha entrado: no estoy seguro que sea necesario en este caso*/
                in_cpu2 -> response_t = t - in_cpu2 -> start_time;
            }
            return in_cpu2;
        } else {
            return in_cpu;
        }
    }

    /*Caso se me acaba quantum*/
    if (in_cpu -> cur_quantum == 0) {
        in_cpu -> interrups++;
        if (in_cpu -> cur_burst_value == 0) {
            in_cpu -> cur_burst_idx++;
            in_cpu -> cur_burst_value = in_cpu -> array[in_cpu -> cur_burst_idx];
        }
        Process* in_cpu2 = encontrar_siguiente_proceso(in_cpu, queue, QueueArray, quantum, t, queues);
        bajar_prioridad(in_cpu, queue, QueueArray, quantum, queues, v3);
        if (in_cpu2 -> response_t == -1) { /*Nunca ha entrado: no estoy seguro que sea necesario en este caso*/
            in_cpu2 -> response_t = t - in_cpu2 -> start_time;
        }
        in_cpu2 -> elegido_cpu++;
        return in_cpu2;
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
void imprimir_colas(LinkedList * QueueArray[], int total){
    for (int i= 0; i < total; i++){
        
        if (QueueArray[i] -> count == 0){
            printf("Cola %i: vacia\n",i);
        }
        else{
            printf("Cola %i tiene los procesos: \n", i);
            linkedlist_imprimir(QueueArray[i]);
        }
    }

}



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
            // linkedlist_imprimir(list);
			Process* prev;
			while (cur != process) {
				prev = cur;
				cur = cur -> siguiente_q;
			}
            if (cur == list -> puntero_final) {
                list -> puntero_final = prev;
                prev -> siguiente_q = NULL;
            } else {
                prev -> siguiente_q = cur -> siguiente_q;
            }
		}
	}
	list -> count = list -> count - 1;
}

void linkedlist_imprimir(LinkedList * list){
    Process * curr;
    curr = list -> puntero_inicio;
    while (curr != list -> puntero_final){
        printf("Proceso: %s \n", curr -> name);
        curr = curr -> siguiente_q;
    }
    printf("Proceso: %s \n", curr -> name);
}
/* Termino funciones de linked list */
