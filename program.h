/* Definimos la estructuras process */
struct process {
    int pid;
    char * name;
    long start_time;
    long count;
    int *array;
    
    struct process * siguiente; /*Usado para bodega inicial*/
    /*estados ru, re, fi */
    char estado[2];

};
typedef struct process Process;

struct linked_list
{
  Process *puntero_inicio;
  Process *puntero_final;
  int count;
};
typedef struct linked_list LinkedList;




struct queue
{
    /* le definimos una lista de punteros de procesos, inicialemtente de tamaño 8 */
    /*Process** process_list = malloc(8*sizeof(Process *));*/
    
};

LinkedList* linkedlist_init();

/*declaramos funcion para leer*/
void input_read(char path[]);

/*declaramos funcion para iniciar y retornar su puntero */
Process* process_init(int pid, char * name, int  start_time, int  count, int * lista, LinkedList * pointer_lista);

void linkedlist_append(LinkedList* list, Process* process);