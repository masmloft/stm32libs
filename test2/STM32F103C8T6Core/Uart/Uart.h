#pragma once

#include <string.h>

#include "stm32f1xx_hal_uart.h"


template<uint32_t TADDR>
class Uart
{
	UART_HandleTypeDef _huart{};
	uint32_t _txTimeout = 100;
public:
	Uart(uint32_t baudRate)
	{
		_huart.Instance = USART2;
		_huart.Init.BaudRate = baudRate;
		_huart.Init.WordLength = UART_WORDLENGTH_8B;
		_huart.Init.StopBits = UART_STOPBITS_1;
		_huart.Init.Parity = UART_PARITY_NONE;
		_huart.Init.Mode = UART_MODE_TX_RX;
		_huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		_huart.Init.OverSampling = UART_OVERSAMPLING_16;
		if (HAL_UART_Init(&_huart) != HAL_OK)
		{
		  _Error_Handler(__FILE__, __LINE__);
		}
	}

	int write(const char* buf, int bufSize)
	{
		if(bufSize <= 0)
			return 0;
		HAL_StatusTypeDef res = HAL_UART_Transmit(&_huart, (uint8_t*)buf, bufSize, _txTimeout);
		return _huart.TxXferSize - _huart.TxXferCount;
	}

	int write(const char* buf)
	{
		int bufSize = strlen(buf);
		return write(buf, bufSize);
	}

	int read(char* buf, int bufSize, uint32_t timeout)
	{
		HAL_StatusTypeDef res = HAL_UART_Receive(&_huart, (uint8_t*)buf, bufSize, timeout);
		int rxSize = _huart.RxXferSize - _huart.RxXferCount - 1;
		return rxSize;
	}

};

typedef Uart<USART2_BASE> Uart2;
