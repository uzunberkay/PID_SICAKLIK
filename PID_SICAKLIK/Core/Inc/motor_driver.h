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

#ifndef INC_MOTOR_DRIVER_H_
#define INC_MOTOR_DRIVER_H_

#include "stm32f4xx.h"
#include "pid.h"
#include "com.h"
#include "i2c-lcd.h"
#include "BMP180.h"
#include "stdlib.h"
#include "stdio.h"


extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart2;

typedef struct
{
	pidc_t pid;
	com_t	com;
	bmp_180_data_t bmp_180;
	bmp_180_t bmp_180_cal;

}motor_driver_t;

/* ----------------------> FONKSIYON PROTOTIPLERI <----------------------------- */

void appLevelComProcessor(motor_driver_t *driver);
void powerDriver (motor_driver_t* driver,float input);


#endif /* INC_MOTOR_DRIVER_H_ */
