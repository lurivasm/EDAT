
#include "table.h"

int main(void){
  type_t *tipos;

  tipos = (type_t*)malloc(sizeof(type_t)*4);
  if(!tipos){
    printf("Error reservando memoria para tipos\n");
    return -1;
  }
  tipos[0] = INT; /*Score*/
  tipos[1] = STR; /*Título*/
  tipos[2] = STR; /*ISBN*/
  tipos[3] = INT; /*LibroId*/

  printf("Creando tabla score de 4 columnas: Score, Título, ISBN, LibroId\n");
  table_create("score.dat", 4, tipos);

  free(tipos);
  return 0;
}
