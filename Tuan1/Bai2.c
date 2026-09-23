#include "stm32f1xx.h"


__attribute__((noinline)) void delay(volatile unsigned int count) {
    while(count--) {
        __asm__("nop");
    }
}


void SystemInit(void) {
}

int main(void) {
    
    RCC->APB2ENR |= (1 << 2);

    GPIOA->CRL = 0x22222222;

    while(1) {
       
        for(int i = 0; i < 8; i++) {
            GPIOA->ODR = (1 << i);
            delay(400000);
        }
        
       
        for(int i = 6; i > 0; i--) {
            GPIOA->ODR = (1 << i);
            delay(400000);
        }
    }
}
