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
    char estado[2];                     /*estados ru, re, fi */

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


void revisar_llegadas(LinkedList * puntero_bodega, int t);      /*revisar llegadas */




/*Funciones de process */
Process* process_init(int pid, char * name, int  start_time, int  count, int * lista, LinkedList * pointer_lista);
/* Fin funciones de process




/* Funciones de Linked List */
LinkedList* linkedlist_init();
void linkedlist_append(LinkedList* list, Process* process);
void linkedlist_remove(LinkedList* list, Process* process);
/* Fin funciones de linked list */

