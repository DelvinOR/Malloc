#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <time.h>



void workloadA(){
    // Workload A: malloc() 1 byte and immediately free it - do this 150 times
int i;
    for(i = 0; i < 150; i++){
        void* a = malloc(1);
        free(a);
    }
    
    return;
}

void workloadB(){
    // Workload B: malloc() 1 byte, store the pointer in an array - do this 150 times
    // Once you've malloc()ed 50 byte chunks, then free() the 50 1 byte pointers one by one.
    char*arr[50];
	int i;
    for(i = 0; i< 150; i++){
        arr[i%50] = (char*) malloc(1);
        if(i%50 == 49){
            // free() one by one
		int j;
            for(j = 0; j < 50; j++){
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
	int i;
   for(i = 0; i<50; i++){
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
	int i;
    for(i = 0; i < 150; i++){
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
	int j = 0;
	//printf("DEBUG: HERE\n");
	char* ptr;
    while ((ptr = (char*) malloc(1)) != NULL){//fill array with 1 byte chars
		a[j] = ptr;
		*(a[j]) = (char)(('a' + j)%26);
		//printf("DEBUG: %d\n", j);
		j++;
	}
	int i = 0;
	while (i < j){
		if (*(a[i]) == (char)(('a' + i)%26)){
			//printf("DEBUG: good\n");
		}
		else {
			printf("Data corruption in workload F!\n Expected: %c Got: %c\n", (char)('a' + i)%256, *a[i]);
		}
		i++;
	}
	i--;
	//printf("RUNBACK\n");
	while (i >= 0){//run it back
		if (*(a[i]) == (char)(('a' + i)%26)){
			//printf("DEBUG: Freeing a[%d]\n", i);
			free(a[i]);
		}
		else {
			printf("Data corruption in workload F!\n");
		}
		i--;
	}
	
    return;
}

int main(int argc, char** argv){

    double runTime[6];
	int i;
    for(i = 0; i< 6; i++){
        runTime[i] = 0;
    }

    struct timespec start, end;
	int j;
    for(j = 0; j< 100; j++){

        clock_gettime(CLOCK_REALTIME, &start);
        workloadA();
        clock_gettime(CLOCK_REALTIME, &end);
        runTime[0] += ((double) (end.tv_nsec - start.tv_nsec))/1000000000;

        clock_gettime(CLOCK_REALTIME, &start);
        workloadB();
        clock_gettime(CLOCK_REALTIME, &end);
        runTime[1] += ((double) (end.tv_nsec - start.tv_nsec))/1000000000;

        clock_gettime(CLOCK_REALTIME, &start);
        workloadC();
        clock_gettime(CLOCK_REALTIME, &end);
        runTime[2] += ((double) (end.tv_nsec - start.tv_nsec))/1000000000;

        clock_gettime(CLOCK_REALTIME, &start);
        workloadD();
        clock_gettime(CLOCK_REALTIME, &end);
        runTime[3] += ((double) (end.tv_nsec - start.tv_nsec))/1000000000;

        clock_gettime(CLOCK_REALTIME, &start);
        workloadE();
        clock_gettime(CLOCK_REALTIME, &end);
runTime[4] += ((double) (end.tv_nsec - start.tv_nsec))/1000000000;

        clock_gettime(CLOCK_REALTIME, &start);
        workloadF();
        clock_gettime(CLOCK_REALTIME, &end);
		runTime[5] += ((double) (end.tv_nsec - start.tv_nsec))/1000000000;

    }

   // FILE * fp;
    //fp = fopen("readme.txt","w");

    char workloads[6] = {'A','B','C','D', 'E', 'F'};
	int t;
    for(t = 0; t < 6; t++){
        runTime[t] /= 100;
        printf("The run time for workload %c is %.8f seconds\n", workloads[t], runTime[t]);
    }
    
    //fclose(fp);
    return 0;
}
