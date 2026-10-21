#ifndef LED_H
#define LED_H

#include "stm32f10x.h"

#define LED_COUNT      3
#define LED_MAX_LEVEL  10   /* muc sang 0..10 tuong ung 0%..100% */

/* TIM2 PWM 1kHz: CH1=PA0, CH2=PA1, CH3=PA2 */
void led_pwm_init(void);

/* Dat do sang LED (index 0..2, level 0..LED_MAX_LEVEL) */
void led_set_level(uint8_t index, uint8_t level);

#endif
