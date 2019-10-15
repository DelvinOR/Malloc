#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"


//typedef struct metadata{
//	int size;
//	char inUse;
//	char key1;
//	char key2;
//	struct metadata* next;
//}md;




int main(int argc, char *argv[]){//temp main driver
	char conarray[] = "Hello this is a test designed to see if my program works as i intended it to. So far this is working as intended, if there are any irregularities, then perhaps something has gone wrong! 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100!";
	char* array = malloc(26);
	int* array1 = malloc(40 * sizeof(int));
	char** array3 = malloc(420 * sizeof(char**));
	char* array2 = malloc(sizeof(conarray));
	//char** array3 = malloc(400 * sizeof(char**));
	
	if (array2){
		int i = 0;
		while (i < sizeof(conarray)){
			array2[i] = conarray[i];
			i++;
		}
	}
	int i = 0;
	int j = 97;
	if (array){//not null
		if (array1){//not null
			int integer = 0;
			while (integer < 40){
				array1[integer] = 10000 * integer;
				integer++;
			}
		}
		while (i < 26){
			array[i] = j++;
			i++; 
		}
		i = 0;
		while (i < 26){
			printf("array[%d]: %c\n", i, array[i]);
			i++;
		}
		i = 0;
		int d = 1;//debug = 1
		while (i < 50 && d == 1){
			int* intptr = &myblock[i];
			char** addptr = &myblock[i];
			printf("MYBLOCK: ADD: %X VAL: HEX: %X DEC: %d CHAR: %c\n", &myblock[i], *intptr, *addptr, myblock[i]);
			i++;
		}
		int m = 1;
		md* metaptr = &myblock[2];
		while (metaptr){//not NULL
			printf("MD block %d:\n", m);
			printf("Size: %d\n", metaptr->size);
			printf("Next block ADD: %x\n", metaptr->next);
			printf("In use: %c\n", metaptr->inUse);
			metaptr = metaptr->next;
			m++;
		}
	}
	else {//is null
		printf("ERROR: Malloc gave null pointer back!\n");
	}
	//array1 = malloc(40 * sizeof(int));
	printf("Size: %d\n", sizeof(md));
	printf("Message:\n %s \n", array2);
	printf("DEBUG: freeing a0\n");
	free(array);
	if (((md*)(array)-1)->inUse == 'n'){
		printf("DEBUG: array0 freed!\n");
	}
	else{
		printf("DEBUG: ERROR: array0 NOT freed!\n");
	}
	
	printf("DEBUG: freeing a1\n");
	free(array1);
	if (((md*)(array1)-1)->inUse == 'n'){
		printf("DEBUG: array1 freed!\n");
	}
	else{
		printf("DEBUG: ERROR: array1 NOT freed!\n");
	}
	printf("DEBUG: freeing a2\n");
	free(array2);
	if (((md*)(array2)-1)->inUse == 'n'){
		printf("DEBUG: array2 freed!\n");
	}
	else{
		printf("DEBUG: ERROR: array2 NOT freed!\n");
	}
	printf("DEBUG: freeing a3\n");
	free(array3);
	if (((md*)(array3)-1)->inUse == 'n'){
		printf("DEBUG: array3 freed!\n");
	}
	else{
		printf("DEBUG: ERROR: array3 NOT freed!\n");
	}
	return 0;
}

void* mymalloc (int size, char* file, int line){
	char* byteptr = myblock;
	int* intptr = myblock;
	md* metaptr = NULL;
	if (size <= 0){
		printf("Error in %s on line %d:\n", file, line);
		printf("\tEnter an amount > 0!\n");
		return NULL;
	}
	if (myblock[0] == (char)0xFF && myblock[1] == (char)0x55){//initialization bytes check passes, metadata exists
				md* block = &myblock[2];
                while (block){//not null
					if (block->inUse == 'n'){//found an open block
						if (block->size >= size){//usable
							if (block->size > size + sizeof(md)){//can split
								int remaining = block->size - size - sizeof(md);
								block->size = size;
								block->inUse = 'y';
								md* newblock = (char*)(block+1) + size;
								md* oldnext = block->next;
								block->next = newblock;
								newblock->size = remaining;
								newblock->inUse = 'n';
								newblock->key1 = (char)0xFF;
								newblock->key2 = (char)0x55;
								newblock->next = oldnext;
								return ++block;
							}
							else {//cant split
								block->inUse = 'y';
								printf("DEBUG: can't split\n");
								return ++block;
							}
						}
						//else{//not usable, too small
					}
					block = block->next;
				}
				printf("Error in %s on line %d:\n", file, line);
				printf("\tNot enough memory!\n");
                return NULL;
    }
	else{//uninitialized
		printf("DEBUG: Mymalloc uninitialized\n");
		metaptr = &myblock[2];
		int remaining = (4096 - 2 - sizeof(md));
		if (remaining - size > sizeof(md)){//can create another block
			metaptr->size = size;
			metaptr->inUse = 'y';
			metaptr->key1 = (char)0xFF;
			metaptr->key2 = (char)0x55;
			md* newmeta = (char*)(metaptr + 1) + size;
			newmeta->size = remaining - sizeof(md) - size;
			newmeta->next = NULL;
			newmeta->inUse = 'n';
			newmeta->key1 = (char)0xFF;
			newmeta->key2 = (char)0x55;
			metaptr->next = newmeta;
			myblock[0] = 0xFF;//set check bytes
			myblock[1] = 0x55;
			return ++metaptr;//address requested
		}
		else if (remaining - size > 0){//no more blocks, but can still give this one away.
			metaptr->size = remaining;
			metaptr->next = NULL;
			metaptr->inUse = 'y';
			metaptr->key1 = (char)0xFF;
			metaptr->key2 = (char)0x55;
			myblock[0] = 0xFF;//set check bytes
			myblock[1] = 0x55;
			return ++metaptr;//address requested
		}
		else {//not enough space in array
			printf("DEBUG: requested size is larger than array on init\n");
			metaptr->size = remaining;
			metaptr->next = NULL;
			metaptr->inUse = 'n';
			metaptr->key1 = (char)0xFF;
			metaptr->key2 = (char)0x55;
			myblock[0] = 0xFF;//set check bytes
			myblock[1] = 0x55;
			printf("Error in %s on line %d:\n", file, line);
			printf("\tNot enough memory!\n");
			return NULL;
		}
	}
}


