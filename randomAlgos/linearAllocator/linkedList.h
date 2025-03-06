#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

struct _Node;

struct _Node
{
    struct _Node* _next;
    size_t val;
};

bool addElement(struct _Node* head, size_t val);
struct _Node* getElementIndex(struct _Node* head, size_t index);
size_t getLength(struct _Node* head);
struct _Node* removeElementIndex(struct _Node* head, size_t index);
struct _Node* getHead(struct _Node* head);
struct _Node* getTail(struct _Node* head);
void freeList(struct _Node* head);

#endif // LINKEDLIST_H_
