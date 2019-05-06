#ifndef FUNCTIONALINTERRUPTS_H
#define FUNCTIONALINTERRUPTS_H

#include <Arduino.h>
#include <functional>

// Structures for communication

struct InterruptInfo
{
    InterruptInfo(uint8_t _pin) : pin(_pin) {}
    const uint8_t pin;
    uint8_t value = 0;
    uint32_t micro = 0;
};

struct ArgStructure
{
    ArgStructure() = default;
    ~ArgStructure()
    {
        delete interruptInfo;
    }
    ArgStructure(const ArgStructure& as) noexcept :
        interruptInfo(as.interruptInfo),
        reqScheduledFunction(as.reqScheduledFunction)
    { }
    ArgStructure(ArgStructure&& as) noexcept :
        interruptInfo(as.interruptInfo),
        reqScheduledFunction(as.reqScheduledFunction)
    {
        as.interruptInfo = nullptr;
    }
    ArgStructure& operator=(const ArgStructure& as) noexcept
    {
        interruptInfo = as.interruptInfo;
        reqScheduledFunction = as.reqScheduledFunction;
        return *this;
    }
    ArgStructure& operator=(ArgStructure&& as) noexcept
    {
        interruptInfo = as.interruptInfo;
        as.interruptInfo = nullptr;
        reqScheduledFunction = as.reqScheduledFunction;
        return *this;
    }
    InterruptInfo* interruptInfo = nullptr;
    std::function<void(const InterruptInfo&)> reqScheduledFunction = nullptr;
};

void attachScheduledInterrupt(uint8_t pin, std::function<void(const InterruptInfo&)> scheduledIntRoutine, int mode);

#endif //INTERRUPTS_H
