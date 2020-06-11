#include "uart.h"

#include "stm32f1xx_hal.h"
#include "CubeMX/Inc/usart.h"

static uint8_t uart2_txBuf[1];
static uint8_t uart2_rxBuf[1];
RingBuf<uint8_t, 1024> UartIt2::txBuf;

static uint8_t uart3_txBuf[1];
static uint8_t uart3_rxBuf[1];
RingBuf<uint8_t, 1024> UartIt3::rxBuf;

void UartIt2::send(char val)
{
	if(huart2.gState == HAL_UART_STATE_READY)
	{
		uart2_txBuf[0] = val;
		HAL_UART_Transmit_IT(&huart2, uart2_txBuf, sizeof(uart2_txBuf));
	}
	else
	{
		UartIt2::txBuf.push(val);
	}
}

void UartIt2::send(const char* buf, int size)
{
	if(huart2.gState == HAL_UART_STATE_READY)
	{
		UartIt2::txBuf.append((const uint8_t*)buf + 1, size - 1);
		uart2_txBuf[0] = buf[0];
		HAL_UART_Transmit_IT(&huart2, uart2_txBuf, sizeof(uart2_txBuf));
	}
	else
	{
		UartIt2::txBuf.append((const uint8_t*)buf, size);
	}
}

void UartIt2::recvIt()
{
	HAL_StatusTypeDef res = HAL_UART_Receive_IT(&huart2, uart2_rxBuf, sizeof(uart2_rxBuf));
}

void UartIt3::recvIt()
{
	HAL_StatusTypeDef res = HAL_UART_Receive_IT(&huart3, uart3_rxBuf, sizeof(uart3_rxBuf));
}

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart3)
	{
		HAL_UART_Transmit(&huart2, (uint8_t*)"\r\nERR\r\n", 7, 1000);
		UartIt3::recvIt();
	}
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart2)
	{
		if(UartIt2::txBuf.isEmpty() == false)
		{
			uart2_txBuf[0] = UartIt2::txBuf.pop();
			HAL_UART_Transmit_IT(&huart2, uart2_txBuf, sizeof(uart2_txBuf));
		}
	}
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart3)
	{
		UartIt3::rxBuf.push(uart3_rxBuf[0]);
		UartIt3::recvIt();
	}
}
