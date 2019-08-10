#ifndef __KBINDLIST_H__
#define __KBINDLIST_H__

#include <stdbool.h>

typedef struct _kBindLinkedlist
{
    int trigger;
    int operation;
    int target;
    struct _kBindLinkedlist *next;
} kbindList;

void createKBindList (kbindList **head);
void insertKBindList (kbindList *head, int trigger, int opeation, int target);
kbindList *searchKBindList (kbindList *head, int trigger);
void destoryKBindList (kbindList *head);
bool isemptyList (kbindList *head);

#endif // __KBINDLIST_H__