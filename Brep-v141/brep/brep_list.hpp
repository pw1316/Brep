#pragma once

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

void brep_list_clear(BrepList *list);
BrepListIterator brep_list_insert(BrepListIterator it, void *nodeptr);
void *brep_list_remove(BrepListIterator it);
BrepListIterator brep_list_move(BrepListIterator it, void *begin, void *end);
size_t brep_list_size(BrepList *list);

__forceinline BrepListIterator brep_list_begin(BrepList *list)
{
    return list->header.next;
}

__forceinline BrepListIterator brep_list_end(BrepList *list)
{
    return &(list->header);
}

__forceinline BrepListIterator brep_list_next(BrepListIterator it)
{
    return it->next;
}

__forceinline BrepListIterator brep_list_prev(BrepListIterator it)
{
    return it->prev;
}

__forceinline bool brep_list_empty(BrepList *list)
{
    return brep_list_begin(list) == brep_list_end(list);
}
