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
    
    RCC->APB2ENR |= (1 << 2);

    GPIOA->CRL &= ~((0xF << 4) | (0xF << 28)); 
    GPIOA->CRL |=  ((0x8 << 4) | (0x2 << 28));

    GPIOA->ODR |= (1 << 1);

    int last_btn_state = 1; // GIA SU TRANG THAI BAN DAU NUT NHAN O MUC CAO (PULL-UP)
    int led_state = 0;      // TRANG THAI HIEN TAI CUA LED

    while(1) {
      
        int current_btn_state = (GPIOA->IDR & (1 << 1)) ? 1 : 0;

      
        if (last_btn_state == 0 && current_btn_state == 1) {
            led_state = !led_state; // DAO TRANG THAI LED
            
            if (led_state) {
                GPIOA->ODR |= (1 << 7);   // BAT LED TAI PA7
            } else {
                GPIOA->ODR &= ~(1 << 7);  // TAT LED TAI PA7
            }
        }

        last_btn_state = current_btn_state;
        delay(50000); // CHONG DOI PHIM DON GIAN (DEBOUNCE)
    }
}
