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
    int quantum = argv[3];
    /*queues */
    int queues = atoi(argv[4]);
    /* manejar el error en caso de que estemos en v1 y haya <S> */
    if (argc > 5){
        if (strcmp(version, "v1") == 0) { /*tirar error*/
            printf("Error: estamos en v1, y se dio argumento S\n");
            return 0;
        } else {
            int s = argv[5];
            // hacer versión 3
        }
        
    }
    LinkedList * bodega;
    LinkedList QueueArray[queues];


    bodega = input_read(path);
    /*iniciar simulacion */

    if (strcmp(version, "v1") == 0) {
        int t=0;

        for (t; t < 40; t++){   /* Aqui falta ponerle una condicion de término */
            revisar_llegadas(bodega, t);
        }
    }
    

}