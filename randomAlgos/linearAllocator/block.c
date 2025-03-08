#include "block.h"

// Sum the values in a linked list
static size_t getSummedValues(struct _Node* head)
{
    return (head->val + ((head->_next == NULL) ? 0 : getSummedValues(head->_next)));
}

void* alloc(size_t allocSize, struct _alloc_block* block)
{
    // is it bigger than the block
    if(allocSize > block->_size)
        return 0x0;

    // get curr position, and determine whether there is enough space left
    // If there isnt enough space, return null
    if((block->_size - getSummedValues(block->list) - allocSize < 0))
       return 0x0;

    return block->_start + getSummedValues(block->list) + 1;
}
