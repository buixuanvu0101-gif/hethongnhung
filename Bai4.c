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
    // 1. BAT CLOCK CHO GPIOA (BIT 2 TUONG UNG VOI IOPAEN)
    RCC->APB2ENR |= (1 << 2);

    // 2. CAU HINH PA1 LAM INPUT (PULL-UP/PULL-DOWN) VA PA7 LAM OUTPUT PUSH-PULL 2MHz
    // - PA1 NAM O CAC BIT [7:4] CUA GPIOA_CRL: MODE1 = 00 (Input), CNF1 = 10 (Input with pull-up/pull-down) -> GIA TRI b'1000 = 0x8
    // - PA7 NAM O CAC BIT [31:28] CUA GPIOA_CRL: MODE7 = 10 (Output 2MHz), CNF7 = 00 (Push-pull) -> GIA TRI b'0010 = 0x2
    
    GPIOA->CRL &= ~((0xF << 4) | (0xF << 28)); // XOA CAU HINH CU CUA PA1 VA PA7
    GPIOA->CRL |=  ((0x8 << 4) | (0x2 << 28)); // THIET LAP CAU HINH MOI

    // BAT DIEN TRO KEO LEN (PULL-UP) CHO PA1 BANG CACH GHI MUC 1 VAO ODR1
    GPIOA->ODR |= (1 << 1);

    int last_btn_state = 1; // GIA SU TRANG THAI BAN DAU NUT NHAN O MUC CAO (PULL-UP)
    int led_state = 0;      // TRANG THAI HIEN TAI CUA LED

    while(1) {
        // DOC TRANG THAI TAI CHAN PA1
        int current_btn_state = (GPIOA->IDR & (1 << 1)) ? 1 : 0;

        // PHAT HIEN SU KIEN NHAN ROI NHA (KIEM TRA THAY DOI TU THAP LEN CAO)
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
