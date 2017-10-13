#pragma once
#include <list>

struct BrepListNode
{
    BrepListNode *prev;
    BrepListNode *next;
};

using BrepListIterator = BrepListNode*;

struct BrepList
{
    BrepListNode header;
};

void brep_list_clear(BrepList *);
BrepListIterator brep_list_insert(BrepListIterator, void *);
void *brep_list_remove(BrepListIterator);
BrepListIterator brep_list_move(BrepListIterator, void *, void *);
size_t brep_list_size(BrepList *);

__forceinline BrepListNode *brep_list_begin(BrepList *list)
{
    std::list<int> l;
    return list->header.next;
}

