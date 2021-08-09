#include "test_mimiCom.h"
#include "mimiCom.h"

int32_t TEST_mimiCom(int32_t isShow)
{
    MimiCom *com = New_mimiCom(NULL);

    com->getChar(com, 'a');
    com->getChar(com, 'a');
    com->getChar(com, 'a');
    com->getChar(com, '\r');
    com->getChar(com, '\n');

    char *RxSingleLine = args_getStr(com->args, "RxSingleLine");
    if (isShow)
    {
        printf("Single Line is: %s\r\n", RxSingleLine);
    }

    com->deinit(com);
    return 0;
}