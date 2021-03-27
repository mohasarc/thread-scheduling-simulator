/*------------------------------------------
                Linked List
------------------------------------------*/
#ifndef RQ_H
#define RQ_H
#include "./b.h"

struct LL_NODE
{
    int index;
    struct B_DATA *data;
    struct LL_NODE *next;
    struct LL_NODE *prev;
};

// FUNCTIONS' PROTOTYPES
void LL_push(struct B_DATA *data, struct LL_NODE **HEAD, struct LL_NODE **TAIL);
void LL_shift(struct B_DATA *data, struct LL_NODE **HEAD, struct LL_NODE **TAIL);
int LL_pop(struct B_DATA **data, struct LL_NODE **HEAD, struct LL_NODE **TAIL);
int LL_unshift(struct B_DATA **data, struct LL_NODE **HEAD, struct LL_NODE **TAIL);
void LL_remove(struct LL_NODE *bToRemove, struct LL_NODE **HEAD, struct LL_NODE **TAIL);
void LL_remove_by_value(struct B_DATA *data, struct LL_NODE **HEAD, struct LL_NODE **TAIL);
void LL_print(struct LL_NODE* HEAD, struct LL_NODE* TAIL);
#endif