#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <time.h>



void workloadA(){
    // Workload A: malloc() 1 byte and immediately free it - do this 150 times
    int i = 0;
    while(i < 150){
        void* a = malloc(1);
        free(a);
        i++;
    }
    
    return;
}

void workloadB(){
    // Workload B: malloc() 1 byte, store the pointer in an array - do this 150 times
    // Once you've malloc()ed 50 byte chunks, then free() the 50 1 byte pointers one by one.
    char*arr[50];

    int i = 0;
    while(i< 150){
        arr[i%50] = (char*) malloc(1);
        if(i%50 == 49){
            // free() one by one
            int j = 0;
            while(j < 50){
                free( (void*) arr[j] );
                j++;
            }
        }
        i++;
    }

    return;
}

void workloadC(){
    /*
        Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer
        > do this until you have allocated 50 times
        - Keep track of each operation so that you eventually malloc() 50 bytes, in total
        > if you have already allocated 50 times, disregard the random and just free() on each
        iteration
        - Keep track of each operation so that you eventually free() all pointers
        > don't allow a free() if you have no pointers to free()
    */
   char*arr[50];

   int c = 0;
   int num = 0;
   while(c < 50){
       // if the result of a rand() is even then malloc(1)
       // else check if we can free() 1 byte if not then dont do anything
       num = rand();
       if( num % 2 == 0){// even
            arr[c] = (char*)malloc(1);
            c++;
       }else{// odd
            if(c > 0){
                c--;
                free((void*) arr[c]);
            }
       }
   }
   // Loop ended and so we allocated 50 times and now we just free 50 times
   while(c > 0){
       c--;
       free((void*) arr[c]);
   }

   return;
}

void workloadD(){
    /*
    Randomly choose between a randomly-sized malloc() or free()ing a pointer – do this many
    times (see below)
    - Keep track of each malloc so that all mallocs do not exceed your total memory capacity
    - Keep track of each operation so that you eventually malloc() 50 times
    - Keep track of each operation so that you eventually free() all pointers
    - Choose a random allocation size between 1 and 64 bytes:
        - num = (rand() % 64) + 1
    */
   char*arr[50];

   int maxMemoryCapacity = (4096 - 2 - sizeof(md)); 
   int num = 0;
   int randMemory = 0;
   int c = 0;
   int memoryTrack[50]; // array to help keep track of the size of memory we decide to allocate

   int i = 0;
   while(i<50){
       memoryTrack[i] = 0;
       i++;
   }

   while(c < 50){
       num = rand();
       if(num % 2 == 0){//malloc
            randMemory = (rand() % 64) + 1;
            if(randMemory <= maxMemoryCapacity){
                arr[c] = (char*) malloc(randMemory);
                maxMemoryCapacity -= randMemory;
                memoryTrack[c] = randMemory;
                c++;
            }
       }else{//free
            if(c > 0){
                c--;
                maxMemoryCapacity += memoryTrack[c];
                memoryTrack[c] = 0;
                free((void*) arr[c]);
            }
       }
   }

   while(c > 0){
       c--;
       free((void*) arr[c]);
   }

    return;
}

void workloadE(){
    // malloc() entire memory capacity and free() it as well 150 times.
    // It would be interesting to test this workload against workload A.

    int i = 0;
    while(i < 150){
        void* a = malloc((4096 - 2 - sizeof(md)));
        free(a);
        i++;
    }
    
    return;
}

void workloadF(){
    // malloc() 1 until we run out of storage space or if malloc return null 
    // but immeadiately free after successfully malloc()ing, we free().
    // We should have all available space back after so just malloc max storage and then free it immediately 150 times.
    // A modification of workload E but in F we make sure that free() successfully emptied out our array so that we can
    // malloc again.
    
    char* a[4096];
    int j = 0;
    while(j < (4096 - 2 - sizeof(md))){
        a[j] = (char*)malloc(1);
        if(a[j] == NULL){
            break;
        }
        free((void*) a[j]);
        j++;
    }

    int i = 0;
    while(i < 150){
        a[0] = (char*)malloc((4096 - 2 - sizeof(md)));
        free((void*) a[0]);
        i++;
    }

    return;
}

int main(int argc, char** argv){

    double runTime[6];
    int i = 0;
    while(i< 6){
        runTime[i] = 0;
        i++;
    }

    struct timespec start, end;
    int j = 0;
    while(j< 100){

        clock_gettime(CLOCK_REALTIME, &start);
        workloadA();
        clock_gettime(CLOCK_REALTIME, &end);
        runTime[0] += ((double) (end.tv_nsec - start.tv_nsec))/1000000.0;

        clock_gettime(CLOCK_REALTIME, &start);
        workloadB();
        clock_gettime(CLOCK_REALTIME, &end);
        runTime[1] += ((double) (end.tv_nsec - start.tv_nsec))/1000000.0;

        clock_gettime(CLOCK_REALTIME, &start);
        workloadC();
        clock_gettime(CLOCK_REALTIME, &end);
        runTime[2] += ((double) (end.tv_nsec - start.tv_nsec))/1000000.0;

        clock_gettime(CLOCK_REALTIME, &start);
        workloadD();
        clock_gettime(CLOCK_REALTIME, &end);
        runTime[3] += ((double) (end.tv_nsec - start.tv_nsec))/1000000.0;

        clock_gettime(CLOCK_REALTIME, &start);
        workloadE();
        clock_gettime(CLOCK_REALTIME, &end);
        runTime[4] += ((double) (end.tv_nsec - start.tv_nsec))/1000000.0;

        clock_gettime(CLOCK_REALTIME, &start);
        workloadF();
        clock_gettime(CLOCK_REALTIME, &end);
        runTime[5] += ((double) (end.tv_nsec - start.tv_nsec))/1000000.0;

        j++;
    }

    FILE * fp;
    fp = fopen("readme.txt","w+");

    char workloads[6] = {'A','B','C','D', 'E', 'F'};
    int t = 0;
    while(t < 6){
        runTime[t] /= 100;
        fprintf(fp, "The run time for workload %c is %f microseconds\n", workloads[t], runTime[t]);
        t++;
    }
    
    fclose(fp);
    return 0;
}