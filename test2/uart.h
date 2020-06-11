#pragma once

#include "Class/RingBuf.h"


class UartIt2
{
public:
	static RingBuf<uint8_t, 1024> txBuf;
//	static RingBuf<uint8_t, 1024> rxBuf;
public:
	static void send(char val);
	static void send(const char* buf, int size);
};


class UartIt3
{
public:
	static RingBuf<uint8_t, 1024> rxBuf;

};
