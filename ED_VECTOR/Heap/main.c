
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"

typedef struct
{
    char nome[32];
    char cat[32];
    int id;
    int prioridade;
} Proc;

Proc *proc_constructor(char *nome, char *cat, int id, int prioridade)
{
    Proc *proc = (Proc *)malloc(sizeof(Proc));

    strcpy(proc->nome, nome);
    strcpy(proc->cat, cat);
    proc->id = id;
    proc->prioridade = prioridade;

    return proc;
}

void proc_destructor(void *proc)
{
    free(proc);
}

int proc_cmp(const void *a, const void *b)
{
    Proc *proc_a = (Proc *)a;
    Proc *proc_b = (Proc *)b;

    if (proc_a->prioridade < proc_b->prioridade)
        return -1;

    if (proc_a->prioridade > proc_b->prioridade)
        return 1;

    return 0;
}

int main()
{
    int n;

    Heap *heap = heap_constructor(proc_cmp);

    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        char nome[32];
        char cat[32];
        int id;
        int prioridade;

        scanf(" %s %s %d %d", nome, cat, &id, &prioridade);

        heap_push(heap, proc_constructor(nome, cat, id, prioridade));
    }

    while (heap_size(heap) > 0)
    {
        Proc *proc = (Proc *)heap_pop(heap);

        printf("%s %s %d %d\n",
               proc->nome,
               proc->cat,
               proc->id,
               proc->prioridade);

        proc_destructor(proc);
    }
    heap_destroy(heap);

    return 0;
}
