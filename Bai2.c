#define RCC_BASE      0x40021000
#define GPIOA_BASE    0x40010800

#define RCC_APB2ENR   (*(volatile unsigned int *)(RCC_BASE + 0x18))
#define GPIOA_CRL     (*(volatile unsigned int *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR     (*(volatile unsigned int *)(GPIOA_BASE + 0x0C))

__attribute__((noinline)) void delay(volatile unsigned int count) {
    while(count--) {
        __asm__("nop");
    }
}

int main(void) {
    // Bat clock cho GPIOA (Bit 2)
    RCC_APB2ENR |= (1 << 2);

    // Cau hinh 8 chan PA0 - PA7 o che do Output Push-Pull 2MHz
    // Moi chan chiem 4 bit trong GPIOA_CRL. Cau hinh 0x2 cho ca 8 chan: 0x22222222
    GPIOA_CRL = 0x22222222;

    while(1) {
        // Chay tu PA0 den PA7
        for(int i = 0; i < 8; i++) {
            GPIOA_ODR = (1 << i);
            delay(400000);
        }
        // Chay nguoc lai tu PA6 ve PA1
        for(int i = 6; i > 0; i--) {
            GPIOA_ODR = (1 << i);
            delay(400000);
        }
    }
}
