#include "block.h"

// Sum the values in a linked list
static size_t getSummedValues(struct _Node* head)
{
    return (head->val + ((head->_next == NULL) ? 0 : getSummedValues(head->_next)));
}

void* alloc(size_t allocSize, struct _alloc_block* block)
{
    // There is def a better... less sketchy way to do this

    // is it bigger than the block
    if(allocSize > block->_size)
        return 0x0;

    // get curr position, and determine whether there is enough space left
    // If there isnt enough space, return null
    if((block->_size - getSummedValues(block->list) < 0))
       return 0x0;

    addElement(block->list, allocSize);

    return (block->_start + getSummedValues(block->list));
}

void dealloc(void* start, struct _alloc_block* block)
{
    // itterate through the list till we find the block that the memory is allocated to
}
