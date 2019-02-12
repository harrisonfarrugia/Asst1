#ifndef MYMALLOC_H
#define MYMALLOC_H

#define malloc( x ) mymalloc( x, __FILE__, __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )

typedef struct _block{
    int size;
    char assigned;
    struct _block* next;
    struct _block* prev;
    void* address;
} block;

void initializeMainMemory();

void* mymalloc(size_t, char*, int);

void myfree(void*, char*, int);

void printMemory();

#endif
