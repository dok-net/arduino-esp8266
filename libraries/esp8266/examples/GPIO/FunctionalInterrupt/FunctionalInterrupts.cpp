#include "FunctionalInterrupts.h"
#include <Schedule.h>
#include <Arduino.h>
#include <memory>

void ICACHE_RAM_ATTR interruptFunctional(const ArgStructure& localArg) {
  if (localArg.reqScheduledFunction) {
    if (localArg.interruptInfo) {
      localArg.interruptInfo->value = digitalRead(localArg.interruptInfo->pin);
      localArg.interruptInfo->micro = micros();
    }
    schedule_function(
      [reqScheduledFunction = localArg.reqScheduledFunction,
                         interruptInfo = *localArg.interruptInfo]() {
      reqScheduledFunction(interruptInfo);
    });
  }
}

void attachScheduledInterrupt(uint8_t pin, std::function<void(const InterruptInfo&)> scheduledIntRoutine, int mode) {
  std::shared_ptr<ArgStructure> arg(new ArgStructure());
  arg->interruptInfo = new InterruptInfo(pin);
  arg->reqScheduledFunction = scheduledIntRoutine;

  attachInterrupt(pin, [arg = move(arg)]() {
    interruptFunctional(*arg);
  }, mode);
}
