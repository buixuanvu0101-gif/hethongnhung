#define RCC_BASE      0x40021000
#define GPIOC_BASE    0x40011000

#define RCC_APB2ENR   (*(volatile unsigned int *)(RCC_BASE + 0x18))
#define GPIOC_CRH     (*(volatile unsigned int *)(GPIOC_BASE + 0x04))
#define GPIOC_ODR     (*(volatile unsigned int *)(GPIOC_BASE + 0x0C))

__attribute__((noinline)) void delay(volatile unsigned int count) {
    while(count--) {
        __asm__("nop");
    }
}

int main(void) {
    RCC_APB2ENR |= (1 << 4);

    GPIOC_CRH &= ~(0xF << 20);   
    GPIOC_CRH |=  (0x2 << 20);   

    while(1) {
        GPIOC_ODR &= ~(1 << 13);   
        delay(1000000);           
        GPIOC_ODR |= (1 << 13);    
        delay(1000000);           
    }
}
