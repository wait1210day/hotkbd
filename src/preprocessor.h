#ifndef __PREPROCESSOR_H__
#define __PREPROCESSOR_H__

#include "kbindList.h"
#include <stdio.h>
#include <stdbool.h>

#define RUNAS_APPLICATION 1
#define RUNAS_DAEMON      2
#define OPERAT_BIND       1
#define OPERAT_MONITOR    2

typedef struct _optionTable
{
    int runas;
    FILE *monitorFile;
    bool quiet;
    bool continuousTrigger;
    char device[1024];
} optionTable;

void parseConfig (char const *file, kbindList *kbindTable, optionTable *opts);
int deadloopCheck (kbindList *kbindTable);

#endif // __PREPROCESSOR_H__