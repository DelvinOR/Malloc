#ifndef mymalloc_h
#define mymalloc_h
#define malloc(x) mymalloc(x,__LINE__, __FILE__);
#define free(x) myfree(x,__LINE__, __FILE__)
void* mymalloc(int,int, char*);
void myfree(void*,int,char*);
static char myblock[4096];
#endif