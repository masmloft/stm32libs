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
	__HAL_RCC_GPIOB_CLK_ENABLE();

//	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_USB_DEVICE_Init();

	GpioC13Out led(GpioSpeed::HIGH);

	Uart2 uart2;

	led.setHi();

//    std::string s = "drwfw";
//    std::c ss;

	//uint32_t i = 0;
	uint32_t iPrint = 0;

	for(uint32_t i = 0; ; ++i)
	{
//		if(i++ & (0x10000 * 4 * 2))
//		if(i++ & (0x100))
//			led = true;
//		else
//			led = false;

		led.setToggle();

		char rxBuf[256];

		int rxSize = uart2.read(rxBuf, sizeof(rxBuf), 10);
		uart2.write(rxBuf, rxSize);


		{
//			char txBuf[] = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\r\n";
//			sprintf(txBuf, "%u\r\n", i);
//			uart2.write(txBuf);
		}

//		iPrint++;
//		if(iPrint == 0x10000 * 4)
//		{
//			iPrint = 0;
//			uint8_t msg[] = "ok\r\n";
//			CDC_Transmit_FS(msg, sizeof(msg));
//		}

//		{
//			uint8_t rxBuf[256];
//			HAL_UART_Receive(&huart2, rxBuf, sizeof(rxBuf), 100);
//			int16_t rxSize = huart2.RxXferSize - huart2.RxXferCount - 1;

//			if(rxSize > 0)
//			{
////				if(rxSize > 1)
////					HAL_UART_Transmit(&huart2, (uint8_t*)"!", 3, 100);
//				HAL_UART_Transmit(&huart2, rxBuf, rxSize, 100);
//			}
//		}



	}
}
