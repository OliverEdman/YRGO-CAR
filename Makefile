TARGET = YRGO-CAR
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

BUILD_DIR = build

INC = -I drivers/include \
      -I modules/include \
      -I app \
      -I arch \
      -I include

# Separera C-källfiler och Assembler-källfiler
C_SRCS += app/main.c
C_SRCS += drivers/source/gpio.c
C_SRCS += drivers/source/systick.c
#C_SRCS += drivers/source/timer.c
#C_SRCS += drivers/source/pwm.c
#C_SRCS += drivers/source/adc.c
#C_SRCS += drivers/source/spi.c
#C_SRCS += drivers/source/uart.c

ASM_SRCS += arch/startup.s

# Skapa objektfilslistor för både .c och .s
OBJS = $(addprefix $(BUILD_DIR)/, $(C_SRCS:.c=.o))
OBJS += $(addprefix $(BUILD_DIR)/, $(ASM_SRCS:.s=.o))

CFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
         -O2 -g -Wall -Wextra -Werror -Wshadow -Wundef -Wconversion -std=c99 $(INC)

ASMFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g

LDFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
          -Tarch/stm32f446re.ld --specs=nano.specs -nostartfiles -Wl,--gc-sections

all: $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).list
	@echo ""
	@echo "========================================="
	@echo " YRGO-CAR built successfully!"
	@echo "========================================="
	@echo ""

build: all

# Regel för att kompilera C-filer
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Regel för att kompilera Assembler-filer (.s)
$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(ASMFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/$(TARGET).list: $(BUILD_DIR)/$(TARGET).elf
	$(OBJDUMP) -h -S $< > $@

format:
	@clang-format -i $$(find . -type f \( -name "*.c" -o -name "*.h" \) ! -path "./build/*")
	@echo ""
	@echo "========================================="
	@echo " Code formatted successfully (YRGO-CAR)!"
	@echo "========================================="
	@echo ""

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Clean complete! /build directory removed."

flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash write $< 0x08000000
	@echo ""
	@echo "=================================================="
	@echo " YRGO-CAR flashed successfully to target!"
	@echo "=================================================="
	@echo " Memory Usage:"
	@eval $$(arm-none-eabi-size $(BUILD_DIR)/$(TARGET).elf | awk 'NR==2 {print "TEXT="$$1 "; DATA="$$2 "; BSS="$$3}') ; \
	FLASH_USED=$$(($$TEXT + $$DATA)) ; \
	RAM_USED=$$(($$DATA + $$BSS)) ; \
	FLASH_TOTAL=524288 ; \
	RAM_TOTAL=131072 ; \
	FLASH_PCT=$$(($$FLASH_USED * 100 / $$FLASH_TOTAL)) ; \
	RAM_PCT=$$(($$RAM_USED * 100 / $$RAM_TOTAL)) ; \
	echo "  FLASH: $$FLASH_USED / $$FLASH_TOTAL bytes [$$FLASH_PCT% Used]" ; \
	echo "  SRAM:  $$RAM_USED / $$RAM_TOTAL bytes [$$RAM_PCT% Used]"

.PHONY: all build format clean flash