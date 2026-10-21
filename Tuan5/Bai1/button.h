#ifndef BUTTON_H
#define BUTTON_H

#include "stm32f10x.h"

typedef enum {
    BTN_SELECT = 0,   /* PB12: chuyen LED       */
    BTN_UP,           /* PB13: tang do sang LED */
    BTN_DOWN,         /* PB14: giam do sang LED */
    BTN_COUNT
} button_id_t;

void    button_init(void);
uint8_t button_is_pressed(button_id_t id);   /* 1 = dang nhan */

#endif
