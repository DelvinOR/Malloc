#ifndef mymalloc_h
#define mymalloc_h
#define malloc( x ) mymalloc( x, __FILE__, __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )
typedef struct metadata{
	int size;
	char inUse;
	char key1;
	char key2;
	struct metadata* next;
}md;
void* mymalloc(int, char*, int);
void myfree(void*, char*, int);
static char myblock[4096];
#endif
