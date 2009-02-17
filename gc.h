
#define TYPE(obj) ((obj)->type)
#define VALUE(obj) ((obj)->value)
#define FORWARD(obj) ((obj)->forward)
#define OBJECT_SET(obj,t,m,v) do {\
    TYPE(obj) = (t);\
    VALUE(obj).m = (v);\
} while(0)
#define IVALUE(obj) (VALUE(obj).ivalue)
#define IVALUE_SET(obj,v) OBJECT_SET(obj,T_INT,ivalue,v)
#define FVALUE(obj) (VALUE(obj).fvalue)
#define FVALUE_SET(obj,v) OBJECT_SET(obj,T_FLOAT,fvalue,v)
#define STR(obj) (VALUE(obj).str)
#define STR_LEN(obj) (STR(obj).length)
#define STR_BUF(obj) (STR(obj).buffer)
#define STR_SET(obj,len,buf) do {\
    TYPE(obj) = T_STR;\
    STR_LEN(obj) = len;\
    STR_BUF(obj) = buf;\
} while (0)
#define PAIR(obj) (VALUE(obj).pair)
#define PAIR_CAR(obj) (PAIR(obj).car)
#define PAIR_CDR(obj) (PAIR(obj).cdr)
#define PAIR_CAAR(obj) (PAIR(obj).car.car)
#define PAIR_CADR(obj) (PAIR(obj).cdr.car)
#define PAIR_CDAR(obj) (PAIR(obj).car.cdr)
#define PAIR_CDDR(obj) (PAIR(obj).cdr.cdr)
#define PAIR_SET(obj,a,d) do {\
    TYPE(obj) = T_PAIR;\
    PAIR_CAR(obj) = a;\
    PAIR_CDR(obj) = d;\
} while (0)

typedef struct String String;
typedef struct Pair Pair;
typedef enum Type Type;
typedef union Value Value;
typedef struct Object *Object;
typedef struct Array Array;
typedef struct Memory Memory;

struct String {
    size_t length;
    char *buffer;
};
struct Pair {
    Object car, cdr;
};
enum Type {
    T_INT, T_FLOAT, T_STR,
    T_PAIR
};
union Value {
    long ivalue;
    double fvalue;
    String str;
    Pair pair;
};
struct Object {
    Type type;
    Value value;
    Object forward;
};
struct Array {
    size_t size;
    Object head, current, tail;
};
struct Memory {
    Array *stack, *heap;
    Array h1, h2, s1;
};

Array *fixed_array_init(Array *array, size_t size);
Object fixed_array_pop(Array *array);
Object fixed_array_push(Array *array, Object obj);

Memory *fixed_memory_init(Memory *mem, size_t hsize, size_t ssize);
size_t fixed_memory_sweep(Memory *mem);
Object fixed_memory_alloc(Memory *mem);
Object fixed_memory_push(Memory *mem, Object obj);
Object fixed_memory_pop(Memory *mem);
void fixed_memory_printinfo(Memory *mem);

/* vim: set sw=4 ts=4 et: */
