#include "shApp_print.h"
#include "dataMemory.h"
#include "dataString.h"
#include "mimiProcess.h"
#include "shApp_argSet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#define ROOT_PTR argv[argc - 1]
#define PROCESS_DIR argv[1]
#define PRINT_NAME argv[2]

void *app_print(int argc, char **argv)
{
    DMEM *memOut = DynMemGet(sizeof(char) * 256);
    ((char *)(memOut->addr))[0] = 0;
    mimiProcess_t *root = (mimiProcess_t *)atoi(ROOT_PTR);
    mimiProcess_t *processNow = goToProcess(root, PROCESS_DIR);

    if (NULL == processNow)
    {
        strPrint(memOut->addr, "[error: process no found]\r\n");
        return (void *)memOut;
    }

    char *printStr = NULL;
    printStr = processNow->argPrint(processNow, PRINT_NAME);
    memcpy(memOut->addr, printStr, ARG_CONTANT_LENGTH);
    strPrint(memOut->addr, "\r\n");
    return (void *)memOut;
}
