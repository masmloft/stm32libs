#include "UartIt.h"

//#include "CubeMX/Inc/usart.h"

UartIt::UartData UartIt::_allUartData[UartIt::UART_COUNT] = {};

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	UartIt::interruptErr(huart);
//	if(huart == &huart3)
//	{
//		HAL_UART_Transmit(&huart2, (uint8_t*)"\r\nERR\r\n", 7, 1000);
////		UartIt::getUartIt(huart)->startRxIt();
//	}
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	UartIt::interruptTx(huart);
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UartIt::interruptRx(huart);
}


UartIt* UartIt::getUartIt(UART_HandleTypeDef* huart)
{
	for(UartData& uartData : _allUartData)
		if(uartData.huart == huart)
			return uartData.uartIt;
	return nullptr;
}

void UartIt::interruptErr(UART_HandleTypeDef* huart)
{
	if(UartIt* uartIt = getUartIt(huart))
	{
	}
}

void UartIt::interruptTx(UART_HandleTypeDef* huart)
{
	if(UartIt* uartIt = getUartIt(huart))
	{
		uartIt->startTxIt();
	}
}

void UartIt::interruptRx(UART_HandleTypeDef* huart)
{
	if(UartIt* uartIt = getUartIt(huart))
	{
		uartIt->_rxBuf.push(uartIt->_uartData->uart_rxBuf[0]);
		uartIt->startRxIt();
	}
}


UartIt::UartIt(UART_HandleTypeDef* huart)
{
	if(huart == nullptr)
		return;
	for(UartData& uartData : _allUartData)
	{
		if(uartData.huart == huart)
			return;
//			_uartData = &uartData;
	}

	if(_uartData == nullptr)
	{
		for(UartData& uartData : _allUartData)
		{
			if(uartData.huart == nullptr)
			{
				uartData.huart = huart;
				uartData.uartIt = this;
				_uartData = &uartData;
				break;
			}
		}
	}
}

UartIt::~UartIt()
{
	_uartData = {};
}

bool UartIt::startRxIt()
{
	HAL_StatusTypeDef res = HAL_UART_Receive_IT(_uartData->huart, _uartData->uart_rxBuf, sizeof(_uartData->uart_rxBuf));
	return (res == HAL_OK);
}

bool UartIt::startTxIt()
{
	if(_uartData->huart->gState == HAL_UART_STATE_READY)
	{
		if(_txBuf.isEmpty() == false)
		{
			_uartData->uart_txBuf[0] = _txBuf.pop();
			HAL_StatusTypeDef res = HAL_UART_Transmit_IT(_uartData->huart, _uartData->uart_txBuf, sizeof(_uartData->uart_txBuf));
			return (res == HAL_OK);
		}
	}
	return false;
}

//void UartIt::write(char val)
//{
//	if(huart2.gState == HAL_UART_STATE_READY)
//	{
//		uart2_txBuf[0] = val;
//		HAL_UART_Transmit_IT(&huart2, uart2_txBuf, sizeof(uart2_txBuf));
//	}
//	else
//	{
//		UartIt2::txBuf.push(val);
//	}
//}

void UartIt::write(const char* buf, int size)
{
	_txBuf.append((const uint8_t*)buf, size);
	startTxIt();
}
