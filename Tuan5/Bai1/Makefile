TARGET  := led_freertos
BUILD   := build
STD      := lib/STM32F10x_StdPeriph_Lib_V3.5.0/Libraries
CMSIS_DEV:= $(STD)/CMSIS/CM3/DeviceSupport/ST/STM32F10x
CMSIS_CORE:= $(STD)/CMSIS/CM3/CoreSupport
SPL      := $(STD)/STM32F10x_StdPeriph_Driver
FRT      := lib/FreeRTOS-Kernel

C_SRCS := $(wildcard src/*.c) \
  $(CMSIS_DEV)/system_stm32f10x.c \
  $(SPL)/src/stm32f10x_rcc.c \
  $(SPL)/src/stm32f10x_gpio.c \
  $(SPL)/src/misc.c \
  $(FRT)/tasks.c \
  $(FRT)/list.c \
  $(FRT)/queue.c \
  $(FRT)/portable/GCC/ARM_CM3/port.c \
  $(FRT)/portable/MemMang/heap_4.c

ASM_SRCS := $(CMSIS_DEV)/startup/gcc_ride7/startup_stm32f10x_md.s

INCLUDES := -Iinc -I$(CMSIS_DEV) -I$(CMSIS_CORE) -I$(SPL)/inc \
  -I$(FRT)/include -I$(FRT)/portable/GCC/ARM_CM3
DEFINES := -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER

PREFIX := arm-none-eabi-
CC := $(PREFIX)gcc
SZ := $(PREFIX)size
OBJCOPY := $(PREFIX)objcopy

CPU := -mcpu=cortex-m3 -mthumb
CFLAGS  := $(CPU) $(DEFINES) $(INCLUDES) -Os -g3 -Wall -ffunction-sections -fdata-sections -MMD -MP
ASFLAGS := $(CPU) -g3
LDFLAGS := $(CPU) -T linker/STM32F103C8Tx_FLASH.ld --specs=nano.specs --specs=nosys.specs \
  -Wl,--gc-sections -Wl,-Map=$(BUILD)/$(TARGET).map

OBJS := $(addprefix $(BUILD)/,$(notdir $(C_SRCS:.c=.o))) $(addprefix $(BUILD)/,$(notdir $(ASM_SRCS:.s=.o)))
vpath %.c $(sort $(dir $(C_SRCS)))
vpath %.s $(sort $(dir $(ASM_SRCS)))

all: $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).bin $(BUILD)/$(TARGET).hex
	$(SZ) $<

$(BUILD)/%.o: %.c | $(BUILD)
	@echo CC $<
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.s | $(BUILD)
	@echo AS $<
	@$(CC) $(ASFLAGS) -c $< -o $@

$(BUILD)/$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(BUILD)/%.bin: $(BUILD)/%.elf
	$(OBJCOPY) -O binary $< $@

$(BUILD)/%.hex: $(BUILD)/%.elf
	$(OBJCOPY) -O ihex $< $@

$(BUILD):
	mkdir -p $@

flash: all
	st-flash write $(BUILD)/$(TARGET).bin 0x8000000

flash-openocd: all
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program $(BUILD)/$(TARGET).elf verify reset exit"

clean:
	rm -rf $(BUILD)

-include $(wildcard $(BUILD)/*.d)
.PHONY: all flash flash-openocd clean
