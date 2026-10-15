#ifndef LED_H
#define LED_H
#include "stm32f10x.h"
typedef struct {
    GPIO_TypeDef *port;
    uint16_t      pin;
    float         freq_hz;
} led_cfg_t;
void led_init(GPIO_TypeDef *port, uint16_t pin);
void led_blink(GPIO_TypeDef *port, uint16_t pin, float freq_hz);
#endif
