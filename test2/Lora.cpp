#include "Lora.h"

#include "stm32f1xx_hal.h"
#include "CubeMX/Inc/usart.h"
#include "Class/RingBuffer.h"

static uint8_t loraTxBuf[1];

static RingBuffer<uint8_t, 1024> gTxBuf;

static void _sendTxBuf()
{
	if(huart2.gState == HAL_UART_STATE_READY)
	{
		if(gTxBuf.isEmpty() == false)
		{
			loraTxBuf[0] = gTxBuf.pop();
			HAL_UART_Transmit_IT(&huart2, loraTxBuf, sizeof(loraTxBuf));
		}
	}
}

void lora_send(const char val)
{
	gTxBuf.push(val);
	_sendTxBuf();
}

void lora_send(const char* buf, int size)
{
	for(int i = 0; i < size; ++i)
	{
		gTxBuf.push(buf[i]);
	}

	_sendTxBuf();
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart2)
	{
		_sendTxBuf();
	}
}
