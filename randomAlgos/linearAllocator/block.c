#include "block.h"

// Sum the values in a linked list
static size_t getSummedValues(struct _Node* head)
{
    /* return (head->val + ((head->_next == NULL) ? 0 : getSummedValues(head->_next))); */
    return ((head->_next == NULL) ? head->val : getSummedValues(head->_next) + head->val);
}

// Alloc needs to look for the smallest contigious block that is large enough
void* alloc(size_t allocSize, struct _alloc_block* block)
{
    // There is def a better... less sketchy way to do this

    // is it bigger than the block
    if(allocSize > block->_size)
        return 0x0;

    // get curr position, and determine whether there is enough space left
    // If there isnt enough space, return null
    struct _Node* head = block->list;


    addElement(block->list, allocSize, allocSize);

    return (block->_start + getSummedValues(block->list));
}


// Dealloc simply needs to set area to 0, and then remove from linked list
void dealloc(void* start, struct _alloc_block* block)
{
    size_t prevSize;
    void* i = block->_start;
    struct _Node* temp = block->list;

    while(i < start)
    {
        if(temp == NULL)
            return;
        prevSize = temp->val;
        i += temp->val;
        temp = temp->_next;
    }

    memcpy(i-prevSize, i, getSummedValues(temp));

}
