#include <stdlib.h>
#include <stdio.h>

/**
 * The Ready Queue implentation (as a linked list)
 * */
// 
struct PCB
{
    int index;
    struct PCB* next;
    struct PCB* prev;
};

// FUNCTION PROTOTYPES
void LL_push(int index, struct PCB **HEAD, struct PCB **TAIL);
void LL_shift(int index, struct PCB **HEAD, struct PCB **TAIL);
void LL_pop(int *index, struct PCB **HEAD, struct PCB **TAIL);
void LL_unshift(int *index, struct PCB **HEAD, struct PCB **TAIL);
void LL_remove(struct PCB *pcbToRemove, struct PCB **HEAD, struct PCB **TAIL);
void LL_remove_by_value(int index, struct PCB **HEAD, struct PCB **TAIL);
void LL_print(struct PCB* HEAD, struct PCB* TAIL);

// FUNCTION IMPLEMENTATION
/**
 * Inserts a new item to the end of the linked list
 * @param index The value to be inserted
 * @param HEAD Pointer to the head of the linked list
 * @param TAIL Pointer to the tail of the linked list
 * */
void LL_push(int index, struct PCB **HEAD, struct PCB **TAIL){
    if (!*HEAD){
        // Empty list
        // Create new node
        *HEAD = (struct PCB*)malloc(sizeof(struct PCB));

        // Set its values
        (*HEAD)->next   = NULL;
        (*HEAD)->prev   = NULL;
        (*HEAD)->index  = index;

        // Set the tail pointer
        *TAIL = *HEAD;
    } else {
        // List not empty
        // Create new node
        (*TAIL)->next = (struct PCB*)malloc(sizeof(struct PCB));

        // Set its values
        (*TAIL)->next->next = NULL;
        (*TAIL)->next->prev = *TAIL;
        (*TAIL)->next->index = index;

        // Move the tail pointer
        *TAIL = (*TAIL)->next;
    }
}

/**
 * Inserts a new item to the begining of the linked list
 * @param index The value to be inserted
 * @param HEAD Pointer to the head of the linked list
 * @param TAIL Pointer to the tail of the linked list
 * */
void LL_shift(int index, struct PCB **HEAD, struct PCB **TAIL){
    if (!*HEAD){
        LL_push(index, HEAD, TAIL);
    } else {
        // List not empty
        // Create new node
        struct PCB* newNode = (struct PCB*)malloc(sizeof(struct PCB));

        // Set its data
        newNode->next = (*HEAD);
        newNode->prev = NULL;
        newNode->index = index;

        // Update the head
        (*HEAD)->prev = newNode;
        (*HEAD) = newNode;
    }
}

/**
 * remove the last item and return its value
 * @param index The value to be inserted
 * @param HEAD Pointer to the head of the linked list
 * @param TAIL Pointer to the tail of the linked list
 * */
void LL_pop(int *index, struct PCB **HEAD, struct PCB **TAIL){
    struct PCB *nodeToRemove;

    if (TAIL){
        // Return the value
        *index = (*TAIL)->index;

        // Remove the node
        nodeToRemove = (*TAIL);
        (*TAIL) = (*TAIL)->prev;
        if ((*TAIL)){
            (*TAIL)->next = NULL;
        }

        // Check if it was the last node
        if (nodeToRemove == (*HEAD)){
            (*HEAD) = NULL;
        }

        // Dealocate the memory of the deleted node
        free(nodeToRemove);
    }

    // if (*TAIL){
    //     *index = (*TAIL)->index;
    //     LL_remove(*TAIL, HEAD, TAIL);
    // }
}

/**
 * remove the first item and return its value
 * @param index The value to be inserted
 * @param HEAD Pointer to the head of the linked list
 * @param TAIL Pointer to the tail of the linked list
 * */
void LL_unshift(int *index, struct PCB **HEAD, struct PCB **TAIL){
    struct PCB *itemToRemove;

    if (HEAD){
        // Return the value
        *index = (*HEAD)->index;

        // Remove the node
        itemToRemove = *HEAD;
        (*HEAD) = (*HEAD)->next;
        if (*HEAD){
            (*HEAD)->prev = NULL;
        }

        // Dealocate the memory
        free(itemToRemove);
    }

    // if (*HEAD){
    //     *index = (*HEAD)->index;
    //     LL_remove(*HEAD, HEAD, TAIL);
    // }
}

/**
 * @param pcbToRemove Pointer to the node to be removed
 * @param HEAD Pointer to the head of the linked list
 * @param TAIL Pointer to the tail of the linked list
 * */
void LL_remove(struct PCB *pcbToRemove, struct PCB **HEAD, struct PCB **TAIL){
    struct PCB *cur = *HEAD;

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
 * @param index The value to be removed
 * @param HEAD Pointer to the head of the linked list
 * @param TAIL Pointer to the tail of the linked list
 * */
void LL_remove_by_value(int index, struct PCB **HEAD, struct PCB **TAIL){
    struct PCB *cur = *HEAD;

    while (cur)
    {
        if(cur->index == index){
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
void LL_print(struct PCB* HEAD, struct PCB* TAIL){
    struct PCB* cur = HEAD;

    while (cur)
    {
        printf("%d\n", cur->index);
        cur = cur->next;
    }
}

int main(){
    // Pointers to the linked list head/ tail
    struct PCB* LINKED_LIST_HEAD = NULL;
    struct PCB* LINKED_LIST_TAIL = NULL;

    // 
    // LL_push(4, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
    // LL_push(3, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
    // LL_push(42, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
    // LL_push(8, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
    int removedValue;
    
    LL_shift(4, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
    LL_shift(3, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
    LL_shift(42, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
    LL_shift(8, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);

    LL_unshift(&removedValue, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
    // LL_pop(&removedValue, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
    // LL_pop(&removedValue, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
    LL_pop(&removedValue, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);

    LL_unshift(&removedValue, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
    LL_pop(&removedValue, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
    // LL_unshift(&removedValue, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
    LL_unshift(&removedValue, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
  
  
    // LL_remove(LINKED_LIST_HEAD, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);
    // LL_remove(LINKED_LIST_TAIL, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);

    // LL_remove_by_value(555, &LINKED_LIST_HEAD, &LINKED_LIST_TAIL);

    LL_print(LINKED_LIST_HEAD, LINKED_LIST_TAIL);
    printf("removed value: %d\n", removedValue);
}