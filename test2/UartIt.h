#pragma once

#include "stm32f1xx_hal.h"
#include "Class/RingBuf.h"

class UartIt
{
public:
	constexpr static size_t UART_COUNT = 3;
	using TxBuf = RingBuf<uint8_t, 512>;
	using RxBuf = RingBuf<uint8_t, 512>;
public:
	static UartIt* getUartIt(UART_HandleTypeDef* huart);
	static void interruptErr(UART_HandleTypeDef* huart);
	static void interruptTx(UART_HandleTypeDef* huart);
	static void interruptRx(UART_HandleTypeDef* huart);
public:
	UartIt(UART_HandleTypeDef* huart);
	~UartIt();
public:
	bool startRxIt();

	RxBuf& rxBuf() { return _rxBuf; }
	const TxBuf& txBuf() { return _txBuf; }
	void write(const char* buf, int size);
private:
	struct UartData
	{
		UART_HandleTypeDef* huart;
		UartIt* uartIt;
		uint8_t uart_txBuf[1];
		uint8_t uart_rxBuf[1];
	};
private:
	static UartData _allUartData[UART_COUNT];
private:
	UartData* _uartData = nullptr;
	TxBuf _txBuf;
	RxBuf _rxBuf;
private:
	bool startTxIt();
};

