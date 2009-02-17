#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc.h"

Array *fixed_array_init(Array *array, size_t size) {
    array->head = malloc(sizeof(struct Object)*size);
    if(!array->head) return NULL;
    array->current = array->head;

    array->size = size;
    array->tail = array->head+array->size;
    return array;
}
Object fixed_array_pop(Array *array) {
    if(array->head == array->current) {
        fprintf(stderr, "array underflow");
        return NULL;
    }
    return array->current--;
}
Object fixed_array_push(Array *array, Object obj) {
    if(array->current == array->tail) {
        fprintf(stderr, "array overflow");
        return NULL;
    }
    *array->current = *obj;
    return array->current++;
}

Memory *fixed_memory_init(Memory *mem, size_t hsize, size_t ssize) {
    if(!fixed_array_init(&mem->h1, hsize)) return NULL;
    if(!fixed_array_init(&mem->h2, hsize)) return NULL;
    mem->heap = &mem->h1;
    if(!fixed_array_init(&mem->s1, ssize)) return NULL;
    mem->stack = &mem->s1;
    return mem;
}
/* Cheney compacting Algorithm */
#define COPY_TO(obj) do {\
    if(FORWARD(obj)==NULL) {\
        Object *forward = &FORWARD(obj);\
        obj = fixed_array_push(to, obj);\
        *forward = obj;\
    } else {\
        obj = FORWARD(obj);\
    }\
} while (0)
static void copy_space(Array *from, Array *to) {
    Object scanned = from->head;
    while(scanned!=from->current) {
        switch(TYPE(scanned)) {
            case T_PAIR:
                COPY_TO(PAIR_CAR(scanned));
                COPY_TO(PAIR_CDR(scanned));
                break;
            default:
                break;
        }
        ++scanned;
    }
}
size_t fixed_memory_sweep(Memory *mem) {
    Array *stack = mem->stack;
    Array *from = mem->heap;
    Array *to = from==&mem->h1 ? &mem->h2 : &mem->h1;
    Object scanned;

    to->current = to->head;

    scanned = from->head;
    while(scanned!=from->current) {
        FORWARD(scanned) = NULL;
        ++scanned;
    }
    /* copy object referenced from stack */
    copy_space(stack, to);
    
    copy_space(to, to);

    scanned = from->head;
    while(scanned!=from->current) {
        if(FORWARD(scanned)==NULL && TYPE(scanned)==T_STR) {
                free(STR_BUF(scanned));
        }
        ++scanned;
    }

    mem->heap = to;
    return (from->current-from->head) - (to->current-to->head);
}
Object fixed_memory_alloc(Memory *mem) {
    Array *heap = mem->heap;
    if(heap->current+1 >= heap->tail) {
        size_t freed = fixed_memory_sweep(mem);
        if(freed == 0) {
            fprintf(stderr, "heap overflow\n");
            return NULL;
        }
    }
    return heap->current++;
}
Object fixed_memory_push(Memory *mem, Object obj) {
    return fixed_array_push(mem->stack, obj);
}
Object fixed_memory_pop(Memory *mem) {
    return fixed_array_pop(mem->stack);
}
void fixed_memory_printinfo(Memory *mem) {
    Array *heap = mem->heap, *stack = mem->stack;
    printf("heap: %d / %d\n",
            heap->current - heap->head, heap->size);
    printf("stack: %d / %d\n",
            stack->current - stack->head, stack->size);
}

/* vim: set sw=4 ts=4 et: */
