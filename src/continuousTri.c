#include "continuousTri.h"
#include "kmaps.h"
#include "renderer.h"
#include <stdio.h>

int lastTg = -1;
int lastTri = -1;

bool catchContinuousTriEvent (kbindList *current)
{
    if (lastTg == current->trigger)
    {
        dri (
            DCR_CAUGHT,
            "refused continuous-trigger (%s -> %s -> %s)\n",
            desearchKMap (lastTri), desearchKMap (lastTg),
            desearchKMap (current->target)
        );
        lastTg = -1;
        lastTri = current->trigger;
        
        return true;
    }
    return false;
}

void updateContinuousTriFlag (kbindList *current)
{
    lastTg = current->target;
    lastTri = current->trigger;
}