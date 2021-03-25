/*------------------------------------------
        PCB - Process Control Block
------------------------------------------*/
#ifndef PCB_H
#define PCB_H
struct PCB_DATA 
{
    int t_index; // Thread index
    int b_index; // Burst index
    int b_length; // Burst length
    int wall_clock_time; // The wall clock time the bust is generated
};

// FUNCTIONS' PROTOTYPES
struct PCB_DATA *create_pcb_data(int t_index, int b_index, int b_length, int wall_clock_time);
int isDataEqual(struct PCB_DATA *data1, struct PCB_DATA *data2);
void destroy_pcb(struct PCB_DATA *pcb_data);
#endif