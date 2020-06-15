//#include <string>
//#include <sstream>
#include <string.h>

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_wwdg.h"

#include "CubeMX/Inc/usart.h"
//#include "CubeMX/Inc/usb_device.h"
//#include "CubeMX/Inc/usbd_cdc_if.h"

#include "STM32F103C8T6Lib/System/SystemClock.h"
#include "STM32F103C8T6Lib/Gpio/Gpio.h"
//#include "STM32F103C8T6Lib/Uart/Uart.h"
#include "STM32F103C8T6Lib/Uart/UartIt.h"

UartIt uartIt1(&huart1);
//UartIt uartIt2(&huart2);
UartIt uartIt3(&huart3);

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

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart == uartIt1.huart())
		uartIt1.interruptErr();
//	if(huart == uartIt2.huart())
//		uartIt2.interruptErr();
	if(huart == uartIt3.huart())
		uartIt3.interruptErr();
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uartIt1.huart())
		uartIt1.interruptTx();
//	if(huart == uartIt2.huart())
//		uartIt2.interruptTx();
	if(huart == uartIt3.huart())
		uartIt3.interruptTx();
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uartIt1.huart())
		uartIt1.interruptRx();
//	if(huart == uartIt2.huart())
//		uartIt2.interruptRx();
	if(huart == uartIt3.huart())
		uartIt3.interruptRx();
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

	MX_USART1_UART_Init();
	//MX_USART2_UART_Init();
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
					uartIt1.write(nmeaBuf, nmeaBufPos);
				nmeaBufPos = 0;
			}
		}
	}
}

