#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "block.h"
#include "linkedList.h"


int main()
{
    printf("Testing linked list functionality");
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

    freeList(head);
    // End of linked list testing
}
