# Bài tập 01 - FreeRTOS trên STM32F103

Chương trình cho VĐK STM32F103 (Blue Pill, STM32F103C8T6):

- Porting thành công hệ điều hành FreeRTOS lên STM32F103.
- 01 hàm điều khiển nháy LED `led_blink(port, pin, freq_hz)` với đầu vào gồm: chân LED, tần số nhấp nháy.
- 3 task FreeRTOS cùng gọi hàm trên để điều khiển 3 LED ở 3 chân với 3 tần số độc lập.

| LED | Chân | Tần số |
|-----|------|--------|
| 1   | PA0  | 0.1 Hz |
| 2   | PA1  | 1 Hz   |
| 3   | PA2  | 10 Hz  |

Mỗi chân LED nối qua điện trở khoảng 330 Ω, cathode nối GND.

## Video demo

<dán link video>

## Cấu trúc thư mục

    Libraries/          Thư viện STM32F10x Standard Peripheral (CMSIS + Driver) và FreeRTOS Kernel
    Makefile            Build, nạp chương trình
    linker.ld           Linker script (64KB Flash, 20KB RAM)
    main.c              3 task FreeRTOS, hàm main
    led.c / led.h       Hàm led_init(), led_blink()
    stm32f10x_conf.h    Cấu hình thư viện Standard Peripheral
    FreeRTOSConfig.h    Cấu hình FreeRTOS

## Cách hoạt động

- Tần số f (Hz) tương ứng chu kỳ T = 1/f. Mỗi nửa chu kỳ (sáng hoặc tắt) kéo dài 500/f ms: 0.1 Hz là 5000 ms, 1 Hz là 500 ms, 10 Hz là 50 ms.
- Thời gian chờ dùng `vTaskDelay`, nên các task nhường CPU cho nhau và chạy độc lập.
- FreeRTOS dùng SysTick làm tick hệ thống (1 kHz), PendSV để chuyển task, SVC để khởi động scheduler. Heap dùng heap_4.

## Build và nạp

Yêu cầu: Ubuntu, gcc-arm-none-eabi, make, stlink-tools.

    sudo apt install gcc-arm-none-eabi libnewlib-arm-none-eabi make stlink-tools
    make -j4          # build
    make flash        # nạp qua ST-Link
    make clean        # xóa thư mục build

Nối ST-Link: SWDIO → PA13, SWCLK → PA14, GND → GND, 3.3V → 3.3V.
