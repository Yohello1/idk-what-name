#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "block.h"
#include "linkedList.h"


int main()
{
    printf("Testing linked list functionality\n");
    // Linked list testing
    struct _Node* head = malloc(sizeof(struct _Node));
    {
        head->val = 9;
        head->_next = NULL;
    }

    // adding a bunch of elements
    {
        addElement(head, 15);
        addElement(head, 24);
        addElement(head, 36);
    }

    // Getting an element at a certain index
    {
        assert(getElementIndex(head, 2)->val == 24);
        assert(getElementIndex(head, 3)->val == 36);
    }

    // Removing an elemnt at a certain index
    {
        assert(getLength(head) == 4);
        head = removeElementIndex(head, 2);
        assert(getLength(head) == 3);
    }

    // Getting the length of the linked list
    {
        assert(getLength(head) == 3);
    }

    // Get head
    {
        assert(getHead(head) == head);
    }

    // Get tail
    {
        assert(getTail(head) == getElementIndex(head, 2));
    }


    freeList(head);
    // End of linked list testing
}
