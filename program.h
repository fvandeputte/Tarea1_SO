/* Definimos la estructuras process */
struct process {
    int pid;
    char name[255];
    long start_time;
    long count;
    int *array;
    int cur_burst_idx;					/*Índice de burst que toca*/
    int cur_burst_value;				/*Pero si ya lleva un poco de la burst actual, aquí decimos cuánto queda*/
    int cur_quantum;
    struct process * siguiente;         /*Usado para bodega inicial*/
    struct process * siguiente_q;       /*Usado para queue*/
    char estado[2];                     /*estados ru, re, fi */
    int elegido_cpu;
    int interrups;
    int turnaround_t;
    int response_t;
    int processing_t;
    int waiting_t;
    int no_bajar;

};
typedef struct process Process;
/* Fin estructura de procesos */


/* estructura lista ligada */
struct linked_list
{
  Process *puntero_inicio;
  Process *puntero_final;
  int count;
};
typedef struct linked_list LinkedList;
/* Fin estructura de lista_ligada */


/*Estructura cola */
struct queue
{
    /* le definimos una lista de punteros de procesos, inicialemtente de tamaño 8 */
    /*Process** process_list = malloc(8*sizeof(Process *));*/
    
};
/*Fin de estructura queue */



LinkedList * input_read(char path[]);                           /*declaramos funcion para leer*/


void revisar_llegadas(LinkedList * puntero_bodega, int t, LinkedList* queue);      /*revisar llegadas */




/*Funciones de process */
Process* process_init(int pid, char * name, int  start_time, int  count, int * lista, LinkedList * pointer_lista);
/* Fin funciones de process */




/* Funciones de Linked List */
LinkedList* linkedlist_init();
void linkedlist_append(LinkedList* list, Process* process, int Q); /*Q = 0 es append en bodega, 1 en Queue*/
void linkedlist_remove(LinkedList* list, Process* process, int Q); /*Q = 0 es append en bodega, 1 en Queue*/
/* Fin funciones de linked list */



Process* round_robin(LinkedList* queue, int quantum, LinkedList* QueueArray[], Process* in_cpu, int t, int queues, int v3);
Process* encontrar_siguiente_proceso(Process* in_cpu, LinkedList* queue, LinkedList* QueueArray[], int quantum, int t, int queues);
Process* wrapper_rr(int queues, LinkedList* QueueArray[queues], int quantum, Process* in_cpu, int t, int v3);
