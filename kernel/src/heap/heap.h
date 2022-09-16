#pragma once
#include <stdint.h>
#include <stddef.h>

//header that goes before each mem block
struct HeapSegHeader{
    size_t length;
    HeapSegHeader* next;
    HeapSegHeader* last;
    bool isFree;
    void combinenextLine();
    void combineBefore();
    HeapSegHeader* Split(size_t splitLength);
};

void makeHeap(void* heapAddr, size_t heapSize);

void* malloc(size_t size);
void* free(void* addr);
void expandHeap(size_t expandLength);