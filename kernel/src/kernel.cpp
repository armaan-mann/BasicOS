#include "kernelUtil.h"
#include "heap/heap.h"
#include "scheduling/pit.h"

extern "C" void _start(BootInfo* bootInfo){

    KernelInfo kernelInfo = InitializeKernel(bootInfo);

    while(true){
        asm("hlt"); //pause the CPU when it's not doing anything
    }
}