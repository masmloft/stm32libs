#pragma once

#include <string.h>
#include <algorithm>

#include "stm32f1xx_hal_uart.h"


template<uint32_t TADDR>
class Uart
{
	UART_HandleTypeDef _huart{};
	uint32_t _txTimeout = 100;
	char _txBuf[256];
public:
	Uart(uint32_t baudRate)
	{
		_huart.Instance = ((USART_TypeDef *)TADDR); //USART2;
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
public:
	UART_HandleTypeDef& huart() { return _huart; }

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

	void writeIt(const char* buf, int bufSize)
	{
		if(bufSize <= 0)
			return;
		const int s = std::min((int)sizeof(_txBuf), bufSize);
		memcpy(_txBuf, buf, s);
		HAL_StatusTypeDef res = HAL_UART_Transmit_IT(&_huart, (uint8_t*)_txBuf, s);
		//return _huart.TxXferSize - _huart.TxXferCount;
	}

	int read(char* buf, int bufSize, uint32_t timeout)
	{
		HAL_StatusTypeDef res = HAL_UART_Receive(&_huart, (uint8_t*)buf, bufSize, timeout);
		int rxSize = _huart.RxXferSize - _huart.RxXferCount - 1;
		return rxSize;
	}

};

typedef Uart<USART2_BASE> Uart2;
typedef Uart<USART3_BASE> Uart3;
