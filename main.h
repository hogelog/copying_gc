#include "gc.h"

Object new_ivalue(Memory *mem, long ivalue);
Object new_fvalue(Memory *mem, double fvalue);
Object new_string(Memory *mem, const char *str);
Object new_pair(Memory *mem, Object car, Object cdr);
Object push_ivalue(Memory *mem, long ivalue);
Object push_fvalue(Memory *mem, double fvalue);
Object push_string(Memory *mem, const char *str);
Object push_pair(Memory *mem, Object car, Object cdr);

/* vim: set sw=4 ts=4 et: */
