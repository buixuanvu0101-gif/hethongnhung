#define RCC_BASE      0x40021000
#define GPIOA_BASE    0x40010800

#define RCC_APB2ENR   (*(volatile unsigned int *)(RCC_BASE + 0x18))
#define GPIOA_CRL     (*(volatile unsigned int *)(GPIOA_BASE + 0x00))
#define GPIOA_IDR     (*(volatile unsigned int *)(GPIOA_BASE + 0x08))
#define GPIOA_ODR     (*(volatile unsigned int *)(GPIOA_BASE + 0x0C))

__attribute__((noinline)) void delay(volatile unsigned int count) {
    while(count--) {
        __asm__("nop");
    }
}

int main(void) {
    RCC_APB2ENR |= (1 << 2);
    GPIOA_CRL &= ~((0xF << 4) | (0xF << 28)); 
    GPIOA_CRL |=  ((0x8 << 4) | (0x2 << 28));


    int last_btn_state = 1; 
    int led_state = 0;      

    while(1) {
        int current_btn_state = (GPIOA_IDR & (1 << 1)) ? 1 : 0;

        if (last_btn_state == 0 && current_btn_state == 1) {
            led_state = !led_state; // Đảo trạng thái LED
            if (led_state) {
                GPIOA_ODR |= (1 << 7);   
            } else {
                GPIOA_ODR &= ~(1 << 7); 
            }
        }

        last_btn_state = current_btn_state;
        delay(50000); 
    }
}
