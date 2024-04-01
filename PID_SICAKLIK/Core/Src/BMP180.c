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
#include "BMP180.h"

void read_calliberation_data (bmp_180_t *bmp_180)
{
	uint8_t Callib_Data[22] = {0};
	uint16_t Callib_Start = 0xAA;
	HAL_I2C_Mem_Read(BMP180_I2C, BMP180_ADDRESS, Callib_Start, 1, Callib_Data,22, HAL_MAX_DELAY);
	bmp_180->AC1  = ((Callib_Data[0]  << 8)  | Callib_Data[1]);
	bmp_180->AC2  = ((Callib_Data[2]  << 8)  | Callib_Data[3]);
	bmp_180->AC3  = ((Callib_Data[4]  << 8)  | Callib_Data[5]);
	bmp_180->AC4  = ((Callib_Data[6]  << 8)  | Callib_Data[7]);
	bmp_180->AC5  = ((Callib_Data[8]  << 8)  | Callib_Data[9]);
	bmp_180->AC6  = ((Callib_Data[10] << 8)  | Callib_Data[11]);
	bmp_180->B1   = ((Callib_Data[12] << 8)  | Callib_Data[13]);
	bmp_180->B2   = ((Callib_Data[14] << 8)  | Callib_Data[15]);
	bmp_180->MB   = ((Callib_Data[16] << 8)  | Callib_Data[17]);
	bmp_180->MC   = ((Callib_Data[18] << 8)  | Callib_Data[19]);
	bmp_180->MD   = ((Callib_Data[20] << 8)  | Callib_Data[21]);

	if( (bmp_180->AC1 == 0x0000  || bmp_180->AC1 == 0xFFFF) ||
		(bmp_180->AC2 == 0x0000  || bmp_180->AC2 == 0xFFFF) ||
		(bmp_180->AC3 == 0x0000  || bmp_180->AC3 == 0xFFFF) ||
		(bmp_180->AC4 == 0x0000  || bmp_180->AC4 == 0xFFFF) ||
		(bmp_180->AC5 == 0x0000  || bmp_180->AC5 == 0xFFFF) ||
		(bmp_180->AC6 == 0x0000  || bmp_180->AC6 == 0xFFFF) ||
		(bmp_180->B1  == 0x0000  || bmp_180->B1  == 0xFFFF) ||
		(bmp_180->B2  == 0x0000  || bmp_180->B2  == 0xFFFF) ||
		(bmp_180->MB  == 0x0000  || bmp_180->MB  == 0xFFFF) ||
		(bmp_180->MC  == 0x0000  || bmp_180->MC  == 0xFFFF) ||
		(bmp_180->MD  == 0x0000  || bmp_180->MD  == 0xFFFF))
	{
	    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
	    while(1);
	}

}
void BMP180_Start (bmp_180_t *bmp_180)
{
	read_calliberation_data(bmp_180);
}
uint16_t Get_UTemp (void)
{
	uint8_t datatowrite = 0x2E;
	uint8_t Temp_RAW[2] = {0};
	HAL_I2C_Mem_Write(BMP180_I2C, BMP180_ADDRESS, 0xF4, 1, &datatowrite, 1, 1000);
	HAL_Delay (5);  // wait 4.5 ms
	HAL_I2C_Mem_Read(BMP180_I2C, BMP180_ADDRESS, 0xF6, 1, Temp_RAW, 2, 1000);
	return ((Temp_RAW[0]<<8) + Temp_RAW[1]);
}
float BMP180_GetTemp (bmp_180_t *bmp_180)
{
	bmp_180->UT = Get_UTemp();
	bmp_180->X1 = ((bmp_180->UT- bmp_180->AC6) * (bmp_180->AC5/(pow(2,15))));
	bmp_180->X2 = ((bmp_180->MC *(pow(2,11))) / (bmp_180->X1 + bmp_180->MD));
	bmp_180->B5 = bmp_180->X1 + bmp_180->X2;
	bmp_180->Temp = (bmp_180->B5 + 8)/(pow(2,4));

	return bmp_180->Temp/10;
}

uint32_t Get_UPress (int oss)
{
	uint8_t datatowrite = 0x34+(oss<<6);
	uint8_t Press_RAW[3] = {0};
	HAL_I2C_Mem_Write(BMP180_I2C, BMP180_ADDRESS, 0xF4, 1, &datatowrite, 1, 1000);
	switch (oss)
	{
		case (0):
			HAL_Delay (5);
			break;
		case (1):
			HAL_Delay (8);
			break;
		case (2):
			HAL_Delay (14);
			break;
		case (3):
			HAL_Delay (26);
			break;
	}
	HAL_I2C_Mem_Read(BMP180_I2C, BMP180_ADDRESS, 0xF6, 1, Press_RAW, 3, 1000);
	return (((Press_RAW[0]<<16)+(Press_RAW[1]<<8)+Press_RAW[2]) >> (8-oss));
}

float BMP180_GetPress (bmp_180_t *bmp_180 , int oss)
{
	bmp_180->UP = Get_UPress(oss);
	bmp_180->X1 = ((bmp_180->UT - bmp_180->AC6 )* (bmp_180->AC5 / (pow(2,15))));
	bmp_180->X2 = ((bmp_180->MC *(pow(2,11))) / (bmp_180->X1 + bmp_180->MD));
	bmp_180->B5 = bmp_180->X1 + bmp_180->X2;
	bmp_180->B6 = bmp_180->B5 - 4000;
	bmp_180->X1 = (bmp_180->B2 * (bmp_180->B6*bmp_180->B6/(pow(2,12))))/(pow(2,11));
	bmp_180->X2 = bmp_180->AC2 * bmp_180->B6 /(pow(2,11));
	bmp_180->X3 = bmp_180->X1 + bmp_180->X2;
	bmp_180->B3 = (((bmp_180->AC1*4 + bmp_180->X3)<<oss)+2)/4;
	bmp_180->X1 = bmp_180->AC3*bmp_180->B6/pow(2,13);
	bmp_180->X2 = (bmp_180->B1 * (bmp_180->B6*bmp_180->B6/(pow(2,12))))/(pow(2,16));
	bmp_180->X3 = ((bmp_180->X1+bmp_180->X2)+2)/pow(2,2);
	bmp_180->B4 = bmp_180->AC4*(unsigned long)(bmp_180->X3+32768)/(pow(2,15));
	bmp_180->B7 = ((unsigned long)bmp_180->UP-bmp_180->B3)*(50000>>oss);
	if (bmp_180->B7<0x80000000) bmp_180->Press = (bmp_180->B7*2)/bmp_180->B4;
	else bmp_180->Press = (bmp_180->B7/bmp_180->B4)*2;
	bmp_180->X1 = (bmp_180->Press/(pow(2,8)))*(bmp_180->Press/(pow(2,8)));
	bmp_180->X1 = (bmp_180->X1*3038)/(pow(2,16));
	bmp_180->X2 = (-7357*bmp_180->Press)/(pow(2,16));
	bmp_180->Press = bmp_180->Press + (bmp_180->X1+bmp_180->X2+3791)/(pow(2,4));

	return bmp_180->Press;
}

float BMP180_GetAlt (bmp_180_t *bmp_180,int oss)
{
	BMP180_GetPress(bmp_180, oss);
	return 44330*(1-(pow((bmp_180->Press/(float)atmPress), 0.19029495718)));
}

void read_BMP180(bmp_180_t *bmp_180, bmp_180_data_t *data, uint8_t oss)
{

	data->altitude = BMP180_GetAlt(bmp_180, oss);
	data->temperature = BMP180_GetTemp(bmp_180);

}
