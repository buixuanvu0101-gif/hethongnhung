#include "stm32f1xx.h"

// HAM DELAY CO BAN
__attribute__((noinline)) void delay(volatile unsigned int count) {
    while(count--) {
        __asm__("nop");
    }
}

// HAM TRONG DE THOA MAN YEU CAU GỌI HAM CUA STARTUP.S
void SystemInit(void) {
}

int main(void) {
    // BAT CLOCK CHO PORT C (BIT 4 TRONG THANH GHI APB2ENR)
    RCC->APB2ENR |= (1 << 4);

    // XOA CAU HINH CU VA THIET LAP CHAN PC13 LA OUTPUT PUSH-PULL
    GPIOC->CRH &= ~(0xF << 20);   
    GPIOC->CRH |=  (0x2 << 20);   

    while(1) {
        // BAT LED (KEO CHAN PC13 XUONG MUC THAP)
        GPIOC->ODR &= ~(1 << 13);   
        delay(1000000);             
        
        // TAT LED (DAY CHAN PC13 LEN MUC CAO)
        GPIOC->ODR |= (1 << 13);       
        delay(1000000);             
    }
}
