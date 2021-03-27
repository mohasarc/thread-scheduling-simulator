/*---------------------------------------------
            B - Burst Data Struct
---------------------------------------------*/
#ifndef B_H
#define B_H
/**
 * B stands for Burst
 * Stores all information about a particular burst
 */
struct B_DATA
{
    int t_index; // Thread index
    int b_index; // Burst index
    int b_length; // Burst length
    int wall_clock_time; // The wall clock time the bust is generated
    int vruntime; // Thread's Virtual Runtime
};

// FUNCTIONS' PROTOTYPES
struct B_DATA *create_b_data(int t_index, int b_index, int b_length, int wall_clock_time, int vruntime);
int isDataEqual(struct B_DATA *data1, struct B_DATA *data2);
void destroy_b(struct B_DATA *b_data);
#endif