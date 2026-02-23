# Makefile for STM32 Project with FreeRTOS
# Usage:  make TARGET=STM32F07X        (default)
#         make TARGET=STM32H753

# ──────────────────────────────────────────────
# Target selection (override on command line)
# ──────────────────────────────────────────────
TARGET ?= STM32F07X
TARGET_DIR = targets/$(TARGET)

# Per-target settings
ifeq ($(TARGET),STM32F07X)
  CPU_FLAGS    = -mcpu=cortex-m0 -mthumb
  FREERTOS_PORT = GCC/ARM_CM0
  PORT_SRCS    = port.c portasm.c
  FLASH_ADDR   = 0x08000000
  HAS_ETHERNET = 0
endif

ifeq ($(TARGET),STM32H753)
  CPU_FLAGS    = -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16
  FREERTOS_PORT = GCC/ARM_CM7/r0p1
  PORT_SRCS    = port.c
  FLASH_ADDR   = 0x08000000
  HAS_ETHERNET = 1
endif

# ──────────────────────────────────────────────
# Toolchain
# ──────────────────────────────────────────────
CC      = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
MKDIR_P = mkdir -p

# ──────────────────────────────────────────────
# Directories
# ──────────────────────────────────────────────
BUILD_DIR        = build/$(TARGET)
FREERTOS_DIR     = FreeRTOS-Kernel
FREERTOS_PORT_DIR = $(FREERTOS_DIR)/portable/$(FREERTOS_PORT)

# ──────────────────────────────────────────────
# Source files
# ──────────────────────────────────────────────
SRCS = src/main.cpp \
       src/services/sensors/sensors.cpp \
       src/modules/i2c/i2c.cpp \
       src/modules/gnss/gnss.cpp \
       src/modules/imu/imu.cpp \
       src/modules/thermal_humidity/thermal_humidity.cpp \
       src/modules/barometer/barometer.cpp \
       src/modules/aerosol/aerosol.cpp \
       src/modules/solar/solar.cpp \
       src/modules/infrared/infrared.cpp \
       $(FREERTOS_DIR)/tasks.c \
       $(FREERTOS_DIR)/queue.c \
       $(FREERTOS_DIR)/list.c \
       $(FREERTOS_DIR)/timers.c \
       $(FREERTOS_DIR)/event_groups.c \
       $(FREERTOS_DIR)/stream_buffer.c \
       $(addprefix $(FREERTOS_PORT_DIR)/,$(PORT_SRCS)) \
       $(FREERTOS_DIR)/portable/MemMang/heap_4.c

# Conditionally add ethernet module + telemetry service
ifeq ($(HAS_ETHERNET),1)
  SRCS += src/modules/ethernet/ethernet.cpp \
          src/services/telemetry/telemetry.cpp
endif

# F07X-specific: blink4s LED driver + ledring animation
ifeq ($(TARGET),STM32F07X)
  SRCS += src/modules/blink4s/blink4s.cpp \
          src/services/ledring/ledring.cpp
  DEFINES += -DHAS_RING=1
endif

OBJS = $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

# ──────────────────────────────────────────────
# Output files
# ──────────────────────────────────────────────
ELF = $(BUILD_DIR)/main.elf
BIN = $(BUILD_DIR)/main.bin
MAP = $(BUILD_DIR)/main.map
LDSCRIPT = $(wildcard $(TARGET_DIR)/*.ld)

# ──────────────────────────────────────────────
# Flags
# ──────────────────────────────────────────────
INCLUDES = -I $(TARGET_DIR) \
           -I src \
           -I src/modules/blink4s \
           -I src/modules/i2c \
           -I src/modules/gnss \
           -I src/modules/imu \
           -I src/modules/thermal_humidity \
           -I src/modules/barometer \
           -I src/modules/aerosol \
           -I src/modules/solar \
           -I src/modules/infrared \
           -I src/services/ledring \
           -I src/services/sensors \
           -I src/modules/ethernet \
           -I src/services/telemetry \
           -I $(FREERTOS_DIR)/include \
           -I $(FREERTOS_PORT_DIR)

DEFINES += -DHAS_ETHERNET=$(HAS_ETHERNET)

CFLAGS = -std=c++11 $(CPU_FLAGS) -g -O2 -Wall \
         $(DEFINES) $(INCLUDES) \
         -fno-exceptions -fno-rtti \
         -MMD -MP \
         -w

LDFLAGS = -T$(LDSCRIPT) -nostartfiles -Wl,-Map=$(MAP) \
          --specs=nano.specs --specs=nosys.specs

# ──────────────────────────────────────────────
# Rules
# ──────────────────────────────────────────────
.PHONY: all clean flash info

all: $(BIN)

# Link
$(ELF): $(OBJS) $(LDSCRIPT)
	@echo "[LD]  $@  ($(TARGET))"
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@

# Compile C++
$(BUILD_DIR)/%.cpp.o: %.cpp
	@$(MKDIR_P) $(dir $@)
	@echo "[CXX] $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Compile C
$(BUILD_DIR)/%.c.o: %.c
	@$(MKDIR_P) $(dir $@)
	@echo "[CC]  $<"
	@$(CC) $(CFLAGS) -x c -c $< -o $@

# Binary generation
$(BIN): $(ELF)
	@$(OBJCOPY) -O binary $< $@
	@echo "[BIN] $@"

# Clean build directory
clean:
	rm -rf build/

# Clear/Erase the device
clear:
	st-flash erase

# Flash the device (clears first for safety, resets after write)
flash: all clear
	st-flash --reset write $(BIN) $(FLASH_ADDR)

# Print current config
info:
	@echo "TARGET          = $(TARGET)"
	@echo "CPU_FLAGS       = $(CPU_FLAGS)"
	@echo "FREERTOS_PORT   = $(FREERTOS_PORT)"
	@echo "LDSCRIPT        = $(LDSCRIPT)"
	@echo "BUILD_DIR       = $(BUILD_DIR)"
	@echo "FLASH_ADDR      = $(FLASH_ADDR)"
	@echo "HAS_ETHERNET    = $(HAS_ETHERNET)"

-include $(DEPS)
