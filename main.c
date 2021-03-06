#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "program.c"


LinkedList* bodega_global;
int T;

void imprimir_estadisticas() {

    printf("Procesos terminados: %d\n", bodega_global -> count);
    printf("Tiempo total: %d\n", T-1); /*Esto tiene que cambiar*/

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

int calc_max_start(LinkedList* bodega) {
    int max = 0;
    Process* cur = bodega -> puntero_inicio;
    while (cur != NULL) {
        if (max < cur -> start_time) {
            max = cur -> start_time;
        }
        cur = cur -> siguiente;
    }
    return max;
}

int todas_vacias(int queues, LinkedList* QueueArray[]) {
    for (int i=0; i < queues; i++) {
        if (QueueArray[i] -> count > 0) {
            return 0;
        }
    }
    return 1;
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
    int s;
    if (argc > 5){
        if (strcmp(version, "v1") == 0) { /*tirar error*/
            printf("Error: estamos en v1, y se dio argumento S\n");
            return 0;
        } else {
            s = atoi(argv[5]);
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
        Process* in_cpu = (Process*) NULL;                      /*algún proceso basura para la primera iteración*/
        int max_start_time = calc_max_start(bodega);
        int t = 0;
        T = t;
        while (t <= max_start_time || !todas_vacias(queues, QueueArray)) {
            // sleep(1);
            revisar_llegadas(bodega, t, QueueArray[0]);
            in_cpu = wrapper_rr(queues, QueueArray, quantum, in_cpu, t, 0);
            // imprimir_colas(QueueArray, queues); 
            if (in_cpu == NULL) {
                printf("Tiempo %d: proceso en CPU es NULL\n\n", t);
            } else {
                printf("Tiempo %d: proceso en CPU es %s\n\n", t, in_cpu -> name);
            }
            t++;
            T = t;
        }

        imprimir_estadisticas();
    } else if (strcmp(version, "v2") == 0) {
        Process* in_cpu = (Process*) NULL;                     /*algún proceso basura para la primera iteración*/
        int max_start_time = calc_max_start(bodega);
        printf("Max start time is %d\n", max_start_time);
        int t = 0;
        T = t;
        while (t <= max_start_time || !todas_vacias(queues, QueueArray)) {
            // sleep(1);
            revisar_llegadas(bodega, t, QueueArray[0]);
            in_cpu = wrapper_rr(queues, QueueArray, quantum, in_cpu, t, 0);
            // imprimir_colas(QueueArray, queues); 
            if (in_cpu == NULL) {
                printf("Tiempo %d: proceso en CPU es NULL\n\n", t);
            } else {
                printf("Tiempo %d: proceso en CPU es %s\n\n", t, in_cpu -> name);
            }
            if (t % s == 0 && t > 0) { 
                subir_prioridades(queues, QueueArray, in_cpu);
                in_cpu -> no_bajar = 1;
            }
            t++;
            T = t;
        }

        imprimir_estadisticas();
    } else {                                                /* v3 */
        Process* in_cpu = (Process*) NULL;                  /*algún proceso basura para la primera iteración*/
        int max_start_time = calc_max_start(bodega);
        int t = 0;
        T = t;
        while (t <= max_start_time || !todas_vacias(queues, QueueArray)) {
            // sleep(1);
            revisar_llegadas(bodega, t, QueueArray[0]);
            in_cpu = wrapper_rr(queues, QueueArray, quantum, in_cpu, t, 1);
            // imprimir_colas(QueueArray, queues); 
            if (in_cpu == NULL) {
                printf("Tiempo %d: proceso en CPU es NULL\n\n", t);
            } else {
                printf("Tiempo %d: proceso en CPU es %s\n\n", t, in_cpu -> name);
            }
            if (t % s == 0 && t > 0) { 
                subir_prioridades(queues, QueueArray, in_cpu);
            }
            t++;
            T = t;
        }
        imprimir_estadisticas();
    }
}