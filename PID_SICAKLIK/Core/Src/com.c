/*______________________________________________
* Berkay Uzun									|
*												|
*												|
* 												|
* 												|
* Date         : 01.04.2024						|
* By           : Berkay UZUN					|
* e-mail       : berkyuzun@gmail.com			|
* address	   : https://github.com/uzunberkay	|
* ______________________________________________|
*/

#include "com.h"

void comInit ( com_t* driver)
{
	uint8_t i = 0;

	driver->rxIndex = 0;
	driver->txIndex = 0;

	driver->rxReady = FALSE;

	for(i=0;i<BUFFERSIZE;++i)
	{
		driver->rxBuffer[i] = 0;
		driver->txBuffer[i] = 0;

	}
}

void comGetData(com_t* driver,uint8_t data)
{
	if(driver->rxReady == FALSE && driver->rxIndex < BUFFERSIZE)
	{
		driver->rxBuffer[driver->rxIndex++] = data & 0xFF;


		if(driver->rxBuffer[driver->rxIndex-2] == '\r' && driver->rxBuffer[driver->rxIndex-1] == '\n')
		{
			driver->rxReady = TRUE;
		}
	}

}
void sendData(com_t *driver,char *message)
{
	if(strlen(message)< BUFFERSIZE)
	{
		strcpy((char *)driver->txBuffer,message);
		driver->txIndex=strlen((char *) driver->txBuffer);
	}

}



uint8_t comIsRxBufferReady (com_t* driver){ return (driver->rxReady); }

void comRxBufferProcessed (com_t* driver)
{
	driver->rxReady = FALSE;
	driver->rxIndex = 0;

}

uint8_t comGetTxBufferIndex(com_t* driver)
{
	return (driver->txIndex);
}
void comSetTxBufferIndex(com_t* driver, uint32_t index)
{
	driver->txIndex = index;
}
void comRxBufferReset(com_t *driver)
{
	for(int i = 0 ; i<BUFFERSIZE ; i++ )
	{
		driver->rxBuffer[i] = 0 ;
	}
}

