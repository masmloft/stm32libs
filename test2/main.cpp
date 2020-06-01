//#include <string>
//#include <sstream>

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_wwdg.h"

#include <Gpio/Gpio.h>
#include <System/SystemClock.h>

#include "CubeMX/Inc/usart.h"
#include "CubeMX/Inc/usb_device.h"
#include "CubeMX/Inc/usbd_cdc_if.h"

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
    //return 0;
    //t1();

	HAL_Init();
	SystemClock::initClockExt72M();

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	MX_USART1_UART_Init();
	MX_USB_DEVICE_Init();

	GpioC13Out led(GpioSpeed::HIGH);

	led.setHi();

//    std::string s = "drwfw";
//    std::c ss;

    uint32_t i = 0;
	uint32_t iPrint = 0;

	while (1)
	{
		if(i++ & (0x10000 * 4 * 2))
			led = true;
		else
			led = false;

		iPrint++;
		if(iPrint == 0x10000 * 4)
		{
			iPrint = 0;
			uint8_t msg[] = "123\n";
			CDC_Transmit_FS(msg, sizeof(msg));
		}

		//if(i / (0x10000 * 4) == 0x3FFFF)


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
