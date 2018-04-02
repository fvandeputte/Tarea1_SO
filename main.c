#include <stdio.h>
#include <string.h>
#include "program.c"



int main( int argc, char * argv [] ) {
    printf("Cantidad de argumentos: %i\n", argc);
    /*Aqui leemos y clasificamos el inpit */
    char *version = argv[1];
    printf("Version: %s \n", version);
    /* path */
    char *path = argv[2];
    /* quantum */
    int quantum = atoi(argv[3]);
    /*queues */
    int queues = atoi(argv[4]);
    /* manejar el error en caso de que estemos en v1 y haya <S> */
    if (argc > 5){
        if (strcmp(version, "v1") == 0) { /*tirar error*/
            printf("Error: estamos en v1, y se dio argumento S\n");
            return 0;
        } else {
            int s = atoi(argv[5]);
            // hacer versiones 2 y 3
        }
        
    }
    LinkedList * bodega;
    LinkedList* QueueArray[queues];
    /*Crear cada queue con puntero_inicio = NULL*/
    for (int i=0; i < queues; i++) {
        QueueArray[i] = create_queue();
    }

    bodega = input_read(path);
    
    /*iniciar simulacion */

    if (strcmp(version, "v1") == 0) {
        Process* in_cpu = (Process*) NULL; /*algún proceso basura para la primera iteración*/
        for (int t=0; t < 40; t++){   /* Aqui falta ponerle una condicion de término */
            in_cpu = revisar_llegadas(bodega, t, queues, QueueArray, quantum, in_cpu);
            if (in_cpu == NULL) {
                printf("Tiempo %d: proceso en CPU es NULL\n", t);
            } else {
                printf("Tiempo %d: proceso en CPU es %d\n", t, in_cpu -> pid);
            }
            
        }
    }
    

}