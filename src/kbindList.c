#include "kbindList.h"
#include <stdlib.h>

void createKBindList (kbindList **head)
{
    (*head) = (kbindList *) malloc (sizeof(kbindList));
    (*head)->next = (kbindList *) malloc (sizeof(kbindList));
    (*head)->next->next = NULL;
}

void insertKBindList (kbindList *head, int trigger, int opeation, int target)
{
    kbindList *list = head;
    
    while (list->next != NULL)
    {
        list = list->next;
    }
    list->trigger = trigger;
    list->target = target;
    list->operation = opeation;
    list->next = (kbindList *) malloc (sizeof(kbindList));
    list = list->next;
    list->next = NULL;
}

kbindList *searchKBindList (kbindList *head, int trigger)
{
    kbindList *list = head->next;
    while (list->next != NULL)
    {
        if (list->trigger == trigger)
            return list;
        list = list->next;
    }

    return NULL;
}

void destoryKBindList (kbindList *head)
{
    kbindList *list = head;

    while (list->next != NULL)
    {
        kbindList *bak = list;
        free (list);
        list = bak->next;
    }
}

bool isemptyList (kbindList *head)
{
    if (head->next->next == NULL)
        return true;
    return false;
}