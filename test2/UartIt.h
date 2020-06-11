#pragma once

#include "stm32f1xx_hal.h"
#include "Class/RingBuf.h"

class UartIt
{
public:
	using TxBuf = RingBuf<uint8_t, 256>;
	using RxBuf = RingBuf<uint8_t, 512>;
public:
	static UartIt* getUartIt(UART_HandleTypeDef* huart);
	static void interruptTx(UART_HandleTypeDef* huart);
	static void interruptRx(UART_HandleTypeDef* huart);
public:
	UartIt(UART_HandleTypeDef* huart);
	~UartIt();
public:
	bool startRxIt();
	bool startTxIt();

	RxBuf& rxBuf() { return _rxBuf; }
	void write(const char* buf, int size);


//	static void send(char val);
//	static void send(const char* buf, int size);
//	static void recvIt();
private:
	struct UartData
	{
		UART_HandleTypeDef* huart;
		UartIt* uartIt;
		uint8_t uart_txBuf[1];
		uint8_t uart_rxBuf[1];
	};
private:
	static UartData _uartDatas[4];
	UartData* _uartData = nullptr;
	TxBuf _txBuf;
	RxBuf _rxBuf;
};
