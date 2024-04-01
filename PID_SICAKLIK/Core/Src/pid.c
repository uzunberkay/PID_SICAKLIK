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
#include "pid.h"


void pidInit ( pidc_t* driver, float kp, float ki, float kd, float time, float pPartMaxLimit, float pPartMinLimit, float iPartMaxLimit, float iPartMinLimit,
                float dPartMaxLimit, float dPartMinLimit, float pidOutputMaxLimit, float pidOutputMinLimit ,float temperature)
{
    driver->output = pidOutputMinLimit;


    driver->kp = kp;
    driver->ki = ki;
    driver->kd = kd;

    driver->time = time;


    driver->pMax = pPartMaxLimit;
    driver->pMin = pPartMinLimit;


    driver->iMax = iPartMaxLimit;
    driver->iMin = iPartMinLimit;


    driver->dMax = dPartMaxLimit;
    driver->dMin = dPartMinLimit;


    driver->pidMax = pidOutputMaxLimit;
    driver->pidMin = pidOutputMinLimit;



    driver->temperature  = temperature ;

    driver->targetState =  false;
}


void pidChangeCoefficients ( pidc_t* driver, float kp, float ki, float kd, float ts )
{

    driver->kp = kp;
    driver->ki = ki;
    driver->kd = kd;

    driver->time = ts;
}


void pidChangeLimits ( pidc_t* driver, float pPartMaxLimit, float pPartMinLimit, float iPartMaxLimit, float iPartMinLimit,
                        float dPartMaxLimit, float dPartMinLimit, float pidOutputMaxLimit, float pidOutputMinLimit )
{

    driver->pMax = pPartMaxLimit;
    driver->pMin = pPartMinLimit;


    driver->iMax = iPartMaxLimit;
    driver->iMin = iPartMinLimit;


    driver->dMax = dPartMaxLimit;
    driver->dMin = dPartMinLimit;


    driver->pidMax = pidOutputMaxLimit;
    driver->pidMin = pidOutputMinLimit;
}


void pidControl ( pidc_t* driver, float error )
{
    driver->error = error;


    driver->partP = driver->error;


    if ( driver->partP > driver->pMax )
    {
        driver->partP = driver->pMax;
    }
    else if ( driver->partP < driver->pMin )
    {
        driver->partP = driver->pMin;
    }
    else
    {

    }

    driver->partI += ( driver->error * driver->time   );


    if ( driver->partI > driver->iMax )
    {
        driver->partI = driver->iMax;
    }
    else if ( driver->partI < driver->iMin )
    {
        driver->partI = driver->iMin;
    }
    else
    {

    }

    driver->partD = ( ( driver->error - driver->lastError ) / driver->time );


    if ( driver->partD > driver->dMax )
    {
        driver->partD = driver->dMax;
    }
    else if ( driver->partD < driver->dMin )
    {
        driver->partD = driver->dMin;
    }
    else
    {

    }
    driver->output = ( driver->kp * driver->partP ) +
                        ( driver->ki * driver->partI ) +
                        ( driver->kd * driver->partD );


    if ( driver->output > driver->pidMax )
    {
        driver->output = driver->pidMax;
    }
    else if ( driver->output < driver->pidMin )
    {
        driver->output = driver->pidMin;
    }
    else
    {

    }

    driver->lastError = driver->error;
}


float pidGetOutput ( pidc_t* driver )
{
    return ( driver->output );
}

