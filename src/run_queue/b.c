#include <stdlib.h>
#include "./b.h"
#include "../defns.h"

/**
 * @param t_index Thread index
 * @param b_index Burst index
 * @param b_length Burst length
 * @param wall_clock_time The wall clock time the bust is generated
 * @return Pointer to the PCB data structure
 * */
struct B_DATA *create_b_data(int t_index, int b_index, int b_length, int wall_clock_time, int vruntime){
    struct B_DATA * b_data = (struct B_DATA*)malloc(sizeof(struct B_DATA));
    b_data->t_index         = t_index;
    b_data->b_index         = b_index;
    b_data->b_length        = b_length;
    b_data->wall_clock_time = wall_clock_time;
    b_data->vruntime        = vruntime;

    return b_data;
}

/**
 * Compares two instances of B_DATA if they are equal
 * @param data1 The first instance to be compared
 * @param data2 The second instance to be compared
 * @return 1 if equal; 0 if not equal
 * */
int isDataEqual(struct B_DATA *data1, struct B_DATA *data2){
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
 * Free the memory consumed by a B_DATA instance
 * @param b_data Pointer to a B_DATA instance
 * */
void destroy_b(struct B_DATA *b_data){
    free(b_data);
}