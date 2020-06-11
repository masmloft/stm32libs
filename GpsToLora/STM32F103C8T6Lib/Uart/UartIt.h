#pragma once

#include "stm32f1xx_hal_uart.h"

#include "../Class/RingBuf.h"

class UartIt
{
public:
	using TxBuf = RingBuf<uint8_t, 512>;
	using RxBuf = RingBuf<uint8_t, 512>;
public:
	UartIt(UART_HandleTypeDef* huart)
		: _huart(huart)
	{
	}
public:
	void interruptErr()
	{
	}

	void interruptTx()
	{
		startTx();
	}

	void interruptRx()
	{
		_rxBuf.push(_uart_rxBuf[0]);
		startRx();
	}
public:
	UART_HandleTypeDef* huart() { return _huart; }

	bool startRx()
	{
		HAL_StatusTypeDef res = HAL_UART_Receive_IT(_huart, _uart_rxBuf, sizeof(_uart_rxBuf));
		return (res == HAL_OK);
	}

	RxBuf& rxBuf() { return _rxBuf; }

	const TxBuf& txBuf() { return _txBuf; }

	void write(const char* buf, int size)
	{
		_txBuf.append((const uint8_t*)buf, size);
		startTx();
	}

private:
	UART_HandleTypeDef* _huart;
	uint8_t _uart_txBuf[1];
	uint8_t _uart_rxBuf[1];
	TxBuf _txBuf;
	RxBuf _rxBuf;
private:
	bool startTx()
	{
		if(_huart->gState != HAL_UART_STATE_READY)
			return false;
		if(_txBuf.isEmpty() == false)
		{
			_uart_txBuf[0] = _txBuf.pop();
			HAL_StatusTypeDef res = HAL_UART_Transmit_IT(_huart, _uart_txBuf, sizeof(_uart_txBuf));
			return (res == HAL_OK);
		}
		return true;
	}
};
