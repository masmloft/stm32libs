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

#include "uart.h"

uint8_t uart3_rxBuf[1];
//uint8_t uart2_txBuf[1];
//int uart2_txBuf_size = 0;

//RingBuffer<uint8_t, 1024> gRxBuf;
//RingBuffer<uint8_t, 1024> gTxBuf;

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

	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
//	MX_USB_DEVICE_Init();

	GpioC13Out led(GpioSpeed::HIGH);
//	Uart2 uart2(9600);
//	Uart3 uart3(9600);

	led.setHi();

	uint32_t lastTick = HAL_GetTick();

	HAL_StatusTypeDef res = HAL_UART_Receive_IT(&huart3, uart3_rxBuf, sizeof(uart3_rxBuf));

	char nmeaBuf[256];
	int nmeaBufPos = 0;

	for(uint32_t i = 0; true; ++i)
	{
		while(UartIt3::rxBuf.size() > 0)
		{
			if(nmeaBufPos >= sizeof(nmeaBuf))
				nmeaBufPos = 0;
			char b = UartIt3::rxBuf.pop();
			if(b == '$')
				nmeaBufPos = 0;

			nmeaBuf[nmeaBufPos++] = b;
			if(b == '\n')
			{
//				if(memcmp(nmeaBuf, "$GPRMC,", 7) == 0)
//					UartIt2::send(nmeaBuf, nmeaBufPos);
				if(memcmp(nmeaBuf, "$GPGGA,", 7) == 0)
					UartIt2::send(nmeaBuf, nmeaBufPos);
//				if(memcmp(nmeaBuf, "$GPGGA,", 7) == 0)
//				{

//				}

				nmeaBufPos = 0;
			}
		}

	}
}
