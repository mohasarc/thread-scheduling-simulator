/*------------------------------------------
                Linked List
------------------------------------------*/
#ifndef RQ_H
#define RQ_H
#include "./pcb.h"

struct LL_NODE
{
    int index;
    struct PCB_DATA *data;
    struct LL_NODE *next;
    struct LL_NODE *prev;
};

// FUNCTIONS' PROTOTYPES
void LL_push(struct PCB_DATA *data, struct LL_NODE **HEAD, struct LL_NODE **TAIL);
void LL_shift(struct PCB_DATA *data, struct LL_NODE **HEAD, struct LL_NODE **TAIL);
int LL_pop(struct PCB_DATA **data, struct LL_NODE **HEAD, struct LL_NODE **TAIL);
int LL_unshift(struct PCB_DATA **data, struct LL_NODE **HEAD, struct LL_NODE **TAIL);
void LL_remove(struct LL_NODE *pcbToRemove, struct LL_NODE **HEAD, struct LL_NODE **TAIL);
void LL_remove_by_value(struct PCB_DATA *data, struct LL_NODE **HEAD, struct LL_NODE **TAIL);
void LL_print(struct LL_NODE* HEAD, struct LL_NODE* TAIL);
#endif