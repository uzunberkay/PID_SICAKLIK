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

#ifndef INC_BMP180_H_
#define INC_BMP180_H_


#include "stm32f4xx_hal.h"
#include "math.h"



extern I2C_HandleTypeDef hi2c1;

#define BMP180_I2C &hi2c1
#define BMP180_ADDRESS 0xEE
#define atmPress 101325


typedef struct
{
	short AC1;
	short AC2;
	short AC3;
	unsigned short AC4;
	unsigned short AC5;
	unsigned short AC6;
	short B1;
	short B2;
	short MB;
	short MC;
	short MD;
	long UT;
	short oss;
	long UP;
	long X1;
	long X2;
	long X3;
	long B3;
	long B5;
	unsigned long B4 ;
	long B6 ;
	unsigned long B7 ;
	long Press ;
	float Temp ;
	float yukseklik;

}bmp_180_t;

typedef struct{
	float temperature ;
	float altitude ;
}bmp_180_data_t;



/* ----------------------> FONKSIYON PROTOTIPLERI <----------------------------- */
void read_calliberation_data (bmp_180_t *bmp_180);
void BMP180_Start (bmp_180_t *bmp_180);
uint16_t Get_UTemp (void);
float BMP180_GetTemp (bmp_180_t *bmp_180);
uint32_t Get_UPress (int oss);
float BMP180_GetPress (bmp_180_t *bmp_180 , int oss);
float BMP180_GetAlt (bmp_180_t *bmp_180,int oss);
void read_BMP180(bmp_180_t *bmp_180, bmp_180_data_t *data, uint8_t oss);



#endif /* INC_BMP180_H_ */
