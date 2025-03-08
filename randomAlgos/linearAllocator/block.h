#ifndef BLOCK_H_
#define BLOCK_H_

#include "linkedList.h"
#include <stdint.h>

struct _alloc_block
{
    size_t _size;
    void* _start;
    struct _Node* list;
};

void* alloc(size_t size, struct _alloc_block* block);

void dealloc(void* start);

#endif // BLOCK_H_
