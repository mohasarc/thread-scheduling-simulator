#include <stdlib.h>
#include <stdio.h>
#include "./linked_list.h"
#include "./defns.h"

/**
 * Inserts a new item to the end of the linked list
 * @param data The value to be inserted
 * @param HEAD Pointer to the head of the linked list
 * @param TAIL Pointer to the tail of the linked list
 * */
void LL_push(struct PCB_DATA *data, struct LL_NODE **HEAD, struct LL_NODE **TAIL){
    if (!*HEAD){
        // Empty list
        // Create new node
        *HEAD = (struct LL_NODE*)malloc(sizeof(struct LL_NODE));

        // Set its values
        (*HEAD)->next   = NULL;
        (*HEAD)->prev   = NULL;
        (*HEAD)->data  = data;

        // Set the tail pointer
        *TAIL = *HEAD;
    } else {
        // List not empty
        // Create new node
        (*TAIL)->next = (struct LL_NODE*)malloc(sizeof(struct LL_NODE));

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
void LL_shift(struct PCB_DATA *data, struct LL_NODE **HEAD, struct LL_NODE **TAIL){
    if (!*HEAD){
        LL_push(data, HEAD, TAIL);
    } else {
        // List not empty
        // Create new node
        struct LL_NODE* newNode = (struct LL_NODE*)malloc(sizeof(struct LL_NODE));

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
int LL_pop(struct PCB_DATA **data, struct LL_NODE **HEAD, struct LL_NODE **TAIL){
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
int LL_unshift(struct PCB_DATA **data, struct LL_NODE **HEAD, struct LL_NODE **TAIL){
    if (*HEAD){
        *data = (*HEAD)->data;
        LL_remove(*HEAD, HEAD, TAIL);
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @param pcbToRemove Pointer to the node to be removed
 * @param HEAD Pointer to the head of the linked list
 * @param TAIL Pointer to the tail of the linked list
 * */
void LL_remove(struct LL_NODE *pcbToRemove, struct LL_NODE **HEAD, struct LL_NODE **TAIL){
    struct LL_NODE *cur = *HEAD;

    while (cur)
    {
        if(cur == pcbToRemove){
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
void LL_remove_by_value(struct PCB_DATA *data, struct LL_NODE **HEAD, struct LL_NODE **TAIL){
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
        printf("t_index: %d, b_length: %d, b_index: %d, wall_clock_time: %d\n", cur->data->t_index, cur->data->b_length, 
                                                                                cur->data->b_index, cur->data->wall_clock_time);
        cur = cur->next;
    }
}

// int main(){
//     // Pointers to the linked list head/ tail
//     struct LL_NODE* LINKED_LIST_HEAD = NULL;
//     struct LL_NODE* LINKED_LIST_TAIL = NULL;

//     // 
//     // LL_push(4, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
//     // LL_push(3, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
//     // LL_push(42, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
//     // LL_push(8, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
//     struct PCB_DATA *removedValue = NULL;

//     struct PCB_DATA * data1 = create_pcb_data(1, 3, 5, 21);
//     struct PCB_DATA * data2 = create_pcb_data(4, 1, 6, 12);    
//     struct PCB_DATA * data3 = create_pcb_data(5, 2, 5, 13);
//     struct PCB_DATA * data4 = create_pcb_data(2, 1, 5, 23);
    
//     LL_shift(data1, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
//     LL_shift(data2, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
//     LL_shift(data3, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
//     LL_shift(data4, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);

//     LL_unshift(&removedValue, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
//     // LL_pop(&removedValue, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
//     // LL_pop(&removedValue, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
//     // LL_pop(&removedValue, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);

//     // LL_unshift(&removedValue, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
//     // LL_pop(&removedValue, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
//     // LL_unshift(&removedValue, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
//     // LL_unshift(&removedValue, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
  
  
//     // LL_remove(LINKED_LIST_HEAD, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
//     // LL_remove(LINKED_LIST_TAIL, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);

//     // LL_remove_by_value(555, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);

//     LL_print(LINKED_LIST_HEAD, LINKED_LIST_TAIL);
//     printf("removed value: %d\n", removedValue->b_index);

//     destroy_pcb(data1);
//     destroy_pcb(data2);
//     destroy_pcb(data3);
//     destroy_pcb(data4);
// }