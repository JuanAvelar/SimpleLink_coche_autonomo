/*
 * Copyright (c) 2015-2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== pwmled2.c ========
 */
/* For usleep() */
#include <unistd.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/PWM.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/GPIO.h>

/* Example/Board Header files */
#include "Board.h"
#include "I2C_Slave.h"




/*
 *  ======== mainThread ========
 *  Task periodically increments the PWM duty for the on board LED.
 *
 */
/*  I2C data received*/
uint32_t data = (uint32_t)256/2;
/* Period and duty in microseconds*/
uint16_t   pwmPeriod = 3000;
uint16_t   duty = 0;
uint16_t   dutyInc = 100;
PWM_Handle pwm1 = NULL;//pointer to pwm config
PWM_Handle pwm2 = NULL;
PWM_Handle pwm3 = NULL;
uint16_t    duty3 = 0;
uint8_t     bandera = 0;

/* Callback used for updating output variables. */
void timerCallback(Timer_Handle myHandle)
{
            data = (int)I2C_Slave_Get_Data();
            //P21
            if(pwm1 != NULL){
                PWM_setDuty(pwm1, duty);
            }
            //P64
            if(pwm2 != NULL){
                PWM_setDuty(pwm2, duty);
            }
            //P01
            if(pwm3 != NULL){
                PWM_setDuty(pwm3, (1300 + (uint16_t)((float)data*(float)2.6)));//este pwm debe ir de 100 a 200
            }
            //PWM_setDuty(pwm3, 1500);
           //PWM_setDuty(pwm3, (1300 + (uint16_t)((float)duty*(float)0.22)));//30 degrees//duty va de 3000 a 0
            duty = (duty + dutyInc);

            if (duty == pwmPeriod || (!duty)) {
                dutyInc = - dutyInc;
            }
            if(bandera){
                if(pwm2 == NULL){
                    PWM_Params params3;
                    PWM_Params_init(&params3);//give params a default state
                    params3.dutyUnits = PWM_DUTY_US;
                    params3.dutyValue = 2900;
                    params3.periodUnits = PWM_PERIOD_US;
                    params3.periodValue = 3000;
                    pwm2 = PWM_open(Board_PWM1, &params3);
                    if (pwm2 == NULL) {
                    // Board_PWM1 did not open
                        while (1);
                    }
                    else
                        PWM_start(pwm2);
                }
                bandera = 0;
            }

}

/*PIN description
 * I2C_SCL  P03
 * I2C_SDA  P06
 * PWM1     P21
 * PWM2     P64
 * PWM3     P01
 *
 * */

/*
 *  ======== gpioButtonFxn0 ========
 *  Callback function for the GPIO interrupt on Board_GPIO_BUTTON0.
 */
void gpioButtonFxn0(uint_least8_t index)
{
    bandera = 1;
}

void *mainThread(void *arg0)
{
    Timer_Handle timer0;
    Timer_Params params_time;
    Timer_init();
    Timer_Params_init(&params_time);
    params_time.period = 10000;
    params_time.periodUnits = Timer_PERIOD_US;
    params_time.timerMode = Timer_CONTINUOUS_CALLBACK;
    params_time.timerCallback = timerCallback;

    timer0 = Timer_open(Board_TIMER2, &params_time);

    if (timer0 == NULL) {
        // Failed to initialize timer
        while (1) {}
    }
    /* Sleep time in microseconds*/
    uint32_t   time = 10000;
    PWM_Params params;

    /* Call driver init functions. */

    I2C_Slave_init(SimpleLink_Slave_Address);

    PWM_init();//just turns a flag which says it is initialized
    PWM_Params_init(&params);//give params a default state
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = 1000;
    params.periodUnits = PWM_PERIOD_US;
    params.periodValue = pwmPeriod;
    pwm1 = PWM_open(Board_PWM0, &params);//returns the variable of type PWM_Config_//
    if (pwm1 == NULL) {
        // Board_PWM0 did not open
        while (1);
    }
    PWM_start(pwm1);

    PWM_Params params2;
    PWM_Params_init(&params2);//give params a default state
    params2.dutyUnits = PWM_DUTY_US;
    params2.dutyValue = (uint32_t)1500;
    params2.periodUnits = PWM_PERIOD_US;
    params2.periodValue = 20000;
    pwm3 = PWM_open(Board_PWM2, &params2);//returns the variable of type PWM_Config_//
    if (pwm3 == NULL) {
        // Board_PWM0 did not open
        while (1);
    }
    PWM_start(pwm3);
/*
    PWM_Params params3;
    PWM_Params_init(&params3);//give params a default state
    params3.dutyUnits = PWM_DUTY_US;
    params3.dutyValue = 1000;
    params3.periodUnits = PWM_PERIOD_US;
    params3.periodValue = 3000;
    pwm2 = PWM_open(Board_PWM1, &params3);
    if (pwm2 == NULL) {
    // Board_PWM1 did not open
        while (1);
    }
    else
        PWM_start(pwm2);
*/
    GPIO_init();//Sets all GPIO into gpio mode which is not wanted. And it also works with a semaphore so it doesn´t matter if we are using NoRtos
    GPIO_setConfig(Board_GPIO_BUTTON0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    // install Button callback
    GPIO_setCallback(Board_GPIO_BUTTON0, gpioButtonFxn0);
    // Enable interrupts
    GPIO_enableInt(Board_GPIO_BUTTON0);

    //Used timers
    //Timer0A
    //Timer1A
    //Timer2B
    //PWM_setDuty(pwm1,2500);
    //PWM_setDuty(pwm2,2500);

    if (Timer_start(timer0) == Timer_STATUS_ERROR) {
        // Failed to start timer
        while (1) {}
    }
    //PWM_setDuty(pwm1,400);


    /* Loop forever incrementing the PWM duty */
    while (1) {
        usleep(time);//implement a timer instead of usleep
        /*data = (int)I2C_Slave_Get_Data();
        if(pwm3 != NULL)
                        //PWM_setDuty(pwm3, 1500);
                        //PWM_setDuty(pwm3, (1300 + (uint16_t)((float)duty*(float)0.22)));//30 degrees//duty va de 3000 a 0
                        PWM_setDuty(pwm3, (1300 + (uint16_t)((float)data*(float)2.6)));*/
    }
}
/*
 * This callback is called every 1,000,000 microseconds, or 1 second. Because
 * the LED is toggled each time this function is called, the LED will blink at
 * a rate of once every 2 seconds.
 * */

