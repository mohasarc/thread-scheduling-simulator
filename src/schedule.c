#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include "./run_queue/linked_list.h"
#include "./run_queue/b.h"
#include "./scheduling_algos/scheduling_algos.h"
#include "defns.h"

#define FILE_EXTENTION ".txt"
#define READ_FROM_FILE_OPT "-f"

// GLOBAL VARIABLES
int N;                      // The number of W threads.
int Bcount;                 // The number of bursts that each W thread will generate.
int minB;                   // Minimum random value. (if less regenerate)
int avgB;                   // The parameter of the exponentially distributed random 
                            // value of the burst time.
int minA;                   // Minimum random value. (if less regenerate)
int avgA;                   // The parameter of the exponentially distributed random 
                            // value of the interarrival time (sleep time of W threads).
char *ALG;                  // The schedulingalgorithm to use: “FCFS”,  “SJF”, “PRIO”, “VRUNTIME”.
char *inprefix;             // The file to read from
int *thread_exec_data;      // array listing the execution status of each thread
int readFromFile = FALSE;   // Whether to wear from file or not

struct LL_NODE *RQ_HEAD = NULL; // The run queue head
struct LL_NODE *RQ_TAIL = NULL; // The run queue tail

pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t b_queued_signal = PTHREAD_COND_INITIALIZER;
pthread_mutex_t b_count_mutex = PTHREAD_MUTEX_INITIALIZER;

// FUNCTIONS' PROTOTYPES
double getExpRandomNum(int mean );
int readDataFromFile(FILE* in_file, int *interarrival_time, int *b_length);
int threadsFinishedExecution(int *thread_exec_data, int thread_count);
void *doWJob(void *t_index);
void *doSJob(void *param);

// FUNCTIONS' DEFINITIONS
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
    // return  lambda * exp(-lambda * rn);
    return log(1-rn)/(-lambda);
}

/**
 * Reads one line from the file
 * @param in_file The file descriptor to read from
 * @param interarrival_time First value read from the line
 * @param b_length Second value read from the line
 * @return A boolean indicating the read operation success
 */
int readDataFromFile(FILE *in_file, int *interarrival_time, int *b_length){
    if ( fscanf(in_file, "%d", interarrival_time) == 1 && fscanf(in_file, "%d", b_length) == 1){
        return TRUE;
    }

    return FALSE;
}

/**
 * Checks if all threads have finished execution
 * @param thread_exec_data Array listing the execution status of each thread
 * @param thread_count Total number of threads
 * @return A boolean indicating whether all finished execution
 */
int threadsFinishedExecution(int *thread_exec_data, int thread_count){
    int allDone = TRUE;
    for (int i = 0; i < thread_count; i++){
        allDone = allDone && thread_exec_data[i];
    }
    
    return allDone;
}

/**
 * The job a W thread does
 * Adds itself to the run Queue
 * @param t_index The index of the thread
 * */
