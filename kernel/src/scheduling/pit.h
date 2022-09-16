#pragma once
#include <stdint.h>

namespace PIT{
    extern double bootTime; //time taken since the boot 
    const uint64_t BaseFrequency = 1193182; //PIT chip oscillates per second

    void Sleepd(double seconds);
    void Sleep(uint64_t mseconds);
    uint64_t getFrequency();
    void setFrequency(uint64_t frequency);
    void setDivisor(uint16_t divisor);
    void Tick();
    
}