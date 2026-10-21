#include "button.h"

static const uint16_t btn_pin[BTN_COUNT] = { GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14 };

void button_init(void)
{
    GPIO_InitTypeDef gpio;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    gpio.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
    gpio.GPIO_Mode  = GPIO_Mode_IPU;   /* keo len noi bo, nut noi xuong GND */
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &gpio);
}

uint8_t button_is_pressed(button_id_t id)
{
    return (GPIO_ReadInputDataBit(GPIOB, btn_pin[id]) == Bit_RESET) ? 1 : 0;
}
