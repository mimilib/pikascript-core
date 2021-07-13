#ifndef __MIMI_SHELL2_H
#define __MIMI_SHELL2_H
#include "dataMemory.h"
#include "dataArgs.h"

typedef struct Class_mimiShell2 Shell;
#define SHELL2_CMD_NAME_LENGTH 32
#define SHELL2_CMD_LENGTH 256

int strGetArgs(char *CMD, char **argv);

struct Class_mimiShell2
{
    /* attribute */
    DMEM *mem;
    Args *mapList;

    void *context;

    /* override */
    void (*_config)(Shell *self);
    void *(*_detector)(Shell *self, void *(*fun_d)(Shell *self, char *, void *(fun)(Shell *self, int, char **)),
                       char *CMD,
                       void *(fun)(Shell *self, int argc, char **argv));
};

/* operation */
void *shell_cmd(Shell *self, char *);
void shell_addMap(Shell *self, char *, void *(*)(Shell *shell, int argc, char **argv));
void shell_deinit(Shell *self);

Shell *New_shell(Args *initArgs);

#endif
