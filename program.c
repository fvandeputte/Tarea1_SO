#include "program.h"
#include <string.h>
/* Aqui haremos el init de process */
Process * process_init(int pid, char nombre[256]){
    Process *pointer;
    pointer = malloc(sizeof(Process));
    pointer -> pid = pid;
    strncpy(pointer -> nombre, nombre, 256);
    printf("Creando proceso de nombre: %s ", pointer -> nombre);
    printf("y pid: %i \n", pointer -> pid);
}




/* Aqui leemos el los programas de inputs */
void input_read(char *path){
    FILE *fp1;
    char buff[255];
    fp1= fopen (path, "r");
    char * pch;
    int pid = 1;
    while (fgets(buff, 255, (FILE*)fp1) != NULL){
        printf("%s\n", buff );
        pch = strtok (buff," ");
        process_init(pid, pch);
        pid += 1;
    }

}
