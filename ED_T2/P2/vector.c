#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STRING_MAX_SIZE 128
#define INITIAL_ALLOCATED_SPACE 10
#define GROWTH_RATE 2

typedef struct Vector{
  data_type* data;
  int size;
  int allocated;
} Vector;

//Actual Implementation

Vector* vector_construct()
{
  Vector *v = (Vector *)calloc(1, sizeof(Vector));
  v->data = (data_type *)calloc(INITIAL_ALLOCATED_SPACE, sizeof(data_type));
  v->allocated = INITIAL_ALLOCATED_SPACE;
  v->size = 0;
  return v;
}


void vector_destroy(Vector* v)//,void(*delete_data)(Vector*))
{
  if ( v != NULL )
  {
    // delete_data(v);
    free(v->data);
    free(v);
  }
}


void vector_push_back(Vector* v, data_type data)
{
  if ( v != NULL )
  {
    v->size++;
    if (v->size >= v->allocated)
    {
      v->allocated = GROWTH_RATE * v->allocated;
      v->data = (data_type *)realloc((void *)v->data, v->allocated * sizeof(data_type));
    }
    v->data[v->size - 1] = data;
  }
}


int vector_size(Vector* v)
{ 
  if ( v != NULL ) return v->size; 
  else return -1;
}


data_type vector_get(Vector* v, int i)
{
  if ( v != NULL )
  {
    if ( !(i >= 0 && i <= (v->size - 1)) )
    {
      printf("\nIndice Invalido\n");
      exit(1);
    }

    return v->data[i];
  }
  else return NULL;
}


void vector_set(Vector* v, int i, data_type val)
{
  if ( v != NULL )
  {
    if ( !(i >= 0 && i <= (v->allocated - 1)) )
    {
      printf("\nIndice Invalido\n");
      exit(1);
    }

    v->data[i] = val;
  }
}


int vector_find(Vector* v, data_type val,int(*comp_fn)(const void*,const void*))
{
  if ( v != NULL )
  {
    for (int i = 0; i < v->size; i++)
    {
      if( comp_fn(v->data[i],val) == 0 )
      {
        return i;
      }
    }
  }

  return -1;
}


data_type vector_max(Vector* v, int(*comp_fn)(const void*,const void*))
{
  if ( v != NULL )
  {
    data_type max = v->data[0];

    for (int i = 1; i < v->size; i++)
    {
      if( comp_fn(max,v->data[i]) <= -1 )
      {
        max = v->data[i];
      }
    }

    return max;
  }

  return NULL;
}


data_type vector_min(Vector* v,int(*comp_fn)(const void*,const void*))
{
  if ( v != NULL )
  {
    data_type min = v->data[0];

    for (int i = 1; i < v->size; i++)
    {
      if ( comp_fn(min,v->data[i]) >= 1) 
      {
        min = v->data[i];
      }
    }

    return min;
  }

  return NULL;
}


int vector_argmax(Vector* v,int(*comp_fn)(const void*,const void*))
{
  data_type max = vector_max(v,comp_fn);
  return vector_find(v, max,comp_fn);
}


int vector_argmin(Vector* v,int(*comp_fn)(const void*,const void*))
{
  data_type min = vector_min(v,comp_fn);
  return vector_find(v, min,comp_fn);
}


data_type vector_remove(Vector* v, int i)
{
  if ( v != NULL )
  {
    data_type data = v->data[i];

    for (int j = i; j < vector_size(v); j++)
    {
      if(j != vector_size(v)-1) v->data[j] = v->data[j+1];
      else v->data[j] = NULL;
    }

    v->size--;

    return data;
  }
  
  return NULL;
}


data_type vector_pop_front(Vector* v)
{
  return vector_remove(v,0);
}


data_type vector_pop_back(Vector* v)
{
  return vector_remove(v,vector_size(v)-1);
}


void vector_insert(Vector* v, int i, data_type val)
{
  if ( v != NULL )
  {
    if ( (++v->size) >= v->allocated )
    {
      v->allocated = GROWTH_RATE * v->allocated;
      v->data = (data_type *)realloc((void *)v->data, v->allocated * sizeof(data_type));
    }

    //Its better to move the data starting from the last index
    for (int j = vector_size(v)-1; j > i; j--)
    {
      v->data[j] = v->data[j-1];
    }

    v->data[i] = val;
  }
}


void vector_swap(Vector* v, int i, int j)
{
  if ( v != NULL )
  {
    data_type temp_data = v->data[i];
    v->data[i] = v->data[j];
    v->data[j] = temp_data;
  }
}


void vector_sort(Vector* v, int(*comp_fn_sort)(void*,void*))
{
  // Since in the first loop, the greater element will be in pos "vector_size(v)-1",
  // The second loop, the second greater element will be in pos "vector_size(v)-2",
  // There's no need compare both, because they were compared in the first loop,
  // So we can decrease the loop_len every interation, saving some time.

  if ( v != NULL )
  {
    int swap_loop_len = vector_size(v)-1;

    while(1)
    {
      int swap_count = 0;

      for(int i = 0; i < swap_loop_len; i++)
      {
        if ( comp_fn_sort(v->data[i], v->data[i+1]) > 0 )
        {
          vector_swap(v,i,i+1);
          swap_count++;
        }
      }
      
      swap_loop_len--;

      if(swap_count == 0)break;
    }
  }
}


