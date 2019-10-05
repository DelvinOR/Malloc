#ifndef mymalloc_h
#define mymalloc_h
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)
void* mymalloc();
void myfree();
static char myblock[4906];
#endif