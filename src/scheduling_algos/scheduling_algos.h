#ifndef SCHEDULING_ALGOS_H
#define SCHEDULING_ALGOS_H
int get_using_FCFS(struct B_DATA **b_data, struct LL_NODE ** HEAD, struct LL_NODE ** TAIL);
int get_using_SJF(struct B_DATA **b_data, struct LL_NODE ** HEAD, struct LL_NODE ** TAIL);
int get_using_PRIO(struct B_DATA **b_data, struct LL_NODE ** HEAD, struct LL_NODE ** TAIL);
int get_using_VRUNTIME(struct B_DATA **b_data, struct LL_NODE ** HEAD, struct LL_NODE ** TAIL);
#endif