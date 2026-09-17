#include "stm32f1xx.h"

// HAM DELAY CO BAN
__attribute__((noinline)) void delay(volatile unsigned int count) {
    while(count--) {
        __asm__("nop");
    }
}

// HAM TRONG DE THOA MAN YEU CAU GOI HAM CUA STARTUP.S
void SystemInit(void) {
}

int main(void) {
    // BAT CLOCK CHO GPIOA (BIT 2 TRONG THANH GHI APB2ENR)
    RCC->APB2ENR |= (1 << 2);

    // CAU HINH 8 CHAN PA0 - PA7 O CHE DO OUTPUT PUSH-PULL 2MHz
    // MOI CHAN CHIEM 4 BIT TRONG GPIOA_CRL. CAU HINH 0x2 CHO CA 8 CHAN: 0x22222222
    GPIOA->CRL = 0x22222222;

    while(1) {
        // HIEU UNG LED CHAY TREN THANH GHI ODR
        // CHAY TU PA0 DEN PA7
        for(int i = 0; i < 8; i++) {
            GPIOA->ODR = (1 << i);
            delay(400000);
        }
        
        // CHAY NGUOC LAI TU PA6 VE PA1
        for(int i = 6; i > 0; i--) {
            GPIOA->ODR = (1 << i);
            delay(400000);
        }
    }
}
