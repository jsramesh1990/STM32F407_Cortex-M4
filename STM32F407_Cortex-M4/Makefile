# ============================================================
# STM32F407 Cortex-M4
# Scrambler Word Solver
# ============================================================

TARGET = scrambler_stm32

# ------------------------------------------------------------
# Toolchain
# ------------------------------------------------------------

CC      = arm-none-eabi-gcc
AS      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# ------------------------------------------------------------
# MCU
# ------------------------------------------------------------

CPU = cortex-m4

MCU_FLAGS = \
	-mcpu=$(CPU) \
	-mthumb \
	-mfpu=fpv4-sp-d16 \
	-mfloat-abi=hard

# ------------------------------------------------------------
# Compiler flags
# ------------------------------------------------------------

CFLAGS = \
	$(MCU_FLAGS) \
	-std=c11 \
	-O2 \
	-g3 \
	-Wall \
	-Wextra \
	-ffunction-sections \
	-fdata-sections \
	-I. \
	-Iapplication \
	-Ibsp \
	-Idrivers \
	-Idrivers/uart \
	-Idrivers/spi \
	-Idrivers/flash \
	-Idrivers/gpio

# ------------------------------------------------------------
# Linker
# ------------------------------------------------------------

LDSCRIPT = linker/STM32F407VG_FLASH.ld

LDFLAGS = \
	$(MCU_FLAGS) \
	-T$(LDSCRIPT) \
	-Wl,--gc-sections \
	-Wl,-Map=build/$(TARGET).map \
	-specs=nano.specs \
	-specs=nosys.specs

# ------------------------------------------------------------
# Source files
# ------------------------------------------------------------

SOURCES = \
	main.c \
	bsp/board.c \
	bsp/clock.c \
	application/scrambler.c \
	application/input_manager.c \
	application/dictionary.c \
	application/result_manager.c \
	drivers/uart/uart.c \
	drivers/spi/spi.c \
	drivers/flash/flash.c \
	drivers/gpio/gpio.c \
	startup/startup_stm32f407xx.s

# ------------------------------------------------------------
# Object files
# ------------------------------------------------------------

OBJECTS = $(patsubst %.c,build/obj/%.o,$(filter %.c,$(SOURCES)))

OBJECTS += $(patsubst %.s,build/obj/%.o,$(filter %.s,$(SOURCES)))

# ------------------------------------------------------------
# Default target
# ------------------------------------------------------------

all: directories build/$(TARGET).elf build/$(TARGET).hex build/$(TARGET).bin
	@echo ""
	@echo "=========================================="
	@echo " STM32F407 BUILD SUCCESS"
	@echo "=========================================="
	@echo ""
	$(SIZE) build/$(TARGET).elf
	@echo ""
	@echo "Generated files:"
	@echo "  build/$(TARGET).elf"
	@echo "  build/$(TARGET).hex"
	@echo "  build/$(TARGET).bin"
	@echo "  build/$(TARGET).map"
	@echo ""

# ------------------------------------------------------------
# Create directories
# ------------------------------------------------------------

directories:
	@mkdir -p build
	@mkdir -p build/obj
	@mkdir -p build/obj/bsp
	@mkdir -p build/obj/application
	@mkdir -p build/obj/drivers/uart
	@mkdir -p build/obj/drivers/spi
	@mkdir -p build/obj/drivers/flash
	@mkdir -p build/obj/drivers/gpio
	@mkdir -p build/obj/startup

# ------------------------------------------------------------
# Compile C source files
# ------------------------------------------------------------

build/obj/%.o: %.c
	@mkdir -p $(dir $@)
	@echo " CC  $<"
	$(CC) $(CFLAGS) -c $< -o $@

# ------------------------------------------------------------
# Assemble startup file
# ------------------------------------------------------------

build/obj/%.o: %.s
	@mkdir -p $(dir $@)
	@echo " AS  $<"
	$(AS) $(CFLAGS) -c $< -o $@

# ------------------------------------------------------------
# Link ELF
# ------------------------------------------------------------

build/$(TARGET).elf: $(OBJECTS)
	@echo ""
	@echo " LINK $@"
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

# ------------------------------------------------------------
# Generate HEX
# ------------------------------------------------------------

build/$(TARGET).hex: build/$(TARGET).elf
	@echo " HEX  $@"
	$(OBJCOPY) -O ihex $< $@

# ------------------------------------------------------------
# Generate BIN
# ------------------------------------------------------------

build/$(TARGET).bin: build/$(TARGET).elf
	@echo " BIN  $@"
	$(OBJCOPY) -O binary $< $@

# ------------------------------------------------------------
# Clean
# ------------------------------------------------------------

clean:
	rm -rf build

# ------------------------------------------------------------
# Rebuild
# ------------------------------------------------------------

rebuild: clean all

# ------------------------------------------------------------
# Show memory usage
# ------------------------------------------------------------

size: build/$(TARGET).elf
	$(SIZE) $<

.PHONY: all clean rebuild directories size
