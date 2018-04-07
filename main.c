#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "program.c"


LinkedList* bodega_global;

void imprimir_estadisticas() {

    printf("Procesos terminados: %d\n", bodega_global -> count);
    printf("Tiempo total: %d\n", 40); /*Esto tiene que cambiar*/

    Process* cur = bodega_global -> puntero_inicio;
    while (cur != NULL) {
        printf("\n");
        printf("%s\n", cur -> name);
        printf("Turnos de CPU: %d\n", cur -> elegido_cpu);
        printf("Bloqueos: %d\n", cur -> interrups);
        printf("Turnaround time: %d\n", cur -> turnaround_t);
        printf("Response time: %d\n", cur -> response_t);
        printf("Waiting time: %d\n", cur -> waiting_t);
        cur = cur -> siguiente;
    }
    exit(0);

}

int main( int argc, char * argv [] ) {

    signal(SIGINT, imprimir_estadisticas);

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
    bodega_global = bodega;
    
    /*iniciar simulacion */

    if (strcmp(version, "v1") == 0) {
        Process* in_cpu = (Process*) NULL; /*algún proceso basura para la primera iteración*/
        for (int t=0; t < 120; t++){   /* Aqui falta ponerle una condicion de término */
            sleep(2);
            revisar_llegadas(bodega, t, QueueArray[0]);
            in_cpu = wrapper_rr(queues, QueueArray, quantum, in_cpu, t);
            if (in_cpu == NULL) {
                printf("Tiempo %d: proceso en CPU es NULL\n\n", t);
            } else {
                printf("Tiempo %d: proceso en CPU es %s\n\n", t, in_cpu -> name);
            }
            
        }

        imprimir_estadisticas(bodega);

    }
    

}