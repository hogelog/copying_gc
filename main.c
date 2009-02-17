#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc.h"
Object new_ivalue(Memory *mem, long ivalue) {
    Object new = fixed_memory_alloc(mem);
    if(!new) return NULL;
    IVALUE_SET(new, ivalue);
    return new;
}
Object new_fvalue(Memory *mem, double fvalue) {
    Object new = fixed_memory_alloc(mem);
    if(!new) return NULL;
    FVALUE_SET(new, fvalue);
    return new;
}
Object new_string(Memory *mem, const char *str) {
    Object new = fixed_memory_alloc(mem);
    if(!new) return NULL;
    STR_SET(new, strlen(str)+1, strdup(str));
    return new;
}
Object new_pair(Memory *mem, Object car, Object cdr) {
    Object new = fixed_memory_alloc(mem);
    if(!new) return NULL;
    PAIR_SET(new, car, cdr);
    return new;
}

Object push_ivalue(Memory *mem, long ivalue) {
    struct Object new;
    IVALUE_SET(&new, ivalue);
    return fixed_memory_push(mem, &new);
}
Object push_fvalue(Memory *mem, double fvalue) {
    struct Object new;
    FVALUE_SET(&new, fvalue);
    return fixed_memory_push(mem, &new);
}
Object push_string(Memory *mem, const char *str) {
    struct Object new;
    STR_SET(&new, strlen(str)+1, strdup(str));
    return fixed_memory_push(mem, &new);
}
Object push_pair(Memory *mem, Object car, Object cdr) {
    struct Object new;
    PAIR_SET(&new, car, cdr);
    return fixed_memory_push(mem, &new);
}

void print_object(Object obj) {
    switch(obj->type) {
        case T_INT:
            printf("%ld", IVALUE(obj));
            break;
        case T_FLOAT:
            printf("%f", FVALUE(obj));
            break;
        case T_STR:
            printf("%s", STR_BUF(obj));
            break;
        case T_PAIR:
            putchar('(');
            print_object(PAIR_CAR(obj));
            fputs(" . ", stdout);
            print_object(PAIR_CDR(obj));
            putchar(')');
            break;
        default:
            break;
    }
}

static Memory *memory;
void test_01() {
    Object iv, fv, str, pair;
    int i;
    for (i=0;i<2000;++i) {
        fixed_memory_push(memory,
                iv = new_ivalue(memory, i));
        printf("%p: %ld\n", iv, IVALUE(iv));

        fixed_memory_push(memory,
                fv = new_fvalue(memory, (double)i));
        printf("%p: %f\n", fv, FVALUE(fv));

        fixed_memory_push(memory,
                str = new_string(memory, "hogefuga"));
        printf("%p: %s\n", str, STR_BUF(str));

        fixed_memory_push(memory,
                pair = new_pair(memory, iv, fv));
        printf("%p: (%ld . %f)\n", pair,
                IVALUE(PAIR_CAR(pair)), FVALUE(PAIR_CDR(pair)));
        fixed_memory_pop(memory);
        fixed_memory_pop(memory);
        fixed_memory_pop(memory);
        fixed_memory_pop(memory);
    }
}
void test_02() {
    Object iv_0 = new_ivalue(memory, 0);
    Object pair = push_pair(memory, iv_0, iv_0);
    Object *cdr = &PAIR_CDR(pair);
    int i;
    for (i=1;i<10;++i) {
        Object iv_i = new_ivalue(memory, i);
        (*cdr) = new_pair(memory, iv_i, iv_i);
        cdr = &PAIR_CDR(*cdr);
    }
    printf("%p:", pair);
    print_object(pair);
    putchar('\n');
}

int main(int argc, char **argv) {
    Memory mainmem;
    memory = &mainmem;
    fixed_memory_init(memory, 1024, 1024);
    test_01();
    test_02();
    fixed_memory_printinfo(memory);
    fixed_memory_sweep(memory);
    puts("# run sweep");
    fixed_memory_printinfo(memory);
    return 0;
}
/* vim: set sw=4 ts=4 et: */
