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
    // 1. Bật clock cho GPIOA (Bit 2 tương ứng với IOPAEN)
    RCC_APB2ENR |= (1 << 2);

    // 2. Cấu hình PA1 làm Input (Pull-up/Pull-down) và PA7 làm Output Push-Pull 2MHz
    // - PA1 nằm ở các bit [6:4] của GPIOA_CRL: MODE1 = 00 (Input), CNF1 = 10 (Input with pull-up/pull-down) -> Giá trị b'1000 = 0x8
    // - PA7 nằm ở các bit [31:28] của GPIOA_CRL: MODE7 = 10 (Output 2MHz), CNF7 = 00 (Push-pull) -> Giá trị b'0010 = 0x2
    GPIOA_CRL &= ~((0xF << 4) | (0xF << 28)); // Xóa cấu hình cũ của PA1 và PA7
    GPIOA_CRL |=  ((0x8 << 4) | (0x2 << 28)); // Thiết lập mới

    // Bật điện trở kéo lên (Pull-up) cho PA1 bằng cách ghi mức 1 vào ODR1 (nếu dùng nút nhấn kéo xuống GND)
    GPIOA_ODR |= (1 << 1);

    int last_btn_state = 1; // Giả sử trạng thái ban đầu nút nhấn ở mức cao (Pull-up)
    int led_state = 0;      // Trạng thái hiện tại của LED

    while(1) {
        int current_btn_state = (GPIOA_IDR & (1 << 1)) ? 1 : 0;

        // Phát hiện sự kiện nhấn rồi nhả (sườn xuống rồi lên, hoặc đơn giản kiểm tra thay đổi)
        if (last_btn_state == 0 && current_btn_state == 1) {
            led_state = !led_state; // Đảo trạng thái LED
            if (led_state) {
                GPIOA_ODR |= (1 << 7);   // Bật LED tại PA7
            } else {
                GPIOA_ODR &= ~(1 << 7);  // Tắt LED tại PA7
            }
        }

        last_btn_state = current_btn_state;
        delay(50000); // Chống dội phím đơn giản (debounce)
    }
}
