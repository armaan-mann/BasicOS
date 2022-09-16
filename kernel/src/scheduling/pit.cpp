#include "pit.h"
#include "../IO.h"

namespace PIT{
    double bootTime = 0;
    uint16_t Divisor = 65535; //slowest delay

    void setDivisor(uint16_t divisor){
        if (divisor < 100) divisor = 100;

        Divisor = divisor;
        outb(0x40, (uint8_t)(divisor & 0x00ff));
        io_wait();
        outb(0x40, (uint8_t)((divisor & 0xff00) >> 8));
    }

    uint64_t getFrequency(){
        //divide base frequency by divisor
        return BaseFrequency / Divisor;
    }

    void setFrequency(uint64_t frequency){
        setDivisor(BaseFrequency / frequency);
    }

    void Tick(){
        bootTime += 1 / (double) getFrequency();
    }

    void Sleepd(double secs){
        double initalTime = bootTime;
        while(bootTime < initalTime + secs){
            asm("hlt"); //interrupt
        }
    }

    void Sleep(uint64_t msecs){
        Sleepd((double)msecs / 1000);
    }



}