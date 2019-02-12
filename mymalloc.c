#include <stdio.h>
#include <string.h>
#include "mymalloc.h"

static char mainMemory[5000];

void initializeMainMemory(){
    block firstBlock;

    firstBlock.assigned = '0';
    firstBlock.prev = NULL;
    firstBlock.next = NULL;
    firstBlock.address = mainMemory + sizeof(block);
    firstBlock.size = sizeof(mainMemory) - sizeof(block);

    memcpy(mainMemory, &firstBlock, sizeof(block));
}

void printMemory() {
	block* ptr = (block*) mainMemory;
	while(ptr != NULL) {	
		if(ptr->assigned == '1') 
			printf("Block address: %p Not Free Block Size: %d\n", ptr->address, ptr->size);
		else printf("Block address: %p Free Block size: %d\n", ptr->address, ptr->size);
		ptr = ptr->next;
	}
	printf("\n");
}

void* mymalloc(size_t size, char* file, int line){
    block* ptr = (block*) mainMemory;

    //check if memory is initialized
    if(ptr != NULL && ptr->address != ptr + 1){
        initializeMainMemory();
    }

    //check if their is an available memory space
    block* firstFree = NULL;
    while(ptr != NULL){
        if(ptr->assigned == '0' && ptr->size >= size){
            firstFree = ptr;
            break;
        }
        ptr = ptr->next;
    }

    //No block big enough found
    if(ptr == NULL){
        printf("Error in %s on line %d: tried to allocate more memory than available. \n", file, line);
        return NULL;
    }

    //no room to split data
    if(firstFree->size == size || firstFree->size - size <= sizeof(block)){
        firstFree->assigned = '1';
        return firstFree->address;
    }

    //normal split
    block nextFree;
    nextFree.size = firstFree->size - size - sizeof(block);
    nextFree.assigned = '0';
    nextFree.prev = firstFree;
    nextFree.next = firstFree->next;
    nextFree.address = firstFree->address + size + sizeof(block);
    memcpy(firstFree->address + size, &nextFree, sizeof(block));
    firstFree->size = size;
    firstFree->assigned = '1';
    firstFree->next = firstFree->address + size;
    return firstFree->address;
}

void myfree(void* address, char* file, int line){
    block* ptr = (block*) mainMemory;
    if(ptr != NULL && ptr->address != ptr + 1){
        printf("No blocks allocated as of yet. Please allocate memory before freeing.\n");
    }
    while(ptr != NULL){
        if(ptr->address == address){
            if(ptr->assigned == '0'){
                printf("Error in %s on line %d: tried to free already free'd memory. \n", file, line);
                return;
            }

            //set allocated to 0
            ptr->assigned = '0';

            block* prev = ptr->prev;
            block* next = ptr->next;

            //try merging with next block
            if(next != NULL && next->assigned == '0'){
                ptr->size += next->size + sizeof(block);
                ptr->next = next->next;
                if(next->next != NULL){
                    next->next->prev = ptr;
                }
            } 
            //try merging with previous block
            if(prev != NULL && prev->assigned == '0'){
                prev->size += ptr->size + sizeof(block);
                prev->next = ptr->next;
                if(ptr->next != NULL){
                    ptr->next->prev = prev;
                }
            }
            return;
        }
        ptr = ptr->next;
    }
    printf("Error in %s on line %d: tried to free memory that was not allocated by malloc. \n", file, line);
}
