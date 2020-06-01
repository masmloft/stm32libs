#pragma once

#include "stm32f1xx_hal.h"

class SystemClock
{
	SystemClock();
public:
	static void initClockInt8M();
	static void initClockInt64M();
	static void initClockExt72M();
};

