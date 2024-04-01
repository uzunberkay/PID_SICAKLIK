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

#ifndef INC_COM_H_
#define INC_COM_H_

#include "stdint.h"
#include "string.h"
#include "BMP180.h"





#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define BUFFERSIZE 32

typedef struct
{
	uint8_t rxIndex;
	uint8_t txIndex;

	uint8_t rxReady;

    uint8_t rxBuffer[BUFFERSIZE];
    uint8_t txBuffer[BUFFERSIZE];
}com_t;

/* ----------------------> FONKSIYON PROTOTIPLERI <----------------------------- */
void comInit ( com_t* driver);
void comGetData(com_t* driver,uint8_t data);
uint8_t comIsRxBufferReady (com_t* driver);
void comRxBufferProcessed (com_t* driver);
uint8_t comGetTxBufferIndex(com_t* driver);
void comSetTxBufferIndex(com_t* driver, uint32_t index);
void sendData(com_t *driver,char *message);
void comRxBufferReset(com_t *driver);




#endif /* INC_COM_H_ */
