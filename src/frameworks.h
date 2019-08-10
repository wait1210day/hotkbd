#ifndef __FRAMEWORKS_H__
#define __FRAMEWORKS_H__

#include <linux/input.h>
#include "preprocessor.h"

void respondFramework(char const *destName, int destCode, char const *srcName, int devfd, struct input_event *src, optionTable *opts);
void montiorFramework(char const *srcName, struct input_event *src, optionTable *opts);

#endif // __FRAMEWORKS_H__