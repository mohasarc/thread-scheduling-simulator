#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <sys/time.h>
#include <time.h>
// #include <fcntl.h> 

#define FILE_EXTENTION ".txt"
#define READ_FROM_FILE_OPT "-f"
#define TRUE 1
#define FALSE 0

double getExpRandomNum(int mean ){
    double rn;
    double lambda = 1 / (double)mean;

    // Get a random number in [0,1)
    rn = (double)rand() / (double)((unsigned)RAND_MAX + 1);

    // Convert to exponential destribution
    return log((1-rn))/(-lambda);
}

int main(int argc, char *argv[])
{  
    // LOCAL VARIABLES
    int N;      // The number of W threads.
    int Bcount; // The number of bursts that each W thread will generate.
    int minB;   // Minimum random value. (if less regenerate)
    int avgB;   // The parameter of the exponentially distributed random 
                // value of the burst time.
    int minA;   // Minimum random value. (if less regenerate)
    int avgA;   // The parameter of the exponentially distributed random 
                // value of the interarrival time (sleep time of W threads).
    char* ALG;  // The schedulingalgorithm to use: “FCFS”,  “SJF”, “PRIO”, “VRUNTIME”.
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

    printf("Reading from file: %d, fileName: %s \n", readFromFile, fileName);
    printf("Reading params: N %d, Bcount %d, minB %d, avgB %d, minA %d, avgA %d, ALG %s \n", N, Bcount, minB, avgB, minA, avgA, ALG);

    srand ( time(NULL) );
    printf("exp rand num %f \n", getExpRandomNum(30));
}