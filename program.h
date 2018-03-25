/* Definimos la estructuras process */
struct process {
    int pid;
    char nombre[256];
    /*estados ru, re, fi */
    char estado[2];
    
};
typedef struct process Process;
struct queue
{
    /* le definimos una lista de punteros de procesos, inicialemtente de tamaño 8 */
    /*Process** process_list = malloc(8*sizeof(Process *));*/
    
};

/*declaramos funcion para leer*/
void input_read(char path[]);

/*declaramos funcion para iniciar y retornar su puntero */
Process* process_init(int pid, char nombre[256]);