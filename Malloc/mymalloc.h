#ifndef mymalloc_h
#define mymalloc_h
#define malloc( x ) mymalloc( x, __FILE__, __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )
#pragma pack(1)
typedef struct metadata{
	struct metadata* next;
	char inUse;
	char key1;
	short size;
	
}md;
void* mymalloc(int, char*, int);
void myfree(void*, char*, int);
static char myblock[4096];
#endif
