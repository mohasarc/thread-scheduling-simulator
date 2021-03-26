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
        // Generate random numbers
        do {
            interarrival_time = getExpRandomNum(avgA);
        } while(interarrival_time < minA);

        do
        {
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

        // Schedule selt (cretical section)
        pthread_mutex_lock(&a_mutex);
        LL_shift(pcb_data, &RQ_HEAD, &RQ_TAIL);
        pthread_mutex_unlock(&a_mutex);
    }

    return NULL;
}

/**
 * The job an S thread does
 * @param param 
 * */
void *doSJob(void* param){
    return NULL;
}

int main(int argc, char *argv[])
{
    // LOCAL VARIABLES  
    int readFromFile = FALSE;
    char* fileName;

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

    // int result = usleep(1000003);
    // int result = nanosleep((struct timespec[]){{0, 500000000L}},NULL);
    // printf("usleep returned: %d\n",result);

    // Create the threads
    pthread_t tid;
    pthread_attr_t attr;
    pthread_t *tids = malloc((N+1)*sizeof(pthread_t*));
    int *arg;
    
    for (int i = 1; i <= N; i++){
        arg = (int *)malloc(sizeof(int));
        *arg = i;
        pthread_attr_init(&attr);
        pthread_create(tids, &attr, doWJob, arg);
        tids++;
    }

    // Wait for the created threads
    for (int i = N; i > 0; i--){
        pthread_join(*tids, NULL);
        tids--;
    }

    LL_print(RQ_HEAD, RQ_TAIL);
}