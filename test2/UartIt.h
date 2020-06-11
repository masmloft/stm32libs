#pragma once

#include "Class/RingBuf.h"

class UartIt
{
public:
	using TxBuf = RingBuf<uint8_t, 512>;
	using RxBuf = RingBuf<uint8_t, 512>;
public:
	UartIt(void* huart);
	~UartIt();
public:
	void interruptErr();
	void interruptTx();
	void interruptRx();
public:
	void* huart() { return _huart; }
	bool startRx();
	RxBuf& rxBuf() { return _rxBuf; }
	const TxBuf& txBuf() { return _txBuf; }
	void write(const char* buf, int size);
private:
	void* _huart;
	uint8_t _uart_txBuf[1];
	uint8_t _uart_rxBuf[1];
	TxBuf _txBuf;
	RxBuf _rxBuf;
private:
	bool startTx();
};

extern UartIt uartIt2;
extern UartIt uartIt3;
