#ifndef _VECTOR_H_ED
#define _VECTOR_H_ED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void* data_type;

typedef struct Vector Vector;

// Aloca espaço para um elemento do tipo vector e inicializa os seus atributos.
Vector* vector_construct();

// Libera o espaço reservado para o vector.
void vector_destroy(Vector* v); //,void(*delete_data)(Vector*));

// Adiciona o item dado ao final do vector e aumenta o tamanho. Aumenta a
// capacidade alocada se necessário.
void vector_push_back(Vector* v, data_type val);

// Retorna o número de elementos atualmente no vector.
int vector_size(Vector* v);

// Retorna o i-ésimo elemento do vector.
data_type vector_get(Vector* v, int i);

// Atualiza o i-ésimo elemento do vector  para que ele passe a ter o valor val.
void vector_set(Vector* v, int i, data_type val);

// Retorna o índice da primeira ocorrência do item no Vector ou -1 se o elemento
// não for encontrado.
int vector_find(Vector* v, data_type val,int(*comp_fn)(const void*,const void*));

// Retorna o maior elemento do vector (assumindo que podemos comparar elementos
// usando o operador "<")
data_type vector_max(Vector* v, int(*comp_fn)(const void*,const void*));

// Retorna o menor elemento do vector (assumindo que podemos comparar elementos
// usando o operador "<")
data_type vector_min(Vector* v,int(*comp_fn)(const void*,const void*));

// Retorna o índice do maior elemento do vector (assumindo que podemos comparar
// elementos usando o operador "<")
int vector_argmax(Vector* v,int(*comp_fn)(const void*,const void*));

// Retorna o índice do menor elemento do vector (assumindo que podemos comparar
// elementos usando o operador "<")
int vector_argmin(Vector* v,int(*comp_fn_)(const void*,const void*));

void vector_save(Vector* v, char* f,void (*write_item)(FILE*,data_type));

Vector* vector_load(char* f,data_type (*load_items)(FILE*));

void vector_save_binary(Vector* v, char* f,void (*write_item)(FILE*,data_type));

Vector* vector_load_binary(char* f,data_type (*load_items)(FILE*));

void vector_save_strings(FILE* f, char* data);

char* vector_load_strings(FILE* f);

void vector_destroy_strings(Vector* v);

// Remove o i-ésimo elemento do vetor.
data_type vector_remove(Vector *v, int i);

// Remove o primeiro elemento
data_type vector_pop_front(Vector *v);

// Remove o ultimo elemento
data_type vector_pop_back(Vector *v);

// Insere o elemento na i-esima posicao
void vector_insert(Vector *v, int i, data_type val);

// Troca os elementos das posições i e j (i vira j e j vira i)
void vector_swap(Vector *v, int i, int j);

// Ordena o vetor in-place (sem criar um novo vetor)
void vector_sort(Vector *v,int(*comp_fn_sort)(void*,void*));

// Retorna o indice de val usando busca binaria. Retorna -1 se nao encontrado.
int vector_binary_search(Vector *v, data_type val,int(*comp_fn_sort)(const void*,const void*));

// Inverte o vetor in-place (sem criar um novo vetor)
void vector_reverse(Vector *v);

// Cria uma cópia do vector e dos valores de seus atributos.
Vector *vector_copy(Vector *v);

// Remove todos os elementos de v
void vector_clear(Vector *v);

Vector* vector_find_all(Vector *v, data_type val);

#endif