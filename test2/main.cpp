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
//	MX_USART3_UART_Init();
//	MX_USB_DEVICE_Init();

	GpioC13Out led(GpioSpeed::HIGH);
	Uart2 uart2(9600);

	led.setHi();

	uint32_t lastTick = HAL_GetTick();

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
		if(rxSize > 0)
			uart2.write(rxBuf, rxSize);

		uint32_t currTick = HAL_GetTick();
		if(currTick - lastTick > 1000)
		{
			sprintf(rxBuf, "$GPRMC,%u,A,5542.2389,N,03741.6063,E,,,040620,,*2D\r\n", currTick);
			uart2.write(rxBuf);
//			uart2.write("$GPRMC,121412.610,V,,,,,,,040620,,*2D\r\n");
			lastTick = currTick;
		}



//		iPrint++;
//		if(iPrint == 0x10000 * 4)
//		{
//			iPrint = 0;
//			uint8_t msg[] = "ok\r\n";
//			CDC_Transmit_FS(msg, sizeof(msg));
//		}

	}
}
