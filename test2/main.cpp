//#include <string>
//#include <sstream>

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_wwdg.h"

#include <System/SystemClock.h>
#include <Gpio/Gpio.h>
#include <Uart/Uart.h>

#include "CubeMX/Inc/usart.h"
#include "CubeMX/Inc/usb_device.h"
#include "CubeMX/Inc/usbd_cdc_if.h"

#include "UartIt.h"

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
	HAL_Init();
	SystemClock::initClockExt72M();

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
//	MX_USB_DEVICE_Init();

	GpioC13Out led(GpioSpeed::HIGH);

	led.setLow();

//	uint32_t lastTick = HAL_GetTick();

	uartIt3.startRx();

	char nmeaBuf[256];
	size_t nmeaBufPos = 0;

	for(uint32_t i = 0; true; ++i)
	{
//		led.setToggle();
		while(uartIt3.rxBuf().isEmpty() == false)
		{
			if(nmeaBufPos >= sizeof(nmeaBuf))
				nmeaBufPos = 0;
			char b = uartIt3.rxBuf().pop();
			if(b == '$')
			{
				led.setLow();
				nmeaBufPos = 0;
			}
			nmeaBuf[nmeaBufPos++] = b;
			if(b == '\n')
			{
				led.setHi();
//				if(memcmp(nmeaBuf, "$GPRMC,", 7) == 0)
//					uartIt2.write(nmeaBuf, nmeaBufPos);
				if(memcmp(nmeaBuf, "$GPGGA,", 7) == 0)
					uartIt2.write(nmeaBuf, nmeaBufPos);
				nmeaBufPos = 0;
			}
		}
	}
}
