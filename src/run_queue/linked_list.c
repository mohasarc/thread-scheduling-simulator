#include <stdlib.h>
#include <stdio.h>
#include "./linked_list.h"
#include "../defns.h"

/**
 * Inserts a new item to the end of the linked list
 * @param data The value to be inserted
 * @param HEAD Pointer to the head of the linked list
 * @param TAIL Pointer to the tail of the linked list
 * */
void LL_push(struct B_DATA *data, struct LL_NODE **HEAD, struct LL_NODE **TAIL){
    if (!*HEAD){
        // Empty list
        // Create new node
        // *HEAD = (struct LL_NODE*)malloc(sizeof(struct LL_NODE));
        *HEAD = (struct LL_NODE*)calloc(1, sizeof(struct LL_NODE));

        // Set its values
        (*HEAD)->next   = NULL;
        (*HEAD)->prev   = NULL;
        (*HEAD)->data  = data;

        // Set the tail pointer
        *TAIL = *HEAD;
    } else {
        // List not empty
        // Create new node
        // (*TAIL)->next = (struct LL_NODE*)malloc(sizeof(struct LL_NODE));
        (*TAIL)->next = (struct LL_NODE*)calloc(1, sizeof(struct LL_NODE));

        // Set its values
        (*TAIL)->next->next = NULL;
        (*TAIL)->next->prev = *TAIL;
        (*TAIL)->next->data = data;

        // Move the tail pointer
        *TAIL = (*TAIL)->next;
    }
}

/**
 * Inserts a new item to the begining of the linked list
 * @param data The value to be inserted
 * @param HEAD Pointer to the head of the linked list
 * @param TAIL Pointer to the tail of the linked list
 * */
void LL_shift(struct B_DATA *data, struct LL_NODE **HEAD, struct LL_NODE **TAIL){
    if (!*HEAD){
        LL_push(data, HEAD, TAIL);
    } else {
        // List not empty
        // Create new node
        // struct LL_NODE* newNode = (struct LL_NODE*)malloc(sizeof(struct LL_NODE));
        struct LL_NODE* newNode = (struct LL_NODE*)calloc(1, sizeof(struct LL_NODE));

        // Set its data
        newNode->next = (*HEAD);
        newNode->prev = NULL;
        newNode->data = data;

        // Update the head
        (*HEAD)->prev = newNode;
        (*HEAD) = newNode;
    }
}

/**
 * remove the last item and return its value
 * @param data The value was just removed
 * @param HEAD Pointer to the head of the linked list
 * @param TAIL Pointer to the tail of the linked list
 * @return Removal success status
 * */
int LL_pop(struct B_DATA **data, struct LL_NODE **HEAD, struct LL_NODE **TAIL){
    if (*TAIL){
        *data = (*TAIL)->data;
        LL_remove(*TAIL, HEAD, TAIL);
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * remove the first item and return its value
 * @param data The value was just removed
 * @param HEAD Pointer to the head of the linked list
 * @param TAIL Pointer to the tail of the linked list
 * @return Removal success status
 * */
int LL_unshift(struct B_DATA **data, struct LL_NODE **HEAD, struct LL_NODE **TAIL){
    if (*HEAD){
        *data = (*HEAD)->data;
        LL_remove(*HEAD, HEAD, TAIL);
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @param bToRemove Pointer to the node to be removed
 * @param HEAD Pointer to the head of the linked list
 * @param TAIL Pointer to the tail of the linked list
 * */
void LL_remove(struct LL_NODE *bToRemove, struct LL_NODE **HEAD, struct LL_NODE **TAIL){
    struct LL_NODE *cur = *HEAD;

    while (cur)
    {
        if(cur == bToRemove){
            if (cur->prev){
                cur->prev->next = cur->next;
            } else {
                // It is the head
                (*HEAD) = cur->next;
            }

            if (cur->next){
                cur->next->prev = cur->prev;
            } else {
                // It is the tail
                (*TAIL) = cur->prev;
            }

            free(cur);
            break;
        }
        cur = cur->next;
    }
}

/**
 * @param data The value to be removed
 * @param HEAD Pointer to the head of the linked list
 * @param TAIL Pointer to the tail of the linked list
 * */
void LL_remove_by_value(struct B_DATA *data, struct LL_NODE **HEAD, struct LL_NODE **TAIL){
    struct LL_NODE *cur = *HEAD;

    while (cur)
    {
        if(isDataEqual(cur->data, data)){
            LL_remove(cur, HEAD, TAIL);
            break;
        }
        cur = cur->next;
    }
}

/**
 * Prints the linked list values
 * @param HEAD Pointer to the head of the linked list
 * @param TAIL Pointer to the tail of the linked list
 * */
void LL_print(struct LL_NODE* HEAD, struct LL_NODE* TAIL){
    struct LL_NODE* cur = HEAD;

    while (cur)
    {
        printf("t_index: %d, b_length: %d, b_index: %d, wall_clock_time: %ld, vruntime: %d\n", cur->data->t_index, cur->data->b_length, 
                                                                                cur->data->b_index, cur->data->wall_clock_time, cur->data->vruntime);
        cur = cur->next;
    }
}