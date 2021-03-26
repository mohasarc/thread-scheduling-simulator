#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
// #include <sys/types.h>
// #include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
// #include <fcntl.h> 
#include "./run_queue/linked_list.h"
#include "./run_queue/pcb.h"
#include "defns.h"

#define FILE_EXTENTION ".txt"
#define READ_FROM_FILE_OPT "-f"

// GLOBAL VARIABLES
int N;      // The number of W threads.
int Bcount; // The number of bursts that each W thread will generate.
int minB;   // Minimum random value. (if less regenerate)
int avgB;   // The parameter of the exponentially distributed random 
            // value of the burst time.
int minA;   // Minimum random value. (if less regenerate)
int avgA;   // The parameter of the exponentially distributed random 
            // value of the interarrival time (sleep time of W threads).
char* ALG;  // The schedulingalgorithm to use: “FCFS”,  “SJF”, “PRIO”, “VRUNTIME”.

struct LL_NODE* RQ_HEAD = NULL; // The ready queue head
struct LL_NODE* RQ_TAIL = NULL; // The ready queue tail

pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t signal_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pcb_queued_signal = PTHREAD_COND_INITIALIZER;

/**
 * Generates an exponantially distributed random number
 * @param mean
 * */
double getExpRandomNum(int mean ){
    double rn;
    double lambda = 1 / (double)mean;

    // Get a random number in [0,1)
    rn = (double)rand() / (double)((unsigned)RAND_MAX + 1);

    // Convert to exponential destribution
    return log((1-rn))/(-lambda);
}

/**
 * The job a W thread does
 * Adds itself to the Ready Queue
 * @param t_index The index of the thread
 * */
void *doWJob(void* t_index){
    int interarrival_time;
    int b_length;
    struct timeval wall_clock_time;
    long wall_clock_time_usec;
    int this_t_index = *((int *) t_index);

    srand ( time(NULL) );
    
    for (int i = 1; i <= Bcount; i++)
    {
        // Generate exponantially distributed random numbers
        do {
            interarrival_time = getExpRandomNum(avgA);
        } while(interarrival_time < minA);

        do{
            b_length = getExpRandomNum(avgB);
        } while (b_length < minB);
        

        // Sleep
        printf("In thread %d, Interarivaltime: %d\n", this_t_index, interarrival_time);
        usleep(interarrival_time*1000);
        
        // Calculate time of day
        gettimeofday(&wall_clock_time, NULL);
        wall_clock_time_usec = (wall_clock_time.tv_sec * 1000) + wall_clock_time.tv_usec/1000;
        
        // Create PCB_DATA object
        struct PCB_DATA *pcb_data = create_pcb_data(*((int *) t_index), i, b_length, wall_clock_time_usec);

        // Schedule self and signal S thread (cretical section)
        pthread_mutex_lock(&a_mutex);
        {
            LL_shift(pcb_data, &RQ_HEAD, &RQ_TAIL);
            pthread_cond_signal(&pcb_queued_signal);
        }
        pthread_mutex_unlock(&a_mutex);
    }

    return NULL;
}

/**
 * The job an S thread does
 * Decides which thread to run and remove from Ready Queue
 * @param param 
 * */
void *doSJob(void* param){
    struct PCB_DATA *pcb_to_process; 
    int success = FALSE;

    // There will be max. N*Bcount number of bursts
    for (int i = 0; i < (N*Bcount); i++)
    {
        // Try to get an item from Ready Queue
        pthread_mutex_lock(&a_mutex);
        {
            success = LL_pop(&pcb_to_process, &RQ_HEAD, &RQ_TAIL);
            if (success)
            {
                printf("Running thread %d for %d'th bust with length %d \n", pcb_to_process->t_index, pcb_to_process->b_index, pcb_to_process->b_length);
                usleep(pcb_to_process->b_length*1000);
            }
        
            // Ready Queue is empty wait for another pcb to be inserted
            if (!success)
            {
                printf("Ready Queue is emplty, waiting for bursts \n");
                pthread_cond_wait(&pcb_queued_signal, &a_mutex);
            }
        }
        pthread_mutex_unlock(&a_mutex);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    // LOCAL VARIABLES  
    pthread_t *tids = malloc((N+1)*sizeof(pthread_t*));
    pthread_attr_t attr;
    int *arg;
    char* fileName;
    int readFromFile = FALSE;

    // Parsing arguments
    for (int i = 0; i < argc; i++){
        if (strcmp(argv[i], READ_FROM_FILE_OPT) == 0){
            readFromFile = TRUE;
            fileName = argv[i+1];
            break;
        }
    }

    // Parse the rest of arguments
    if (!readFromFile){
        if (argc < 8){
            fprintf(stderr, "Missing arguments!\n");
            exit(1);
        } else {
            N       = atoi(argv[1]);
            Bcount  = atoi(argv[2]);
            minB    = atoi(argv[3]);
            avgB    = atoi(argv[4]);
            minA    = atoi(argv[5]);
            avgA    = atoi(argv[6]);
            ALG     = argv[7];
        }
    } else {
        // Read data from file
    }

    // Create S thread
    pthread_attr_init(&attr);
    pthread_create(tids, &attr, doSJob, NULL);
    tids++;    
    
    // Create W threads
    for (int i = 1; i <= N; i++){
        arg = (int *)malloc(sizeof(int));
        *arg = i;
        pthread_attr_init(&attr);
        pthread_create(tids, &attr, doWJob, arg);
        tids++;
    }

    // Wait for the created threads
    for (int i = (N+1); i > 0; i--){
        pthread_join(*tids, NULL);
        tids--;
    }

    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&a_mutex);
    pthread_cond_destroy(&pcb_queued_signal);
    pthread_exit(NULL);
}