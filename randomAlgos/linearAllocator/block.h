#ifndef BLOCK_H_
#define BLOCK_H_

#include <stdint.h>
#include "linkedList.h"



struct _alloc_block
{
    size_t _size;
    void* _start;
};

void* alloc(size_t size);

void dealloc(void* start);

#endif // BLOCK_H_
