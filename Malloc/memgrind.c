#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <time.h>



void workloadA(){
    // Workload A: malloc() 1 byte and immediately free it - do this 150 times
    for(int i = 0; i < 150; i++){
        void* a = malloc(1);
        free(a);
    }
    
    return;
}

void workloadB(){
    // Workload B: malloc() 1 byte, store the pointer in an array - do this 150 times
    // Once you've malloc()ed 50 byte chunks, then free() the 50 1 byte pointers one by one.
    char*arr[50];

    for(int i = 0; i< 150; i++){
        arr[i%50] = (char*) malloc(1);
        if(i%50 == 49){
            // free() one by one
            for(int j = 0; j < 50; j++){
                free( (void*) arr[j] );
            }
        }
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

   int maxMemoryCapacity = (4096 - 2 - sizeof(md)); // Need to confirm?????????
   int num = 0;
   int randMemory = 0;
   int c = 0;
   int memoryTrack[50]; // array to help keep track of the size of memory we decide to allocate

   for(int i = 0; i<50; i++){
       memoryTrack[i] = 0;
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

    for(int i = 0; i < 150; i++){
        void* a = malloc((4096 - 2 - sizeof(md)));
        free(a);
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
    for(int j = 0; j < (4096 - 2 - sizeof(md)); j++){
        a[j] = (char*)malloc(1);
        if(a[j] == NULL){
            break;
        }
        free((void*) a[j]);
    }

    for(int i = 0; i < 150; i++){
        a[0] = (char*)malloc((4096 - 2 - sizeof(md)));
        free((void*) a[0]);
    }

    return;
}

int main(int argc, char** argv){

    double runTime[6];
    for(int i = 0; i< 6; i++){
        runTime[i] = 0;
    }

    clock_t start, end;
    for(int j = 0; j< 100; j++){

        start = clock();
        workloadA();
        end = clock();
        runTime[0] += ((double) (end - start))/CLOCKS_PER_SEC;

        start = clock();
        workloadB();
        end = clock();
        runTime[1] += ((double) (end - start))/CLOCKS_PER_SEC;

        start = clock();
        workloadC();
        end = clock();
        runTime[2] += ((double) (end - start))/CLOCKS_PER_SEC;

        start = clock();
        workloadD();
        end = clock();
        runTime[3] += ((double) (end - start))/CLOCKS_PER_SEC;

        start = clock();
        workloadE();
        end = clock();
        runTime[4] += ((double) (end - start))/CLOCKS_PER_SEC;

        start = clock();
        workloadF();
        end = clock();
        runTime[5] += ((double) (end - start))/CLOCKS_PER_SEC;

    }

    FILE * fp;
    fp = fopen("readme.txt","w");

    char workloads[6] = {'A','B','C','D', 'E', 'F'};
    for(int t = 0; t < 6; t++){
        runTime[t] /= 100;
        fprintf(fp, "The run time for workload %c is %f seconds\n", workloads[t], runTime[t]);
    }
    
    fclose(fp);
    return 0;
}