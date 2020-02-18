#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_wwdg.h"

#include <Gpio/Gpio.h>
#include <System/SystemClock.h>

#ifdef __cplusplus
extern "C" {
#endif

void _Error_Handler(char *file, int line)
{
	(void)file;
	(void)line;
	while(1)
	{
	}
}

#ifdef __cplusplus
}
#endif

void t1();

int main(void)
{
	//t1();

	HAL_Init();
	SystemClock::initClockExt72M();

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GpioC13Out led(GpioSpeed::HIGH);

	uint32_t i = 0;

	led.setHi();
	while (1)
	{
		if(i++ & (0x10000 * 4))
			led = true;
		else
			led = false;

		//		for(int i = 0; i < 100000000 ; ++i)
		//		{
		//			GPIOC->BSRR = GPIO_BSRR_BS13;
		//			GPIOC->BSRR = GPIO_BSRR_BR13;
		//		}

		//		for(int i = 0; i < 100000000 ; ++i)
		//		{
		//			GPIOC->ODR |= GPIO_ODR_ODR13;
		//			GPIOC->ODR &= ~GPIO_ODR_ODR13;
		//		}

		//		for(int i = 0; i < 100000000 ; ++i)
		//		{
		//			GPIOC->ODR = GPIO_ODR_ODR13;
		//			GPIOC->ODR = 0;
		//		}

	}
	led = true;
}
