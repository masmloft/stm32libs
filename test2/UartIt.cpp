#include "UartIt.h"

#include "stm32f1xx_hal.h"

#include "usart.h"

UartIt uartIt2(&huart2);
UartIt uartIt3(&huart3);

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart == uartIt2.huart())
		uartIt2.interruptErr();
	if(huart == uartIt3.huart())
		uartIt3.interruptErr();
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uartIt2.huart())
		uartIt2.interruptTx();
	if(huart == uartIt3.huart())
		uartIt3.interruptTx();
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uartIt2.huart())
		uartIt2.interruptRx();
	if(huart == uartIt3.huart())
		uartIt3.interruptRx();
}

UartIt::UartIt(void* huart)
	: _huart(huart)
{
}

UartIt::~UartIt()
{
}

void UartIt::interruptErr()
{
}

void UartIt::interruptTx()
{
	startTx();
}

void UartIt::interruptRx()
{
	_rxBuf.push(_uart_rxBuf[0]);
	startRx();
}

bool UartIt::startRx()
{
	HAL_StatusTypeDef res = HAL_UART_Receive_IT((UART_HandleTypeDef*)_huart, _uart_rxBuf, sizeof(_uart_rxBuf));
	return (res == HAL_OK);
}

bool UartIt::startTx()
{
	if(((UART_HandleTypeDef*)_huart)->gState != HAL_UART_STATE_READY)
		return false;

	if(_txBuf.isEmpty() == false)
	{
		_uart_txBuf[0] = _txBuf.pop();
		HAL_StatusTypeDef res = HAL_UART_Transmit_IT((UART_HandleTypeDef*)_huart, _uart_txBuf, sizeof(_uart_txBuf));
		return (res == HAL_OK);
	}
	return true;
}

void UartIt::write(const char* buf, int size)
{
	_txBuf.append((const uint8_t*)buf, size);
	startTx();
}
