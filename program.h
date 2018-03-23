/* Definimos la estructuras process */
struct process {
    int pid;
    char nombre[256];
    /*estados ru, re, fi */
    char estado[2];
};
typedef struct process Process;


/*declaramos funcion para leer*/
void input_read(char path[]);