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
	LinkedList* queue;
	Process* aux;
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
        Process* p = process_init(pid, name, start_time, n, lista_enteros, puntero_bodega); 

    }
    return puntero_bodega;
}
/* Fin lectura archivo */

void bajar_prioridad(Process* cur, LinkedList* queue, LinkedList* QueueArray[], int quantum) {
	int n = sizeof(QueueArray) / sizeof(QueueArray[0]);
	LinkedList* queue_check = QueueArray[0];
	int y = 0;
	while (queue_check != queue) {
		queue_check = QueueArray[y++];
	}
	linkedlist_remove(queue, cur, 1);
	linkedlist_append(QueueArray[y+1], cur, 1);
	cur -> cur_quantum = quantum;
	printf("Movimos de queue n° %d a %d\n", y, y+1);
}

Process* round_robin(LinkedList* queue, int quantum, LinkedList* QueueArray[], Process* in_cpu) {
	Process* cur = queue -> puntero_inicio;
    if (cur -> cur_quantum == -1) {
        cur -> cur_quantum = quantum;
    }
    /*Caso 0: no hay nadie en CPU y tiene que entrar alguien*/
    if (in_cpu == NULL) {
        if (queue -> count > 0) { /*No es necesario: revisar_llegada lo asegura así*/
            printf("Caso 0\n");
            return queue -> puntero_inicio;
        }
    }

    while (cur != in_cpu) {
        cur = cur -> siguiente_q;
    }

    printf("cur_quantum: %d; cur_burst_value: %d\n", cur -> cur_quantum, cur -> cur_burst_value);
    /*1er caso: todavía queda quantum y queda algún burst, sigue el mismo en CPU ()*/
    if (cur -> cur_quantum > 0 && (cur -> cur_burst_value > 0 || cur -> cur_burst_idx < cur -> count - 1)) {
        printf("En caso 1\n");
        cur -> cur_quantum--;
        if (cur -> cur_burst_value > 0) {   /*En este burst*/
            cur -> cur_burst_value--;
        } else {
            cur -> cur_burst_idx++;         /*Siguiente burst*/
            cur -> cur_burst_value = cur -> array[cur -> cur_burst_idx];
        }
        //sumar a tiempo de procesamiento 1
        return cur;
    }

    /*2do caso y 3er caso: se acabó quantum y/o proceso*/

    Process* otro;
    if (queue -> count > 1) {                   /*2do caso: hay más procesos en esta queue*/
        if (cur == queue -> puntero_final) {    /*Caso 2-a: tenemos que volver a principio queue*/
            printf("En caso 2-a\n");
            otro = queue -> puntero_inicio;
        } else {                                /*Caso 2-b: seguimos con el siguiente*/
            printf("En caso 2-b\n");
            otro = cur -> siguiente_q;
        }
        cur -> interrups++;                     /*Sumamos 1 a interrupciones del proceso*/
        strcpy(cur -> estado, "re");
        if (cur -> cur_burst_value != 0 || cur -> cur_burst_idx == cur -> count - 1) { /*Proceso no ha terminado*/
            bajar_prioridad(cur, queue, QueueArray, quantum);
        }
        return otro;
    } else {                                    /*3er caso: hay que ir a la siguiente queue*/
        printf("En caso 3\n");
        int qq = 0;

        queue = QueueArray[qq];
        while (queue -> count == 0) {
            if (qq == sizeof(QueueArray) / sizeof(QueueArray[0]) - 1) {     /*Última queue y no hay nadie*/
                return (Process*) NULL;
            } else {
                qq++;
                queue = QueueArray[qq];
            }
        }
        strcpy(cur -> estado, "fi");
        if (cur != NULL) {
            if (cur -> cur_quantum == 0 && cur -> cur_burst_value > 0) { /*Estamos acá porque se acabó el quantum, no el proceso*/
                bajar_prioridad(cur, queue, QueueArray, quantum);
                strcpy(cur -> estado, "fi");
                cur = queue -> puntero_inicio;
                strcpy(cur -> estado, "ru");
                return cur;
            }
        } else {
            return (Process*) NULL;
        }
           
    }
}



/*Funcion que revisa llegadas */
Process* revisar_llegadas(LinkedList * puntero_bodega, int t, int queues, LinkedList* QueueArray[queues], int quantum, Process* in_cpu)
{ 

    int k;
    Process *puntero_actual;
    Process *puntero_siguiente;
    puntero_actual = puntero_bodega -> puntero_inicio;
    for (k = 0; k < puntero_bodega -> count; k++){
        puntero_siguiente = puntero_actual -> siguiente;
        if (puntero_actual -> start_time == t){
            printf("Process with name %s, start at %li\n",puntero_actual -> name, puntero_actual -> start_time);
            linkedlist_append(QueueArray[0], puntero_actual, 1); 
        }
        puntero_actual = puntero_siguiente;
    }

    for (int i=0; i < queues; i++) {
    	// printf("ACA, llamando a round_robin en queue %d, que tiene %d elemento(s)\n", i, QueueArray[i] -> count);
        if (QueueArray[i] -> count > 0) {
            printf("Llamando a round_robin en queue %d, la primera con al menos un elemento\n", i);
            Process* in_cpu_2 = round_robin(QueueArray[i], quantum, QueueArray, in_cpu);
            return in_cpu_2;
        }
    	
    }

    return (Process*) NULL;
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
			if (list -> count == 1) {
				list -> puntero_final = list -> puntero_inicio;
			}
		} else {
			Process* prev;
			while (cur != process) {
				prev = cur;
				cur -> siguiente_q;
			}
			prev -> siguiente = cur -> siguiente_q;
		}
	}
	list -> count--;
}

/* Termino funciones de linked list */
