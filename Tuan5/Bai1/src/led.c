#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

static void led_enable_clock(GPIO_TypeDef *port)
{
    if (port == GPIOA)      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if (port == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if (port == GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}

void led_init(GPIO_TypeDef *port, uint16_t pin)
{
    GPIO_InitTypeDef gpio;
    led_enable_clock(port);
    gpio.GPIO_Pin   = pin;
    gpio.GPIO_Mode  = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(port, &gpio);
    GPIO_SetBits(port, pin);
}

/* 1 chu ky nhay: nua chu ky = 500/f (ms). 0.1Hz->5000ms, 1Hz->500ms, 10Hz->50ms */
void led_blink(GPIO_TypeDef *port, uint16_t pin, float freq_hz)
{
    uint32_t half_ms = (uint32_t)(500.0f / freq_hz);
    TickType_t half_ticks = pdMS_TO_TICKS(half_ms);

    GPIO_ResetBits(port, pin);
    vTaskDelay(half_ticks);
    GPIO_SetBits(port, pin);
    vTaskDelay(half_ticks);
}
