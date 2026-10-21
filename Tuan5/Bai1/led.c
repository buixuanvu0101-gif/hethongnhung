#include "led.h"

#define PWM_PERIOD   1000   /* 1MHz / 1000 = 1kHz */

void led_pwm_init(void)
{
    GPIO_InitTypeDef        gpio;
    TIM_TimeBaseInitTypeDef tb;
    TIM_OCInitTypeDef       oc;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    gpio.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    gpio.GPIO_Mode  = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    /* Clock TIM2 = 72MHz -> chia 72 = 1MHz -> dem 1000 = 1kHz */
    TIM_TimeBaseStructInit(&tb);
    tb.TIM_Prescaler = 72 - 1;
    tb.TIM_Period    = PWM_PERIOD - 1;
    TIM_TimeBaseInit(TIM2, &tb);

    TIM_OCStructInit(&oc);
    oc.TIM_OCMode      = TIM_OCMode_PWM1;
    oc.TIM_OutputState = TIM_OutputState_Enable;
    oc.TIM_OCPolarity  = TIM_OCPolarity_High;
    oc.TIM_Pulse       = 0;

    TIM_OC1Init(TIM2, &oc);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC2Init(TIM2, &oc);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3Init(TIM2, &oc);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

void led_set_level(uint8_t index, uint8_t level)
{
    uint16_t duty;

    if (level > LED_MAX_LEVEL) level = LED_MAX_LEVEL;
    duty = (uint16_t)level * (PWM_PERIOD / LED_MAX_LEVEL);

    switch (index) {
    case 0: TIM_SetCompare1(TIM2, duty); break;
    case 1: TIM_SetCompare2(TIM2, duty); break;
    case 2: TIM_SetCompare3(TIM2, duty); break;
    default: break;
    }
}