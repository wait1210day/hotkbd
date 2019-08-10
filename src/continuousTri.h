#ifndef __CONTINUOUSTRI_H__
#define __CONTINUOUSTRI_H__

#include <stdbool.h>
#include "kbindList.h"

bool catchContinuousTriEvent (kbindList *current);
void updateContinuousTriFlag (kbindList *current);

#endif // __CONTINUOUSTRI_H__