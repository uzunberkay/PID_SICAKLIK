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


#include "motor_driver.h"



void appLevelComProcessor(motor_driver_t *driver)
{

	if(strstr((char *)driver->com.rxBuffer,"AT+SETPOINT=") != 0){
		driver->pid.temperature = atof((char *)driver->com.rxBuffer+12);

		strcpy((char*)driver->com.txBuffer,"\r\nOK\r\n");
		driver->com.txIndex = strlen((char *)driver->com.txBuffer);
		HAL_UART_Transmit_IT(&huart2, driver->com.txBuffer, driver->com.txIndex);

	}
	else if(strstr((char * )driver->com.rxBuffer,"AT+PIPDRM")!= 0)
	{
		char *ptr = (char *)driver->com.rxBuffer ;
		float tempKp = 0 ;
		float tempKi = 0 ;
		float tempKd = 0 ;
		float tempTs = 0 ;

		ptr+=(strlen("AT+PIPDRM"));
		tempKp = atof(ptr);

		ptr = strstr(ptr,",")+1;
		tempKi = atof(ptr);

		ptr = strstr(ptr,",")+1;
		tempKd = atof(ptr);

		ptr = strstr(ptr,",")+1;
		tempTs = atof(ptr);

		pidChangeCoefficients(&driver->pid, tempKp, tempKi, tempKd, tempTs);

		strcpy((char*)driver->com.txBuffer,"\r\nOK\r\n");
		driver->com.txIndex = strlen((char *)driver->com.txBuffer);
		HAL_UART_Transmit_IT(&huart2, driver->com.txBuffer, driver->com.txIndex);

	}
	else if(strstr((char *)driver->com.rxBuffer,"AT+TEMPERATURE?") != 0 ){
		sprintf((char *)driver->com.txBuffer , "+TEMPERATURE = %2.f\r\n" , driver->bmp_180.temperature);
		driver->com.txIndex =  strlen((char *)driver->com.txBuffer);

	}
	else if(strstr((char *)driver->com.rxBuffer,"AT+ALTITUDE") != 0)
	{
		sprintf((char *)driver->com.txBuffer,"Yukseklik : %2.f ' dir \r\n",driver->bmp_180.altitude);
		driver->com.txIndex = strlen((char *)driver->com.txBuffer);
		HAL_UART_Transmit_IT(&huart2, driver->com.txBuffer, driver->com.txIndex);
	}

	comRxBufferProcessed(&driver->com);



}

void powerDriver (motor_driver_t *driver,float input)
{
	if(!driver->pid.targetState){
		if(input>=100)
			input = 99;
		else if(input <= 1)
			input = 0 ;
		else if(input <= 25 && input > 1)
			input = 25;
		__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,(uint32_t )input);

	}
	if((driver->bmp_180.temperature >= driver->pid.temperature*(1.025)) && driver->pid.targetState)
	{
		driver->pid.targetState = FALSE ;

	}
	if(driver->bmp_180.temperature <= driver->pid.temperature)
	{
		driver->pid.targetState = TRUE ;
		driver->pid.output = 0 ;

	}
}
