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

#ifndef INC_PID_H_
#define INC_PID_H_

#include "stdbool.h"

typedef struct
{
    float error;
    float lastError;

    float output;

    float kp;
    float ki;
    float kd;

    float time;

    float pMax;
    float pMin;

    float iMax;
    float iMin;

    float dMax;
    float dMin;

    float partP;
    float partI;
    float partD;

    float pidMax;
    float pidMin;

    float temperature;

    bool targetState;
} pidc_t;

/* ----------------------> FONKSIYON PROTOTIPLERI <----------------------------- */

void pidInit ( pidc_t* driver, float kp, float ki, float kd, float time, float pPartMaxLimit, float pPartMinLimit, float iPartMaxLimit, float iPartMinLimit,
                float dPartMaxLimit, float dPartMinLimit, float pidOutputMaxLimit, float pidOutputMinLimit ,float temperature);

void pidChangeCoefficients ( pidc_t* driver, float kp, float ki, float kd, float ts );

void pidChangeLimits ( pidc_t* driver, float pPartMaxLimit, float pPartMinLimit, float iPartMaxLimit, float iPartMinLimit,
                        float dPartMaxLimit, float dPartMinLimit, float pidOutputMaxLimit, float pidOutputMinLimit );

void pidControl ( pidc_t* driver, float input );

float pidGetOutput ( pidc_t* driver );






#endif /* INC_PID_H_ */
