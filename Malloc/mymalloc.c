#include <stdio.h>
#include "mymalloc.h"

#define ptrsize sizeof(void*)

struct metadata{
    int size;
    char inUse;
    struct metadata * userData;
};

int main(int argc, char*argv){
    char* array = myblock;
    int * ptr = (int*) malloc(4);

    return 0;
}
void* mymalloc(int size, int codeLine, char*fileName){

    if(myblock[0] == 0xFF && myblock[1] == 0x55){
        // Our static byte array has metadata in it

    }else{// There is data in our myblock
            myblock[0] = 0xFF;
            myblock[1] = 0x55; 
            
            struct metadata startBlock = {4096 - 2 - 1 - ptrsize, 'n', NULL};

            struct metadata* ptr = &myblock[2];
            

            if(size > startBlock.size){
                // Error
            }else{

            }     
    }
}

void myfree(void * ptr){
    if(myblock[0] != 0xFF && myblock[1] != 0x55){
        //fprint(__FILE__, "Cannot free because nothing has been allocated\n");
        return;
    }


}

/** Previous mymalloc code 
 * void * blockPtr;
    // Check that there is metadata in our myblock 
    if(myblock[0] == 0xFF && myblock[1] == 0x55){
        // Our static byte array has metadata in it

    }else{// There is data in our myblock
            myblock[0] = 0xFF;
            myblock[1] = 0x55; 
            int * ptr =(int*) &myblock[2];
            *ptr = 4096 - 2 - 1 - 4 - ptrsize;
            myblock[6] = 'n';
            blockPtr = &myblock[7];
        
        if(size <= ptr[0]){
            myblock[2] = size;
            myblock[6] = 'y';

            myblock[7 + ptrsize + size] = myblock[2] - size;
            myblock[7 + ptrsize + size + 1] = 'n';
            
        }else{
            printf("%s called this malloc method and %d is the line\n", __FILE__, __LINE__);
            return (void*)blockPtr;
        }
        
    }
 * **/