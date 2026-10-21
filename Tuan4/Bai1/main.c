#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"

/* 3 LED o 3 chan khac nhau, 3 tan so doc lap */
static const led_cfg_t led1 = { GPIOA, GPIO_Pin_0, 0.1f  };  /* PA0: 0.1 Hz */
static const led_cfg_t led2 = { GPIOA, GPIO_Pin_1, 1.0f  };  /* PA1: 1 Hz   */
static const led_cfg_t led3 = { GPIOA, GPIO_Pin_2, 10.0f };  /* PA2: 10 Hz  */

/* Moi task goi cung 1 ham led_blink() voi tham so khac nhau */
static void vLedTask(void *pvParameters)
{
    const led_cfg_t *cfg = (const led_cfg_t *)pvParameters;
    for (;;) {
        led_blink(cfg->port, cfg->pin, cfg->freq_hz);
    }
}

int main(void)
{
    SystemInit();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    led_init(led1.port, led1.pin);
    led_init(led2.port, led2.pin);
    led_init(led3.port, led3.pin);

    xTaskCreate(vLedTask, "LED_0.1Hz", 128, (void *)&led1, 1, NULL);
    xTaskCreate(vLedTask, "LED_1Hz",   128, (void *)&led2, 1, NULL);
    xTaskCreate(vLedTask, "LED_10Hz",  128, (void *)&led3, 1, NULL);

    vTaskStartScheduler();
    for (;;);
}
