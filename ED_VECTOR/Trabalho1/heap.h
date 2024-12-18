
#ifndef _HEAP_H_
#define _HEAP_H_

typedef struct Heap Heap;

Heap *heap_constructor(int (*cmp_fn)(const void *, const void *));
void heap_push(Heap *heap, void *data);
int heap_size(Heap *heap);
void *heap_pop(Heap *heap);
void heap_destroy(Heap *heap);
void* heap_get(Heap* heap, int i);

#endif