#include "frameworks.h"
#include "renderer.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

void respondFramework(char const *destName, int destCode, char const *srcName, int devfd, struct input_event *src, optionTable *opts)
{
    struct input_event respond;

    memcpy (&(respond.time), &(src->time), sizeof(struct timeval));
    respond.code = destCode;
    respond.type = src->type;
    respond.value = src->value;
    write (devfd, &respond, sizeof(struct input_event));
    if (src->value == 1 && !opts->quiet)
    {
        dri (DCR_TRIGGER, "%s -> %s (Pressed)\n", srcName, destName);
    }
    else if (src->value == 0)
    {
        if (!opts->quiet)
            dri (DCR_TRIGGER, "%s -> %s (Free)\n", srcName, destName);
        respond.code = 0;
        respond.type = 0;
        respond.value = 0;
        write (devfd, &respond, sizeof(struct input_event));
    }
    else if (src->value == 2 && !opts->quiet)
    {
        dri (DCR_TRIGGER, "%s -> %s (LongPressed)\n", srcName, destName);
    }
}

void montiorFramework(char const *srcName, struct input_event *src, optionTable *opts)
{
    int i = 0;
    time_t timep;
    time (&timep);
    char *localTime = ctime (&timep);
    while (localTime[i] != '\n')
        i++;
    localTime[i] = '\0';

    int value = src->value;
    int code = src->code;
    switch (value)
    {
    case 1:
        fprintf (opts->monitorFile, "[%s] Monitor: %s<%d> Pressed\n", localTime, srcName , code);
        break;
    case 0:
        fprintf (opts->monitorFile, "[%s] Monitor: %s<%d> Free\n", localTime, srcName , code);
        break;
    case 2:
        fprintf (opts->monitorFile, "[%s] Monitor: %s<%d> LongPressed\n", localTime, srcName , code);
        break;
    
    default:
        break;
    }
}