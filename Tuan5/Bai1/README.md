# Tuần 5 - Bài 1: FreeRTOS, PWM LED và nút nhấn trên STM32F103

Chương trình cho VĐK STM32F103 (Blue Pill) với FreeRTOS:

- 3 LED băm xung (PWM) tần số 1 kHz (TIM2 CH1, CH2, CH3).
- 3 nút nhấn: chuyển LED, tăng độ sáng, giảm độ sáng.
- 3 LED hoạt động bằng 3 task độc lập.
- 1 task đọc 3 nút nhấn (có chống rung), gửi dữ liệu qua Queue sang 3 task LED.

| Thành phần | Chân |
|------------|------|
| LED 1 / 2 / 3 (PWM) | PA0 / PA1 / PA2 |
| Nút chuyển LED | PB12 |
| Nút tăng độ sáng | PB13 |
| Nút giảm độ sáng | PB14 |

Nút nối giữa chân và GND (dùng điện trở kéo lên nội). Mỗi LED nối qua điện trở khoảng 330 Ω.

## Video demo

<dán link video>

## Cách hoạt động

- TIM2 chạy 1 MHz, chu kỳ đếm 1000, nên PWM là 1 kHz. Độ sáng có 11 mức (0 đến 10), mỗi mức 10% độ rộng xung.
- Task nút nhấn quét mỗi 20 ms, chống rung 2 mẫu liên tiếp. Khi có lệnh mới, task gửi `LED_CMD_UP`, `LED_CMD_DOWN` hoặc `LED_CMD_SELECT` vào Queue của LED đang được chọn.
- Mỗi task LED chờ Queue riêng bằng `xQueueReceive`, nhận lệnh thì cập nhật độ sáng. Khi nhận `LED_CMD_SELECT`, LED nháy 2 lần để báo đang được điều khiển.

## Cấu trúc thư mục

    Libraries/           Thư viện STM32F10x Standard Peripheral và FreeRTOS Kernel
    Makefile             Build, nạp chương trình
    linker.ld            Linker script
    main.c               Task nút nhấn, 3 task LED, Queue, hàm main
    led.c / led.h        Cấu hình PWM TIM2, đặt độ sáng LED
    button.c / button.h  Cấu hình và đọc nút nhấn
    stm32f10x_conf.h     Cấu hình thư viện Standard Peripheral
    FreeRTOSConfig.h     Cấu hình FreeRTOS

## Build và nạp

    make -j4
    make flash
    