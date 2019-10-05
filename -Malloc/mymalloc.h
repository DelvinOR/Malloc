#ifndef mymalloc_h
#define mymalloc_h
#define malloc(x) mymalloc(x)
#define free(x) myfree(x)
void* mymalloc(int);
void myfree(void*);
static char myblock[4906];
#endif