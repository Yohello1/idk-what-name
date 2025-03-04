#include "linkedList.h"

// Add an element to the end of the list
// head: struct _Node*
// val: size_t
bool addElement(struct _Node* head, size_t val)
{
    while((head->_next) != NULL)
    {
        head = head->_next;
    }
    struct _Node* temp = malloc(sizeof(struct _Node));

    head->_next = temp;
    temp->val = val;
    temp->_next = NULL;

    return true; // Should return false if the malloc fails, but Im lazy :P
}

// Gets an element in the list by index
// head: struct _Node*
// index: size_t
struct _Node* getElementIndex(struct _Node* head, size_t index)
{
    for(size_t i = 0; i < index; i++)
    {
        head = head->_next;
    }
    return head;
}

// Gets the size of the linked list
// head: struct _Node*
size_t getlength(struct _Node* head)
{
    size_t i = 0;
    while((head->_next) != NULL)
    {
        i++;
        head = head->_next;
    }
    return i;
}

// Removes an element based upon the index
// head: struct _Node*
// index: size_t
void removeElementIndex(struct _Node* head, size_t index)
{
    // FIXME: Does not handle first, and last elements
    struct _Node* elementBefore = getElementIndex(head, index - 1); // get the element before
    struct _Node* elementNext = (elementBefore->_next)->_next; // get the next-next element

    // Free the element at the specific index
    free(elementBefore->_next);

    // set the ptr from before, to point to next
    elementBefore->_next = elementNext;
}


// Gets the head of the linked list
// head: struct _Node*
struct _Node* getHead(struct _Node* head)
{
    return head;
}

// Gets the tail of the linked list
// head: struct _Node*
struct _Node* getTail(struct _Node* head)
{
    while(head->_next != NULL)
    {
        head = head->_next;
    }
    return head;
}

// Frees the entire list
// head: struct _Node*
void freeList(struct _Node* head)
{
    while(head->_next != NULL)
    {
        struct _Node* temp = head->_next;
        free(head);
        head = temp;
    }
    free(head);
    head = NULL;
}
