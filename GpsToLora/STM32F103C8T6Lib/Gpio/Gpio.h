#pragma once

#include "stm32f1xx_hal_gpio.h"

enum class GpioSpeed
{
	LOW = GPIO_SPEED_FREQ_LOW,
	MEDIUM = GPIO_SPEED_FREQ_MEDIUM,
	HIGH = GPIO_SPEED_FREQ_HIGH
};

template<uint32_t TGPIO_ADDR, uint16_t TPIN>
class GpioOut
{
	constexpr GPIO_TypeDef* gpio_addr() { return ((GPIO_TypeDef*)TGPIO_ADDR); }
public:
	GpioOut(GpioSpeed gpioSpeed)
	{
		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(gpio_addr(), TPIN, GPIO_PIN_RESET);

		{
			GPIO_InitTypeDef GPIO_InitStruct;
			GPIO_InitStruct.Pin = TPIN;
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			//GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
			GPIO_InitStruct.Speed = uint32_t(gpioSpeed);
			HAL_GPIO_Init(gpio_addr(), &GPIO_InitStruct);
		}
	}
	void setHi() { gpio_addr()->BSRR = TPIN; }
	void setLow() { gpio_addr()->BSRR = (uint32_t)TPIN << 16U; }
	void setToggle() { gpio_addr()->ODR ^= TPIN; }

	void setVal(bool val) { val ? setHi() : setLow(); }
	void operator=(bool val) { setVal(val); }
};

typedef GpioOut<GPIOA_BASE, GPIO_PIN_8> GpioA8Out;

typedef GpioOut<GPIOC_BASE, GPIO_PIN_13> GpioC13Out;
typedef GpioOut<GPIOC_BASE, GPIO_PIN_14> GpioC14Out;
