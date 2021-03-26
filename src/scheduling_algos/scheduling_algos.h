#ifndef SCHEDULING_ALGOS_H
#define SCHEDULING_ALGOS_H

int get_using_FCFS(struct PCB_DATA **pcb_data, struct LL_NODE ** HEAD, struct LL_NODE ** TAIL);
int get_using_SJF(struct PCB_DATA **pcb_data, struct LL_NODE ** HEAD, struct LL_NODE ** TAIL);
int get_using_PRIO(struct PCB_DATA **pcb_data, struct LL_NODE ** HEAD, struct LL_NODE ** TAIL);
// int get_using_VRUNTIME(struct PCB_DATA **pcb_data, struct LL_NODE ** HEAD, struct LL_NODE ** TAIL);

#endif