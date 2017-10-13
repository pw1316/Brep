#include "brep_list.hpp"

void brep_list_clear(BrepList *list)
{
    list->header.next = &(list->header);
    list->header.prev = &(list->header);
}

BrepListIterator brep_list_insert(BrepListIterator it, void *nodeptr)
{
    BrepListIterator res = reinterpret_cast<BrepListIterator>(nodeptr);
    res->prev = it->prev;
    res->next = it;
    res->prev->next = res;
    it->prev = res;
    return res;
}

void *brep_list_remove(BrepListIterator it)
{
    it->prev->next = it->next;
    it->next->prev = it->prev;
    return it;
}

BrepListIterator brep_list_move(BrepListIterator it, void *begin, void *end)
{
    BrepListIterator first = reinterpret_cast<BrepListIterator>(begin);
    BrepListIterator last = reinterpret_cast<BrepListIterator>(end);
    first->prev->next = last->next;
    last->next->prev = first->prev;
    first->prev = it->prev;
    last->next = it;
    first->prev->next = first;
    last->next->prev = last;
    return first;
}

size_t brep_list_size(BrepList *list)
{
    size_t size = 0;
    for (BrepListIterator it = brep_list_begin(list); it != brep_list_end(list); it = brep_list_next(it))
    {
        ++size;
    }
    return size;
}
