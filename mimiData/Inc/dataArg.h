#ifndef _arg__H
#define _arg__H
#include "dataMemory.h"

#define ARG_CONTANT_LENGTH 256
#define ARG_NAME_LENGTH 32
#define ARG_TYPE_LENGTH 32

typedef struct Class_arg arg_t;
struct Class_arg
{
    /* attribute */
    DMEM *mem;
    void *context;

    DMEM *contantDynMem;
    DMEM *nameDynMem;
    DMEM *typeDynMem;

    /* operation */
    void (*dinit)(arg_t *self);
    void (*init)(arg_t *self, void *voidPointer);

    void (*setName)(arg_t *self, char *name);
    void (*setContant)(arg_t *self, char *contant, int size);
    void (*setType)(arg_t *self, char *type);

    void (*setInt64)(arg_t *self, long long val);
    void (*setFloat)(arg_t *self, float val);
    void (*setPointer)(arg_t *self, void *pointer);
    void (*setString)(arg_t *self, char *string);

    long long (*getInt64)(arg_t *self);
    float (*getFloat)(arg_t *self);
    void *(*getPointer)(arg_t *self);
    char *(*getString)(arg_t *self);

    char *(*getContant)(arg_t *self);

    /* virtual operation */

    /* object */
};

arg_t *New_arg(void *voidPointer);
#endif
