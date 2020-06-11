#pragma once

#include <stdint.h>

#include "stm32f1xx_hal.h"
#include "CubeMX/Inc/usart.h"
//#include "Class/RingBuffer.h"

#include "uart.h"

//class Lora
//{
//	RingBuffer<uint8_t, 1024> gTxBuf;
//public:
//	void txEmpty()
//	{
//		_sendTxBuf();
//	}

//	void send(const char val)
//	{
//		gTxBuf.push(val);
//		_sendTxBuf();
//	}

//	void send(const char* buf, int size)
//	{
//		for(int i = 0; i < size; ++i)
//		{
//			gTxBuf.push(buf[i]);
//		}

//		_sendTxBuf();
//	}

//private:
//	void _sendTxBuf()
//	{
//		if(huart2.gState == HAL_UART_STATE_READY)
//		{
//			if(gTxBuf.isEmpty() == false)
//				uart2_send(gTxBuf.pop());
//		}
//	}

//};