void myfree(void* ptr, char* file, int line){
	//printf("DEBUG: myfree: ptr: %x\n", ptr);
	md* mdptr = (md*)(ptr) - 1;
	//printf("DEBUG: myfree: mdptr: %x\n", mdptr);
	if (ptr == NULL){//NULL pointer
		printf("Error in %s on line %d:\n", file, line);
		printf("\tNULL pointer!\n");
		return;
	}
	if (ptr > &myblock[4095] || ptr < myblock){//not a pointer
		printf("Error in %s on line %d:\n", file, line);
		printf("\tNot a pointer!\n");
		return;
	}
	if (mdptr->key1 == (char)0xFF && mdptr->key2 == (char)0x55){//is proper pointer
		//printf("DEBUG: myfree: ptr is a block\n");
		if (mdptr->inUse == 'n'){//already freed
			printf("Error in %s on line %d:\n", file, line);
			printf("\tBlock already freed!\n");
			return;
		}
		mdptr->inUse = 'n';
		md* block = &myblock[2];
		if (block == mdptr){//we are first block
			//printf("DEBUG: we are first block!\n");
			int reclaim = 0;
			block = block->next;
			while (block != NULL && block->inUse == 'n'){//perform cleanup
				//printf("eeeeeeeeeeeeeeeeeeeee\n");
				reclaim += block->size + sizeof(md);
				block->key1 = (char)(0xCC);//mark block as cleaned
				block->key2 = (char)(0xCC);
				block = block->next;
			}
			//printf("DDDDDDDDDDDDD\n");
			if (block == NULL){//ran out of blocks
				mdptr->next = NULL;
				mdptr->size = mdptr->size + reclaim;
				//printf("DEBUG: block freed!\n");
				return;
			}
			else {//ran into a block in use
				mdptr->next = block;
				mdptr->size = mdptr->size + reclaim;
				return;
			}
		}//not first block
		while (block){//not null
			//printf("DEBUG: LOOP\n");
			if (block->next == mdptr){//found our block
				if (block->inUse == 'n'){//parent block needs cleanup
					if(block == &myblock[2]){//cleanup from first block
						int reclaim = mdptr->size + sizeof(md);
						mdptr->key1 = (char)(0xCC);
						mdptr->key2 = (char)(0xCC);
						mdptr = mdptr->next;
						while (mdptr != NULL && mdptr->inUse == 'n'){//perform cleanup
							reclaim += mdptr->size + sizeof(md);
							mdptr->key1 = (char)(0xCC);//mark block as cleaned
							mdptr->key2 = (char)(0xCC);
							mdptr = mdptr->next;
						}
						if (mdptr == NULL){//ran out of blocks
							block->next = NULL;
							block->size = block->size + reclaim;
							return;
						}
						else {//ran into a block in use
							block->next = mdptr;
							block->size = block->size + reclaim;
							return;
						}
					}//else block not first block
					int reclaim = block->size + sizeof(md);
					block->key1 = (char)(0xCC);
					block->key2 = (char)(0xCC);
					reclaim += mdptr->size + sizeof(md);
					mdptr->key1 = (char)(0xCC);
					mdptr->key2 = (char)(0xCC);
					mdptr = mdptr->next;
					while (mdptr != NULL && mdptr->inUse == 'n'){//perform cleanup
						reclaim += mdptr->size + sizeof(md);
						mdptr->key1 = (char)(0xCC);//mark block as cleaned
						mdptr->key2 = (char)(0xCC);
						mdptr = mdptr->next;
					}
					--block;
					if (mdptr == NULL){//ran out of blocks
						block->next = NULL;
						block->size = block->size + reclaim;
						return;
					}
					else {//ran into a block in use
						block->next = mdptr;
						block->size = block->size + reclaim;
						return;
					}
				}//end of parent block not in use
				else {//block still in use
					int reclaim = mdptr->size + sizeof(md);
					mdptr->key1 = (char)(0xCC);
					mdptr->key2 = (char)(0xCC);
					mdptr = mdptr->next;
					while (mdptr != NULL && mdptr->inUse == 'n'){//perform cleanup
						reclaim += mdptr->size + sizeof(md);
						mdptr->key1 = (char)(0xCC);//mark block as cleaned
						mdptr->key2 = (char)(0xCC);
						mdptr = mdptr->next;
					}
					if (mdptr == NULL){//ran out of blocks
						block->next = NULL;
						block->size = block->size + reclaim;
						return;
					}
					else {//ran into a block in use
						block->next = mdptr;
						block->size = block->size + reclaim;
						return;
					}
				}//end of parent block still in use
			}//did not find block
			block = block->next;
		}
		printf("Error in %s on line %d:\n", file, line);
		printf("\tSOMETHING BAD, COULDNT FIND BLOCK!\n");
		return;
	}
	printf("Error in %s on line %d:\n", file, line);
	printf("\tNot a pointer allocated by mymalloc!\n");


	return;
}








