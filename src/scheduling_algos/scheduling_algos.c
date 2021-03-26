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
            shortest_burst_node = cur;
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

/**
 * A W thread i has priority i. The smaller the number, the 
 * higher the priority. The burst in the runqueue belonging 
 * to the highest priority thread is selected. Again, we can 
 * not reorder the bursts of a particular thread. We have look 
 * to the earliest bursts of each thread in the runqueue and 
 * select the one that has highest priority.
 */
int get_using_PRIO(struct PCB_DATA **pcb_data, struct LL_NODE ** HEAD, struct LL_NODE ** TAIL){
    struct LL_NODE *cur = *TAIL; // The earliest inserted
    struct LL_NODE *h_priority_burst_node = *TAIL;

    while (cur)
    {
        if (cur->data->t_index != h_priority_burst_node->data->t_index && cur->data->t_index < h_priority_burst_node->data->t_index){
            h_priority_burst_node = cur;
        }

        cur = cur->prev;
    }

    if (h_priority_burst_node){
        *pcb_data = h_priority_burst_node->data;
        LL_remove(h_priority_burst_node, HEAD, TAIL);
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * Each thread is associated with a virtual runtime (vruntime). 
 * When a burst of a thread is executed, the vruntime of the 
 * thread is advanced by some amount. The amount depends on the 
 * priority of the thread. When thread i runs t ms, its virtual 
 * runtime is advanced by t(0.7 + 0.3i). For example, virtual 
 * runtime of thread 1 is advanced by t ms when it runs t ms in 
 * cpu; the virtual runtime of thread 11 is advanced by 4t when 
 * it runs t ms in cpu. When scheduling decision is to be made, 
 * the burst of the thread that has the smallest virtual runtime 
 * is selected for execution. Again we can not reorder the bursts 
 * of a particular thread. They need to be served in the order 
 * they are added to the runqueue.
 */
// int get_using_VRUNTIME(struct PCB_DATA **pcb_data, struct LL_NODE ** HEAD, struct LL_NODE ** TAIL){
//     // return LL_pop(pcb_data, HEAD, TAIL);
// }