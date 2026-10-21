#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "led.h"
#include "button.h"

#define LED_INIT_LEVEL    5     /* 50% luc khoi dong          */
#define DEBOUNCE_SAMPLES  2     /* 2 mau x 20ms = 40ms        */
#define BUTTON_PERIOD_MS  20

/* Du lieu task nut nhan gui sang task LED qua Queue */
typedef enum {
    LED_CMD_UP,       /* tang do sang        */
    LED_CMD_DOWN,     /* giam do sang        */
    LED_CMD_SELECT    /* LED duoc chon: nhay 2 lan */
} led_cmd_t;

/* Moi LED co 1 Queue rieng */
static QueueHandle_t ledQueue[LED_COUNT];

/* ---- 3 task LED doc lap (cung 1 ham, khac tham so index) ---- */
static void vLedTask(void *pvParameters)
{
    const uint8_t idx = (uint8_t)(uint32_t)pvParameters;
    uint8_t level = LED_INIT_LEVEL;
    led_cmd_t cmd;
    uint8_t i;

    led_set_level(idx, level);

    for (;;) {
        /* Cho du lieu moi tu task nut nhan */
        if (xQueueReceive(ledQueue[idx], &cmd, portMAX_DELAY) == pdPASS) {
            switch (cmd) {
            case LED_CMD_UP:
                if (level < LED_MAX_LEVEL) level++;
                break;
            case LED_CMD_DOWN:
                if (level > 0) level--;
                break;
            case LED_CMD_SELECT:
                for (i = 0; i < 2; i++) {
                    led_set_level(idx, 0);
                    vTaskDelay(pdMS_TO_TICKS(100));
                    led_set_level(idx, LED_MAX_LEVEL);
                    vTaskDelay(pdMS_TO_TICKS(100));
                }
                break;
            }
            led_set_level(idx, level);   /* cap nhat trang thai LED */
        }
    }
}

/* ---- Task doc 3 nut nhan, gui lenh qua Queue ---- */
static void vButtonTask(void *pvParameters)
{
    uint8_t selected = 0;                 /* LED dang duoc dieu khien */
    uint8_t stable[BTN_COUNT] = {0, 0, 0};
    uint8_t cnt[BTN_COUNT]    = {0, 0, 0};
    uint8_t id, raw;
    led_cmd_t cmd;

    (void)pvParameters;

    /* Bao hieu LED dau tien dang duoc chon */
    cmd = LED_CMD_SELECT;
    xQueueSend(ledQueue[selected], &cmd, 0);

    for (;;) {
        for (id = 0; id < BTN_COUNT; id++) {
            raw = button_is_pressed((button_id_t)id);

            if (raw != stable[id]) {
                if (++cnt[id] >= DEBOUNCE_SAMPLES) {   /* chong rung */
                    stable[id] = raw;
                    cnt[id] = 0;

                    if (raw) {   /* canh xuong: vua nhan nut */
                        if (id == BTN_SELECT) {
                            selected = (selected + 1) % LED_COUNT;
                            cmd = LED_CMD_SELECT;
                        } else if (id == BTN_UP) {
                            cmd = LED_CMD_UP;
                        } else {
                            cmd = LED_CMD_DOWN;
                        }
                        xQueueSend(ledQueue[selected], &cmd, 0);
                    }
                }
            } else {
                cnt[id] = 0;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(BUTTON_PERIOD_MS));
    }
}

int main(void)
{
    uint8_t i;

    SystemInit();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    led_pwm_init();
    button_init();

    for (i = 0; i < LED_COUNT; i++) {
        ledQueue[i] = xQueueCreate(5, sizeof(led_cmd_t));
        if (ledQueue[i] == NULL) for (;;);
    }

    xTaskCreate(vLedTask, "LED1", 128, (void *)0, 1, NULL);
    xTaskCreate(vLedTask, "LED2", 128, (void *)1, 1, NULL);
    xTaskCreate(vLedTask, "LED3", 128, (void *)2, 1, NULL);
    xTaskCreate(vButtonTask, "BTN", 128, NULL, 2, NULL);

    vTaskStartScheduler();
    for (;;);
}