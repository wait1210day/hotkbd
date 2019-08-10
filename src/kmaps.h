#ifndef __KMAPS_H__
#define __KMAPS_H__

typedef struct _kBindMap
{
    char *name;
    int code;
} kbindMap;

int searchKMap (char *name);
char *desearchKMap (int code);

#endif