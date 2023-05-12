#include <sys/mman.h>

#define NULL 0

struct header_t {
    size_t size;
};


void *malloc(size_t size){
    char* data = mmap(NULL, size+sizeof(struct header_t), PROT_READ|PROT_WRITE, MAP_PRIVATE| MAP_ANONYMOUS, -1,0);
    if(data == MAP_FAILED) return NULL;
    ((struct header_t*)data)->size = size;
    return data + sizeof(struct header_t);
}

void free(void *block) {
    if(!block) return;
    size_t size = ((struct header_t*)((char*)block-sizeof(struct header_t)))->size + sizeof(struct header_t);
    munmap((char*)block-sizeof(struct header_t), size);
}

void *calloc(size_t num, size_t nsize) {
    return malloc(num*nsize);
}

void *realloc(void *block, size_t size) {
    if(!block||!size) return malloc(size);
    struct header_t* head = (struct header_t*)block - 1;
    if(head->size>=size) return block;
    char* ret = malloc(size);
    if(ret) {
        for(int i=0;i<head->size;i++)
            ret[i] = ((char*)block)[i];
        free(block);
    }
    return ret;
}