void *doWJob(void *t_index){
    // Local variables
    struct timeval wall_clock_time;
    struct B_DATA *b_data;
    long wall_clock_time_ms = 0;
    int interarrival_time = 0;
    int generatedBursts = 0;
    int b_length = 0;
    int vruntime = 0;
    char fileName[255];
    FILE* in_file;

    srand ( time(NULL) * (*((int *) t_index)) );

    if (readFromFile){
        snprintf(fileName, 255, "%s-%d%s", inprefix,  *((int *) t_index), FILE_EXTENTION);
        in_file = fopen(fileName, "r"); // read only
        if (! in_file ){
            printf("oops, file can't be read\n");
            exit(-1); 
        }
    }

    while (generatedBursts <= Bcount || readFromFile)
    {
        // Generate or read exponantially distributed random numbers
        if (!readFromFile){
            do {
                interarrival_time = getExpRandomNum(avgA);
            } while(interarrival_time < minA);

            do{
                b_length = getExpRandomNum(avgB);
            } while (b_length < minB);
        } else {
            if (!readDataFromFile(in_file, &interarrival_time, &b_length)){
                break;
            }
        }

        // Sleep
        usleep(interarrival_time*1000);
        
        // Calculate time of day
        gettimeofday(&wall_clock_time, NULL);
        wall_clock_time_ms = (wall_clock_time.tv_sec * 1000) + wall_clock_time.tv_usec/1000;
        
        // Create B_DATA object
        b_data = create_b_data(*((int *) t_index), generatedBursts, b_length, wall_clock_time_ms, vruntime);
        vruntime += b_length*(0.7+0.3*((double)(*((int *) t_index))));
        generatedBursts += 1;

        // Schedule self and signal S thread (cretical section)
        pthread_mutex_lock(&a_mutex);
        {
            LL_shift(b_data, &RQ_HEAD, &RQ_TAIL);
            pthread_cond_signal(&b_queued_signal);
        }
        pthread_mutex_unlock(&a_mutex);
    }

    thread_exec_data[*((int *) t_index) - 1] = TRUE; // Indicate the its execution ended
    free(t_index);
    return NULL;
}

/**
 * The job an S thread does
 * Decides which thread to run and remove from run Queue
 * @param param 
 * */
void *doSJob(void *param){
    struct B_DATA *b_to_process; 
    int success = FALSE;

    // Check if there is any burst in run queue or there will be more bursts
    while (!threadsFinishedExecution(thread_exec_data, N) || RQ_HEAD)
    {
        // Try to get an item from run Queue (critical section)
        pthread_mutex_lock(&a_mutex);
        {
            if (strcmp(ALG, "FCFS") == 0){
                success = get_using_FCFS(&b_to_process, &RQ_HEAD, &RQ_TAIL);
            } else if (strcmp(ALG, "SJF") == 0) {
                success = get_using_SJF(&b_to_process, &RQ_HEAD, &RQ_TAIL);
            } else if (strcmp(ALG, "PRIO") == 0){
                success = get_using_PRIO(&b_to_process, &RQ_HEAD, &RQ_TAIL);
            } else if (strcmp(ALG, "VRUNTIME") == 0){
                success = get_using_VRUNTIME(&b_to_process, &RQ_HEAD, &RQ_TAIL);
            }
        }
        pthread_mutex_unlock(&a_mutex);

        // Could retrieve a burst from the run queue
        // Simulate running it by sleeping for the same amount of time
        if (success)
        {
            usleep(b_to_process->b_length * 1000);
            free(b_to_process);
        }

        // run Queue is empty wait for another b to be inserted
        if (!success)
        {
            pthread_mutex_lock(&a_mutex);
            {
                pthread_cond_wait(&b_queued_signal, &a_mutex);
            }
            pthread_mutex_unlock(&a_mutex);
        }
    }

    return NULL;
}

/**
 * Execution:
 * schedule <N> <Bcount> <minB> <avgB> <minA> <avgA> <ALG>
 * schedule <N> <ALG> -f <inprefix>
 * i.e. <inprefix>-i.txt
 */
int main(int argc, char *argv[])
{
    // LOCAL VARIABLES  
    pthread_t *tids = malloc((N+1)*sizeof(pthread_t));
    pthread_attr_t attr;
    int *arg;

    // Parse the arguments
    if (strcmp(argv[3], READ_FROM_FILE_OPT) != 0){
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
        N            = atoi(argv[1]);
        ALG          = argv[2];
        inprefix     = argv[4];
        readFromFile = TRUE;
    }

    thread_exec_data = malloc(N * sizeof(int));
    for (int i = 0; i < N; i++){
        thread_exec_data[i] = FALSE;
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
        tids--;
        pthread_join(*tids, NULL);
    }


    free(tids);
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&a_mutex);
    pthread_cond_destroy(&b_queued_signal);
    pthread_exit(NULL);
}