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

#include "Lora.h"

uint8_t uart3_rxBuf[1];
//uint8_t uart2_txBuf[1];
//int uart2_txBuf_size = 0;

RingBuffer<uint8_t, 1024> gRxBuf;
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

	char txBuf[256];
	int txPos = 0;

	for(uint32_t i = 0; true; ++i)
	{
//		if(gRxBuf.isEmpty() == false)
//		{
//			while(gRxBuf.size() > 0)
//				lora_send(gRxBuf.pop());
//		}

		while(gRxBuf.size() > 0)
		{
			if(txPos >= sizeof(txBuf))
				txPos = 0;
			char b = gRxBuf.pop();
			if(b == '$')
				txPos = 0;

			txBuf[txPos++] = b;
			if(b == '\n')
			{
				if(memcmp(txBuf, "$GPRMC,", 7) == 0)
					lora_send(txBuf, txPos);
				if(memcmp(txBuf, "$GPGGA,", 7) == 0)
					lora_send(txBuf, txPos);
				txPos = 0;
			}
		}

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


//		if(huart2.gState == HAL_UART_STATE_READY)
//		{
//			if(gRxBuf.isEmpty() == false)
//			{
//				uart2_txBuf[0] =gRxBuf.pop();
//				HAL_UART_Transmit_IT(&huart2, uart2_txBuf, sizeof(uart2_txBuf));
//			}
//		}
	}
}

//extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart == &huart2)
//	{
//		if(gTxBuf.isEmpty() == false)
//		{
//			uart2_txBuf[0] = gTxBuf.pop();
//			HAL_UART_Transmit_IT(&huart2, uart2_txBuf, sizeof(uart2_txBuf));
//		}
//	}
//}
