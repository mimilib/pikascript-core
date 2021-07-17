#ifndef METHOD__H
#define METHOD__H
#include "dataArgs.h"
void method_returnStr(Args *args, char *val);
void method_returnInt(Args *args, int val);
void method_returnFloat(Args *args, float val);
void method_returnPtr(Args *args, void *val);
void method_sysOut(Args *args, char *str);
#endif