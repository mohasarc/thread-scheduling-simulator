#include "../run_queue/linked_list.h"
#include "../run_queue/pcb.h"
#include "../defns.h"
#include <stdio.h>

/**
 * Bursts served in the order they are added to the runqueue.
 */
int get_using_FCFS(struct PCB_DATA **pcb_data, struct LL_NODE ** HEAD, struct LL_NODE ** TAIL){
    return LL_pop(pcb_data, HEAD, TAIL);
}

/**
 * the burst that has the smallest burst length is selected 
 * to run next. But the bursts of a particular thread must be
 * scheduled in the same order they are generated. Hence you 
 * look to the earliest burst of each thread and select the 
 * shortest one.
 */
int get_using_SJF(struct PCB_DATA **pcb_data, struct LL_NODE ** HEAD, struct LL_NODE ** TAIL){
    struct LL_NODE *cur = *TAIL; // The earliest inserted
    struct LL_NODE *shortest_burst_node = *TAIL;

    while (cur)
    {
        if (cur->data->t_index != shortest_burst_node->data->t_index && cur->data->b_length < shortest_burst_node->data->b_length){
            shortest_burst_node->data = cur->data;
        }

        cur = cur->prev;
    }

    if (shortest_burst_node){
        *pcb_data = shortest_burst_node->data;
        LL_remove(shortest_burst_node, HEAD, TAIL);
        return TRUE;
    } else {
        return FALSE;
    }
}


// int get_using_PRIO(struct PCB_DATA **pcb_data, struct LL_NODE ** HEAD, struct LL_NODE ** TAIL){
//     // return LL_pop(pcb_data, HEAD, TAIL);
// }

// int get_using_VRUNTIME(struct PCB_DATA **pcb_data, struct LL_NODE ** HEAD, struct LL_NODE ** TAIL){
//     // return LL_pop(pcb_data, HEAD, TAIL);
// }