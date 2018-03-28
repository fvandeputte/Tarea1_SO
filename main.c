#include <stdio.h>
#include "program.c"

void spend_quantum(Process* cur, LinkedList* queue) {
    if (cur -> cur_quantum > cur -> cur_burst_value) { /*Si no te vas a gastar todo el quantum que te queda...*/
        cur -> cur_quantum -= cur -> cur_burst_value;
        cur->cur_burst_idx ++;
        if (cur -> count > cur -> cur_burst_idx) { /*Hay más bursts*/
            cur -> cur_burst_value = cur -> array[cur->cur_burst_idx];
        } else { /*No hay más bursts*/
            printf("Se terminó proceso de pid %d y nombre %s\n", cur -> pid, cur -> name);
            linkedlist_remove(queue, cur);
        }
    } else {
        cur -> cur_burst_value -= cur -> cur_quantum;
        cur -> cur_quantum = 0;
        // bajar_prioridad();
    }
}


void round_robin(LinkedList* queue, int quantum) {
    while (queue -> puntero_inicio != NULL) {
        Process* cur = queue -> puntero_inicio;
        if (cur -> cur_quantum == -1) { /*lo estoy seteando acá, parte siendo -1*/
            cur -> cur_quantum = quantum;
        }
        spend_quantum(cur);
        if (cur == queue -> puntero_final) { /*Si estamos al final, volver al principio*/
            cur = queue -> puntero_inicio;
        } else { /*avanzar al siguiente en otro caso*/
            cur = cur -> siguiente;
        }
    }
}



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
    LinkedList * bodega;
    LinkedList QueueArray[queues];


    bodega = input_read(path);
    /*iniciar simulacion */
    int t=0;

    for (t; t < 40; t++){   /* Aqui falta ponerle una condicion de término */
        revisar_llegadas(bodega, t);
    }





   
}