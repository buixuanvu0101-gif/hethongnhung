#ifndef LED_H
#define LED_H

#include "stm32f10x.h"

typedef struct {
    GPIO_TypeDef *port;     /* GPIOA / GPIOB / GPIOC */
    uint16_t      pin;      /* GPIO_Pin_x            */
    float         freq_hz;  /* tan so nhap nhay (Hz) */
} led_cfg_t;

/* Bat clock + cau hinh chan LED output push-pull */
void led_init(GPIO_TypeDef *port, uint16_t pin);

/* Ham dieu khien nhay LED (dau vao: chan LED, tan so nhap nhay).
   Thuc hien 1 chu ky sang -> tat, goi lap lai trong vong lap cua task. */
void led_blink(GPIO_TypeDef *port, uint16_t pin, float freq_hz);

#endif
