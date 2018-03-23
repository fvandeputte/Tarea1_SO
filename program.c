#include "program.h"
/* Aqui haremos el init de process */




/* Aqui leemos el los programas de inputs */
void input_read(char *path){
    FILE *fp1;
    char buff[255];
    fp1= fopen (path, "r");
    fgets(buff, 255, (FILE*)fp1);
    printf("1: %s\n", buff );
    fgets(buff, 255, (FILE*)fp1);
    printf("2: %s\n", buff );

}
