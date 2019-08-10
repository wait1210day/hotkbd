#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <strings.h>
#include "hotkbd.h"

int deviceFd = 0;
kbindList *kbindTable;
optionTable globalOpts = {
    .runas = RUNAS_APPLICATION,
    .monitorFile = NULL,
    .quiet = false,
    .continuousTrigger = false
};
void signalInterrupt (int sig);

void signalInterrupt (int sig)
{
    int status = 0;

    if (sig > 0)
        dri (DCR_INT, "process interrupted by signal %d\n", sig);
    else
        status = abs(sig);

    fclose (globalOpts.monitorFile);
    close (deviceFd);
    destoryKBindList (kbindTable);
    exit (status);
}

int main(int argc, char const *argv[])
{
    signal (SIGINT, signalInterrupt);
    signal (SIGTERM, signalInterrupt);

    driInit();

    if (argc < 2)
    {
        dri (DCR_ERROR, "Usage %s [config]\n", argv[0]);
        return 1;
    }

    char const *config = argv[1];
    struct input_event keyEvent;
    kbindList *target;

    createKBindList (&kbindTable);
    parseConfig (config, kbindTable, &globalOpts);

    if (isemptyList (kbindTable))
    {
        dri (DCR_ERROR, "No any policy.\n");
        machineException ("EmptyConfigurationException", "character");
    }
    if (globalOpts.device[0] == 0)
    {
        dri (DCR_ERROR, "No any device.\n");
        machineException ("DeviceNotFoundException", "character");
    }
    if (deadloopCheck(kbindTable) && globalOpts.continuousTrigger)
    {
        dri (DCR_ERROR, "configration might be dead-loop but the option continuous-trigger was enabled\n");
        machineException ("RegularDeadloopException", "character");
    }

    // Open device
    if ((deviceFd = open (globalOpts.device, O_RDWR)) < 0)
    {
        dri (DCR_ERROR, "Could not open device %s\n", globalOpts.device);
        return 1;
    }

    // Read device and responding hot key event
    while (1)
    {
        read (deviceFd, &keyEvent, sizeof(struct input_event));
        target = searchKBindList (kbindTable, keyEvent.code);

        if (target != NULL)
        {
            if (catchContinuousTriEvent (target) && !globalOpts.continuousTrigger)
                continue;

            if (target->operation == OPERAT_BIND)
            {
                respondFramework (
                    desearchKMap(target->target),
                    target->target,
                    desearchKMap(target->trigger),
                    deviceFd,
                    &keyEvent,
                    &globalOpts
                );
            }
            else
            {
                montiorFramework (desearchKMap(target->trigger), &keyEvent, &globalOpts);
            }

            updateContinuousTriFlag (target);
        }
    }

    fclose (globalOpts.monitorFile);
    destoryKBindList (kbindTable);
    close (deviceFd);
    return 0;
}
