#include "IO.h"

void outb(uint16_t port, uint8_t value){
    //puts a byte onto the BUS, selects the device it want's to talk to through 'port'
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port){
    //reads the byte from a specific port 
    uint8_t returnVal;
    asm volatile ("inb %1, %0"
    : "=a"(returnVal)
    : "Nd"(port));
    return returnVal;
}

void io_wait(){
    // waits for some time, to account for slower devices
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}