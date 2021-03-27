#include <stdlib.h>
#include "./pcb.h"
#include "../defns.h"

/**
 * @param t_index Thread index
 * @param b_index Burst index
 * @param b_length Burst length
 * @param wall_clock_time The wall clock time the bust is generated
 * @return Pointer to the PCB data structure
 * */
struct PCB_DATA *create_pcb_data(int t_index, int b_index, int b_length, int wall_clock_time, int vruntime){
    struct PCB_DATA * pcb_data = (struct PCB_DATA*)malloc(sizeof(struct PCB_DATA));
    pcb_data->t_index         = t_index;
    pcb_data->b_index         = b_index;
    pcb_data->b_length        = b_length;
    pcb_data->wall_clock_time = wall_clock_time;
    pcb_data->vruntime        = vruntime;

    return pcb_data;
}

/**
 * Compares two instances of PCB_DATA if they are equal
 * @param data1 The first instance to be compared
 * @param data2 The second instance to be compared
 * @return 1 if equal; 0 if not equal
 * */
int isDataEqual(struct PCB_DATA *data1, struct PCB_DATA *data2){
    if (data1->t_index == data2->t_index &&
        data1->b_index == data2->b_index &&
        data1->b_length == data2->b_length &&
        data1->wall_clock_time == data2->wall_clock_time &&
        data1->vruntime == data2->vruntime)
    {    
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * Free the memory consumed by a PCB_DATA instance
 * @param pcb_data Pointer to a PCB_DATA instance
 * */
void destroy_pcb(struct PCB_DATA *pcb_data){
    free(pcb_data);
}