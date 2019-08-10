#ifndef __RENDERER_H__
#define __RENDERER_H__ 1

// DCL means 'Direct Console Rendering'
#define DCR_ERROR   1
#define DCR_TRIGGER 2
#define DCR_INT     3
#define DCR_WARNING 4
#define DCR_CAUGHT  5
#define DCR_ECPT    6
#define DCR_INFO    7

#include <stdio.h>

void driInit();
void driRedirector(unsigned short target, FILE *stream);
void dri(unsigned short type, char *fmt, ...);
void machineException (char const *exceptionStr, char const *code);

#endif // __RENDERER_H__