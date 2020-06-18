#include <string>
//#include <sstream>
#include <string.h>

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_wwdg.h"

#include "CubeMX/Inc/usart.h"
//#include "CubeMX/Inc/usb_device.h"
//#include "CubeMX/Inc/usbd_cdc_if.h"

#include "STM32F103C8T6Lib/System/SystemClock.h"
#include "STM32F103C8T6Lib/Gpio/Gpio.h"
//#include "STM32F103C8T6Lib/Uart/Uart.h"
#include "STM32F103C8T6Lib/Uart/UartIt.h"

UartIt uartIt1(&huart1);
//UartIt uartIt2(&huart2);
UartIt uartIt3(&huart3);

#ifdef __cplusplus
extern "C" {
#endif

void _Error_Handler(char *file, int line)
{
	(void)file;
	(void)line;
	while(1)
	{
	}
}

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart == uartIt1.huart())
	{
		uartIt1.interruptErr();
		uartIt1.startRx();
	}
	//	if(huart == uartIt2.huart())
//		uartIt2.interruptErr();
	if(huart == uartIt3.huart())
	{
		uartIt3.interruptErr();
		uartIt3.startRx();
	}
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uartIt1.huart())
		uartIt1.interruptTx();
//	if(huart == uartIt2.huart())
//		uartIt2.interruptTx();
	if(huart == uartIt3.huart())
		uartIt3.interruptTx();
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uartIt1.huart())
		uartIt1.interruptRx();
//	if(huart == uartIt2.huart())
//		uartIt2.interruptRx();
	if(huart == uartIt3.huart())
		uartIt3.interruptRx();
}

#ifdef __cplusplus
}
#endif

void t1();


int main(void)
{
	HAL_Init();
	SystemClock::initClockExt72M();

	bool loraTxOn = true;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	MX_USART1_UART_Init();
	//MX_USART2_UART_Init();
	MX_USART3_UART_Init();
//	MX_USB_DEVICE_Init();

	GpioC13Out led(GpioSpeed::HIGH);
	GpioA8Out loraCtrl(GpioSpeed::LOW);

	led.setLow();

//	loraCtrl.setLow();

//	uint32_t lastTick = HAL_GetTick();

	uartIt1.startRx();
	uartIt3.startRx();

	char loraCfg[256 + 1] = {};
	{
		loraCtrl.setHi();
		HAL_Delay(1000);
		//get
		uartIt1.write("\xC1\xC1\xC1", 3);
//		//30db
//		uartIt1.write("\xC0\x00\x00\x1A\x17\x44", 6);
//		//27db
//		uartIt1.write("\xC0\x00\x00\x1A\x17\x45", 6);
//		//24db
//		uartIt1.write("\xC0\x00\x00\x1A\x17\x46", 6);
//		//21db
//		uartIt1.write("\xC0\x00\x00\x1A\x17\x47", 6);
		HAL_Delay(1000);

		char* pLoraCfg = loraCfg;
		strcpy(pLoraCfg, "$LORACFG,");
		pLoraCfg += strlen(pLoraCfg);
		while((pLoraCfg < loraCfg + sizeof(loraCfg) - 3) && (uartIt1.rxBuf().size() > 0))
		{
			const char b = uartIt1.rxBuf().pop();
			const char bh = (b >> 4) & 0x0f;
			const char bl = b & 0x0f;
			*pLoraCfg = ((bh < 10) ? '0' : ('A' - 10) ) + bh;
			pLoraCfg++;
			*pLoraCfg = ((bl < 10) ? '0' : ('A' - 10) ) + bl;
			pLoraCfg++;
		}
		*pLoraCfg = '\r';
		pLoraCfg++;
		*pLoraCfg = '\n';
		pLoraCfg++;
		*pLoraCfg = '\0';
		pLoraCfg++;
	}

	loraCtrl.setLow();
	HAL_Delay(500);

//	char nmeaBuf[256];
//	size_t nmeaBufPos = 0;

	std::string uart1RxBuf;
	uart1RxBuf.reserve(64);

	std::string uart3RxBuf;
	uart3RxBuf.reserve(256);

	uartIt1.write(loraCfg, strlen(loraCfg));

	for(uint32_t i = 0; true; ++i)
	{
		while(uartIt1.rxBuf().isEmpty() == false)
		{
			if(uart1RxBuf.size() >= 64)
				uart1RxBuf.clear();
			char b = uartIt1.rxBuf().pop();
			if(b == '$')
				uart1RxBuf.clear();
			if(b == '\r')
				b = '\n';
			if(b == '\n')
			{
				if(uart1RxBuf.empty() == false)
				{
					uartIt1.write(uart1RxBuf.data(), uart1RxBuf.size());
					uartIt1.write("\r\n", 2);
					if(uart1RxBuf == "$TXOFF")
						loraTxOn = false;
					if(uart1RxBuf == "$TXON")
						loraTxOn = true;
					uart1RxBuf.clear();
				}
			}
			else
			{
				uart1RxBuf.push_back(b);
			}
		}

		while(uartIt3.rxBuf().isEmpty() == false)
		{
			if(uart3RxBuf.size() >= uart3RxBuf.capacity())
				uart3RxBuf.clear();
			char b = uartIt3.rxBuf().pop();
			if(b == '$')
			{
				led.setLow();
				uart3RxBuf.clear();
			}
			uart3RxBuf.push_back(b);
			if(b == '\n')
			{
				led.setHi();
				if(loraTxOn)
				{
					//				if(memcmp(nmeaBuf, "$GPRMC,", 7) == 0)
					//					uartIt1.write(nmeaBuf, nmeaBufPos);
					if(memcmp(uart3RxBuf.data(), "$GPGGA,", 7) == 0)
						uartIt1.write(uart3RxBuf.data(), uart3RxBuf.size());
				}
				uart3RxBuf.clear();
			}
		}
	}
}

