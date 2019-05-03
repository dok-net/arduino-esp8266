#include "FunctionalInterrupts.h"
#include <Schedule.h>
#include <Arduino.h>

void ICACHE_RAM_ATTR interruptFunctional(const ArgStructure& localArg)
{
	if (localArg.reqScheduledFunction)
	{
		if (localArg.interruptInfo)
		{
			localArg.interruptInfo->value = digitalRead(localArg.interruptInfo->pin);
			localArg.interruptInfo->micro = micros();
		}
		schedule_function([arg = localArg]() { arg.reqScheduledFunction(arg.interruptInfo); });
	}
}

void attachScheduledInterrupt(uint8_t pin, std::function<void(InterruptInfo*)> scheduledIntRoutine, int mode)
{
	InterruptInfo* ii = new InterruptInfo(pin);

	ArgStructure as;
	as.interruptInfo = ii;
	as.reqScheduledFunction = scheduledIntRoutine;

	attachInterrupt(pin, [as]() { interruptFunctional(as); }, mode);
}
