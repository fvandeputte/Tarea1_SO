#include <stdio.h>
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
        int s = argv[5];
        char *v1 = 'v1';
    }
    input_read(path);




   
}