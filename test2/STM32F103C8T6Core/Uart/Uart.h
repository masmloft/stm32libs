#pragma once

#include "stm32f1xx_hal_uart.h"


template<uint32_t TADDR>
class Uart
{
	UART_HandleTypeDef _huart{};
public:
	Uart()
	{
		_huart.Instance = USART2;
		_huart.Init.BaudRate = 115200;
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

	int write(char* buf, size_t bufSize, uint32_t timeout)
	{
		HAL_StatusTypeDef res = HAL_UART_Transmit(&_huart, (uint8_t*)buf, bufSize, timeout);
		return _huart.TxXferCount;
	}
//	void setHi() { gpio_addr()->BSRR = TPIN; }
//	void setLow() { gpio_addr()->BSRR = (uint32_t)TPIN << 16U; }
//	void setVal(bool val) { val ? setHi() : setLow(); }
//	void operator=(bool val) { setVal(val); }
};

typedef Uart<USART2_BASE> Uart2;
