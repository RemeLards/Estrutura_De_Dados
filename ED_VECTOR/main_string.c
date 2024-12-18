#include "vector.h"
#include <stdio.h>

int main(void)
{
  char* read_file_name = "strings.txt";
  char* save_file_name = "strings.txt";
  char str_salve[6]="salve";
  Vector *v = vector_load(read_file_name,vector_load_strings);

  for (int i = 0; i < vector_size(v); i++) {
    printf("frase %d: %s\n", i, vector_get(v, i));
  }


  data_type str = (char*) malloc(sizeof(char) * (128+1));
  for(int i = 0; i < 6; i++)
  {
    str[i] = str_salve[i];
  }
  vector_push_back(v, str);

  vector_save(v, save_file_name, vector_save_strings);
  
  vector_destroy(v,vector_destroy_strings);

  printf("OK!\n");
  return 0;
}