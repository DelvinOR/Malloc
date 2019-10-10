#ifndef mymalloc_h
#define mymalloc_h
#define malloc(x) mymalloc(x,__LINE__, __FILE__);
#define free(x) myfree(x)
void* mymalloc(int,int, char*);
void myfree(void*);
static char myblock[4906];
#endif