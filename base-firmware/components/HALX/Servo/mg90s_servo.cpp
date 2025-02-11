/*MIT License

Copyright (c) 2023 limitless Aeronautics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include "mg90s_servo.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <sdkconfig.h>
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define ServoMsMin 0.06
#define ServoMsMax 2.1
#define ServoMsAvg ((ServoMsMax-ServoMsMin)/2.0)

uint8_t SERVO_POS_1 = 0;

uint8_t SERVO_POS_2 = 0;

uint8_t SERVO_POS_3 = 0;

uint8_t SERVO_POS_4 = 0;

//____________________________________________________________
/* Utillity subroutine -> linear interpolation method
===========================================================================
===========================================================================
*/
double WingTranslate::linearInterpolate(double input, double input_start, double input_end, 
                                        double output_start, double output_end) {
    // Map input range to output range
    double slope = (output_end - output_start) / (input_end - input_start);
    double output = output_start + slope * (input - input_start);
    return output;
}

//____________________________________________________________
/* Initializes MG90S Servo using LEDC
===========================================================================
|    motor selection   The identification of the motor intended to be interfaced
|    motor selection   The identification of the motor intended to be interfaced
===========================================================================
*/
void WingTranslate::actuateServo(double targetPos, uint8_t pin) {
    ledc_timer_config_t ledc_timer;
    ledc_timer.speed_mode       = LEDC_LOW_SPEED_MODE;
    ledc_timer.timer_num        = LEDC_TIMER_0;
    ledc_timer.duty_resolution  = LEDC_TIMER_13_BIT;
    ledc_timer.freq_hz          = 50;
    ledc_timer.clk_cfg          = LEDC_AUTO_CLK;

    ledc_timer_config(&ledc_timer);
    ledc_channel_config_t ledc_channel;
    ledc_channel.speed_mode     = LEDC_LOW_SPEED_MODE;
    ledc_channel.channel        = LEDC_CHANNEL_0;
    ledc_channel.timer_sel      = LEDC_TIMER_0;
    ledc_channel.intr_type      = LEDC_INTR_DISABLE;
    ledc_channel.gpio_num       = pin;
    ledc_channel.duty           = 0;
    ledc_channel.hpoint         = 0;

    ledc_channel_config(&ledc_channel);  
    int duty = (int)(100.0 * (targetPos / 20.0) * 81.91);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);  
    vTaskDelay( 2000 / portTICK_PERIOD_MS ); 
    ledc_stop(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
}

//____________________________________________________________
/* Main API routine
===========================================================================
|    motor target angle   The motor rotation angle bounded between 0 deg to 90 deg
|    motor selection   The identification of the motor intended to be interfaced
===========================================================================
*/
uint8_t WingTranslate::servo_control(double target, uint8_t pin){
    //Map target in the 0 - 360 range to ServoMsMin and ServoMsMax
    double mapped_target = linearInterpolate(target, 0, 360, ServoMsMin, ServoMsMax);
    actuateServo(mapped_target, pin);
    UPDATE_SERVO_POS(pin,target);
    return mapped_target;
}

//____________________________________________________________
/* Utillity subroutine -> retrieve current motor position 
===========================================================================
|    motor selection   The identification of the motor intended to be interfaced
===========================================================================
*/
uint8_t WingTranslate::GET_SERVO_POS(uint8_t pin)
{
     switch(pin) {
        case 0:
            return SERVO_POS_1;
        break;
        case 1:
            return SERVO_POS_2;
        break;
        case 2:
            return SERVO_POS_3;
        break;
        case 3:
            return SERVO_POS_4;
        break;
    }
    return 0;
}

//____________________________________________________________
/* Utillity subroutine -> update current motor position after movement change
===========================================================================
|    motor selection   The identification of the motor intended to be interfaced
|    updated servo position      New motor position to be updated
===========================================================================
*/
void WingTranslate::UPDATE_SERVO_POS(uint8_t pin, uint8_t updatedValue){
    switch(pin) {
        case 0:
            SERVO_POS_1 = updatedValue;
        break;
        case 1:
            SERVO_POS_2 = updatedValue;
        break;
        case 2:
            SERVO_POS_3 = updatedValue;
        break;
        case 3:
            SERVO_POS_4 = updatedValue;
        break;
    }
}