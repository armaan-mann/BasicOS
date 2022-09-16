#include "heap.h"
#include "../paging/PageTableManager.h"
#include "../paging/PageFrameAllocator.h"

void* startOfHeap;
void* endOfHeap;
HeapSegHeader* lastHeader;

void makeHeap(void* heapAddr, size_t heapSize){
    //local addr for heap addr 
    void* localAddr = heapAddr;

    //map memory to heap addr
    for (size_t i = 0; i < heapSize; i++){
        mainPageTableManager.MapMemory(localAddr, mainAllocator.RequestPage());
        localAddr = (void*)((size_t)localAddr + 0x1000);
    }
    size_t heapLength = heapSize * 0x1000;
    startOfHeap = heapAddr;
    endOfHeap = (void*)((size_t)startOfHeap + heapLength);
    HeapSegHeader* segStart = (HeapSegHeader*)heapAddr;
    segStart->length = heapLength - sizeof(HeapSegHeader);
    segStart->next = NULL;
    segStart->last = NULL;
    segStart->isFree = true;
    lastHeader = segStart;
}

void* free(void* address){
    HeapSegHeader* seg = (HeapSegHeader*) address - 1;
    seg->isFree = true;
    seg->combineBefore();
    seg->combinenextLine();
}

void* malloc(size_t size){
    //round size to a 64 bit number
    if (size % 0x10 > 0) // not a multiple of 128
    {
        size -= (size % 0x010);
        size += 0x10;
    }

    if(size == 0) return NULL;

    HeapSegHeader* segment = (HeapSegHeader*) startOfHeap;
    while (true){
        if (segment->isFree)
        {
            if(segment->length > size){
                segment->Split(size);
                segment->isFree = false;
                return (void*)((uint64_t)segment + sizeof(HeapSegHeader));
            }
            
            if(segment->length == size){
                segment->isFree = false;
                return (void*)((uint64_t)segment + sizeof(HeapSegHeader));
            }

        }

        if(segment->next == NULL) break;
        segment = segment->next;
    }
    
    //segment smaller than size
    expandHeap(size);
    return malloc(size);
    
}

HeapSegHeader* HeapSegHeader::Split(size_t Splittinglength){
    if(Splittinglength < 0x10) return NULL;

    int64_t newLength = length - Splittinglength - (sizeof(HeapSegHeader));
    if(newLength < 0x10) return NULL;

    HeapSegHeader* newHeader = (HeapSegHeader*) ((size_t)this + Splittinglength + sizeof(HeapSegHeader));
    next->last = newHeader;
    newHeader->next = next;
    next = newHeader;
    newHeader->last = this;
    newHeader->length = newLength;
    newHeader->isFree = free;
    length = Splittinglength;

    if (lastHeader == this) lastHeader = newHeader;
    return newHeader;
}

void expandHeap(size_t length){
    //round page upto full page length
    if(length % 0x1000){
        length -= length % 0x1000;
        length += 0x1000;
    }

    size_t pageLength = length / 0x1000;

    //store a refereance to current heap
    HeapSegHeader* newSeg = (HeapSegHeader*)endOfHeap;
    for (size_t i = 0; i < pageLength; i++){
        mainPageTableManager.MapMemory(endOfHeap, mainAllocator.RequestPage());
        endOfHeap = (void*)((size_t)endOfHeap + 0x1000);
    }

    //set up the right values in the header

    newSeg->isFree = true;
    newSeg->last = lastHeader;
    lastHeader->next = newSeg;
    lastHeader = newSeg;
    newSeg->next = NULL;
    newSeg->length = length - sizeof(HeapSegHeader);
    newSeg ->combineBefore();
    

}

void HeapSegHeader::combinenextLine(){
    if(next == NULL) return;
    if(!next->isFree) return;

    if(next == lastHeader) lastHeader = this;

    if(next->next != NULL){
        next->next->last = this;
    }

    next = next->next;

    length = length + next->length + sizeof(HeapSegHeader);
}

void HeapSegHeader::combineBefore(){
    if (last != NULL & last->isFree) last->combinenextLine();
}