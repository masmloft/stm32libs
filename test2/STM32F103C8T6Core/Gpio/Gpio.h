#ifndef GPIO_H
#define GPIO_H

#include "stm32f1xx_hal_gpio.h"

enum class GpioSpeed
{
	LOW = GPIO_SPEED_FREQ_LOW,
	MEDIUM = GPIO_SPEED_FREQ_MEDIUM,
	HIGH = GPIO_SPEED_FREQ_HIGH
};

template<uint16_t TPIN>
class GpioCOut
{

public:
	GpioCOut(GpioSpeed gpioSpeed)
	{
		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(GPIOC, TPIN, GPIO_PIN_RESET);

		{
			GPIO_InitTypeDef GPIO_InitStruct;
			GPIO_InitStruct.Pin = TPIN;
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			//GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
			GPIO_InitStruct.Speed = uint32_t(gpioSpeed);
			HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		}
	}
	void setHi() { GPIOC->BSRR = TPIN; }
	void setLow() { GPIOC->BSRR = (uint32_t)TPIN << 16U; }
	void setVal(bool val) { val ? setHi() : setLow(); }
	void operator=(bool val) { setVal(val); }
};

typedef GpioCOut<GPIO_PIN_13> GpioC13Out;

#endif // GPIO_H
