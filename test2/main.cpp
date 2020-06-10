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

#include "Class/RingBuffer.h"

uint8_t uart3_rxBuf[1];
uint8_t uart2_txBuf[1];
//int uart2_txBuf_size = 0;

RingBuffer<uint8_t, 1024> gRxBuf;

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

	for(uint32_t i = 0; ; ++i)
	{
//		HAL_StatusTypeDef res = HAL_UART_Receive_IT(&huart3, uart3_rxBuf, sizeof(uart3_rxBuf));
		//if(res == HAL_OK)
//		if(huart3.RxXferCount == 0)
//		{
//			HAL_StatusTypeDef res = HAL_UART_Transmit(&huart2, (uint8_t*)rxBuf, 1, 10);
//		}
//		else
//		{
//			HAL_StatusTypeDef res = HAL_UART_Transmit(&huart2, (uint8_t*)"BUSY\r\n", 6, 10);
//		}

//		int rxSize = uart3.read(rxBuf, sizeof(rxBuf), 10);



//		int rxSize = uart3.read(rxBuf, sizeof(rxBuf), 10);
//		if(rxSize > 0)
//		{
//			led.setToggle();
//			uart2.write(rxBuf, rxSize);
//			//uart2.writeIt(rxBuf, rxSize);
//		}
//		else
//		{
//			led.setHi();
//		}

//		uint32_t currTick = HAL_GetTick();
//		if(currTick - lastTick > 1000)
//		{
//			sprintf(rxBuf, "$GPRMC,%u,A,5542.2389,N,03741.6063,E,,,040620,,*2D\r\n", currTick);
//			uart2.write(rxBuf);
////			uart2.write("$GPRMC,121412.610,V,,,,,,,040620,,*2D\r\n");
//			lastTick = currTick;
//		}

//		iPrint++;
//		if(iPrint == 0x10000 * 4)
//		{
//			iPrint = 0;
//			uint8_t msg[] = "ok\r\n";
//			CDC_Transmit_FS(msg, sizeof(msg));
//		}

	}
}

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart3)
	{
		HAL_UART_Transmit(&huart2, (uint8_t*)"\r\nERR\r\n", 7, 1000);
		HAL_StatusTypeDef res = HAL_UART_Receive_IT(&huart3, uart3_rxBuf, sizeof(uart3_rxBuf));
	}
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart3)
	{
		HAL_StatusTypeDef res = HAL_UART_Receive_IT(&huart3, uart3_rxBuf, sizeof(uart3_rxBuf));

		gRxBuf.push(uart3_rxBuf[0]);

		if(huart2.gState == HAL_UART_STATE_READY)
		{
			if(gRxBuf.isEmpty() == false)
			{
				uart2_txBuf[0] =gRxBuf.pop();
				HAL_UART_Transmit_IT(&huart2, uart2_txBuf, sizeof(uart2_txBuf));
			}
		}
	}
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart2)
	{
		if(gRxBuf.isEmpty() == false)
		{
			uart2_txBuf[0] =gRxBuf.pop();
			HAL_UART_Transmit_IT(&huart2, uart2_txBuf, sizeof(uart2_txBuf));
		}
	}
}
