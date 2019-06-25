#include "FunctionalInterrupt.h"
#include <Schedule.h>
#include <Arduino.h>

namespace {

    void ICACHE_RAM_ATTR interruptScheduleFunctional(uint8_t pin, std::function<void(InterruptInfo)> scheduledIntRoutine)
    {
        InterruptInfo interruptInfo(pin);
        interruptInfo.value = digitalRead(pin);
        interruptInfo.micro = micros();
        schedule_function(std::bind(std::move(scheduledIntRoutine), std::move(interruptInfo)));
    }

}

void attachScheduledInterrupt(uint8_t pin, std::function<void(InterruptInfo)> scheduledIntRoutine, int mode)
{
    if (scheduledIntRoutine)
    {
        attachInterrupt(pin, std::bind(interruptScheduleFunctional, pin, std::move(scheduledIntRoutine)), mode);
    }
}