int vector_binary_search(Vector* v, data_type val,int(*comp_fn_sort)(const void*,const void*))
{
  if ( v != NULL )
  {
    int max_range = vector_size(v)-1;
    int min_range = 0;

    while(min_range <= max_range)
    {
      int central_search_index =  min_range + (max_range - min_range)/2;

      if ( comp_fn_sort(vector_get(v,central_search_index),val) == 0) return central_search_index;
      
      else if ( comp_fn_sort(vector_get(v,central_search_index),val) > 0 ) max_range = central_search_index - 1;

      else min_range = central_search_index + 1;

    }
  }

  return -1;
}


void vector_reverse(Vector* v)
{
  if ( v != NULL )
  {
    for ( int i = 0; i < vector_size(v)/2; i++ )
    {
      vector_swap(v,i,(vector_size(v)-1) - i);
    }
  }
}

// //#################################################### Challange  1

// Vector* vector_load(char* file_name, data_type (*load_items)(FILE*))
// {
//   Vector* v = vector_construct();
//   FILE* f = fopen(file_name, "r");
//   data_type data = NULL;

//   if (f == NULL)
//   {
//     printf("Nao foi possivel abrir o arquivo");
//     exit(1);
//   }

//   while((data = load_items(f)) != NULL)
//   {
//     vector_push_back(v,data);
//   }

//   fclose(f);

//   return v;
// }


// void vector_save(Vector* v, char* file_name, void (*write_item)(FILE*,data_type))
// {
//   if ( v != NULL )
//   {
//     FILE* f = fopen(file_name, "w");

//     if (f == NULL)
//     {
//       printf("Nao foi possivel abrir o arquivo");
//       exit(1);
//     }

//     for (int i = 0; i < vector_size(v); i++)
//     {
//       write_item(f,v->data[i]);
//     }

//     fclose(f);
//   }
// }


// Vector* vector_load_binary(char* file_name, data_type (*load_items)(FILE*))
// {
//   Vector* v = vector_construct();
//   FILE* f = fopen(file_name, "rb");
//   data_type data = NULL;

//   if (f == NULL)
//   {
//     printf("Nao foi possivel abrir o arquivo");
//     exit(1);
//   }

//   while( (data = load_items(f)) != NULL )
//   {
//     vector_push_back(v,data);
//   }

//   fclose(f);

//   return v;
// }


// void vector_save_binary(Vector* v, char* file_name, void (*write_item)(FILE*,data_type))
// {
//   if ( v != NULL )
//   {
//     FILE* f = fopen(file_name, "wb");

//     if (f == NULL)
//     {
//       printf("Nao foi possivel abrir o arquivo");
//       exit(1);
//     }

//     for (int i = 0; i < vector_size(v); i++)
//     {
//       write_item(f,v->data[i]);
//     }

//     fclose(f);
//   }
// }


// void vector_save_strings(FILE* f, char* str)
// {
//   int i = 0;
//   while(str[i] != '\0') i++;
  
//   if(str[i-1] == '\n')
//   {
//     fprintf(f, "%s", str); 
//   }
//   else
//   {
//     fprintf(f, "%s\n", str);
//   }
// }


// char* vector_load_strings(FILE* f)
// {
//   char* str = (char*) malloc ( sizeof(char)*(STRING_MAX_SIZE+1) );
  
//   if( fgets(str,STRING_MAX_SIZE+1,f) == NULL )
//   {
//     free(str);
//     return NULL;
//   }
//   return str;
// }


// void vector_destroy_strings(Vector* v)
// {
//   for(int i = 0; i < vector_size(v); i++)free(v -> data[i]);
//   free(v->data);
// }


// //#################################################### Challange  2

// Vector* vector_copy(Vector* v)
// {
//   if ( v != NULL )
//   {
//     Vector* copy_v = (Vector*) calloc(1,sizeof(Vector));
//     copy_v = (Vector*) memcpy( (void*)copy_v, (void*)v , sizeof(Vector));

//     copy_v->data = (data_type*) calloc(v->allocated,sizeof(data_type));
//     copy_v->data = (data_type*) memcpy( (void*)copy_v->data , (void*)v->data  , v->size*sizeof(data_type));

//     return copy_v;
//   }

//   else return v;
// }

// void vector_clear(Vector* v)
// {
//   if ( v != NULL )
//   {
//     free(v->data);
//     v->data = (data_type *)calloc(INITIAL_ALLOCATED_SPACE, sizeof(data_type));
//     v->allocated = INITIAL_ALLOCATED_SPACE;
//     v->size = 0;
//   }
// }

// Vector* vector_find_all(Vector *v, data_type val)
// {
//   if ( v != NULL )
//   {
//     Vector* v_index = vector_construct();

//     for (int i = 0; i < vector_size(v); i++)
//     {
//       if ( v->data[i] == val ) vector_push_back(v_index,i);
//     }

//     return v_index;
//   }

//   else return v;
// }