# Copyright (c) 2019-2020, Arm Limited. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

CROSS_COMPILE?=arm-none-eabi-
TARGET += -mcpu=cortex-m0 -mthumb -mfloat-abi=softfp

CFLAGS += -g2 -O0 -Wall -Werror -fno-strict-aliasing
CFLAGS += -DNRF51
CXXFLAGS += -g2 -O0 -Wall -Werror -fno-strict-aliasing -fno-exceptions -fno-rtti
CXXFLAGS += -DNRF51

BUILDDIR := build
NRF_SDK_DIR := nrf_sdk10

SOURCES := $(shell find . -name "*.c" -not -path "./$(NRF_SDK_DIR)/*")
SOURCES += $(shell find . -name "*.S" -not -path "./$(NRF_SDK_DIR)/*")

# nRF SDK files are added manually
SOURCES += $(NRF_SDK_DIR)/components/toolchain/system_nrf51.c
SOURCES += $(NRF_SDK_DIR)/components/drivers_nrf/delay/nrf_delay.c
SOURCES += $(NRF_SDK_DIR)/components/drivers_nrf/hal/nrf_adc.c
SOURCES += $(NRF_SDK_DIR)/components/toolchain/gcc/gcc_startup_nrf51.s

INCLUDE_DIRS := .
INCLUDE_DIRS += $(NRF_SDK_DIR)/components/device
INCLUDE_DIRS += $(NRF_SDK_DIR)/components/drivers_nrf/delay
INCLUDE_DIRS += $(NRF_SDK_DIR)/components/drivers_nrf/hal
INCLUDE_DIRS += $(NRF_SDK_DIR)/components/toolchain
INCLUDE_DIRS += $(NRF_SDK_DIR)/components/toolchain/gcc

OBJS := $(SOURCES:.c=.o)
OBJS := $(OBJS:.s=.o)
OBJS := $(OBJS:.S=.o)
OBJS := $(addprefix $(BUILDDIR)/,$(OBJS))
DEPS := $(OBJS:.o=.d)

LIBS += m
LIBS += c

LDFLAGS += --specs=nano.specs
LDFLAGS += --specs=nosys.specs
LDFLAGS += -L $(NRF_SDK_DIR)/components/toolchain/gcc/

LDSCRIPT := nrf51_gcc.ld

# Compiler flags
CFLAGS += $(TARGET)
CFLAGS += -fdata-sections -ffunction-sections
CFLAGS += $(addprefix -D,$(DEFINES))
CFLAGS += $(addprefix -I,$(INCLUDE_DIRS))

CXXFLAGS += $(TARGET)
CXXFLAGS += -fdata-sections -ffunction-sections
CXXFLAGS += $(addprefix -D,$(DEFINES))
CXXFLAGS += $(addprefix -I,$(INCLUDE_DIRS))

ASFLAGS += $(TARGET)
ASFLAGS += -x assembler-with-cpp
ASFLAGS += -DNRF51

LDFLAGS += $(TARGET)
LDFLAGS += $(addprefix -L,$(LIB_DIRS))
LDFLAGS += $(addprefix -l,$(LIBS))

CC=$(CROSS_COMPILE)gcc
AS=$(CROSS_COMPILE)g++
LD=$(CROSS_COMPILE)g++
OBJDUMP=$(CROSS_COMPILE)objdump
OBJCOPY=$(CROSS_COMPILE)objcopy

MKDIR:=mkdir -p

all: rover

clean:
	rm -rf $(BUILDDIR) rover.elf rover.bin

rover: rover.elf rover.bin $(BUILDDIR)/rover.dump

$(BUILDDIR)/rover.dump: rover.elf
	@echo "Exporting  $(@)"
	@$(OBJDUMP) -d $(<) > $(@)

rover.bin: rover.elf
	@echo "Creating   $(@)"
	@$(OBJCOPY) -O binary $(<) $(@)

rover.elf: $(OBJS)
	@echo "Linking    $(@)"
	@$(LD) $(LDFLAGS) -T $(LDSCRIPT) $(OBJS) $(STATIC_LIBS) -o $@

$(BUILDDIR)/%.o: %.c
	@echo "CC  $(<)"
	@$(MKDIR) $(dir $(@))
	@$(CC) $(CFLAGS) -MMD -MP -MF$(@:%.o=%.d) -MT$(@) -c $(<) -o $(@)

$(BUILDDIR)/%.o: %.s
	@echo "AS  $(<)"
	@$(MKDIR) $(dir $(@))
	@$(AS) $(ASFLAGS) -c $(<) -o $(@)

$(BUILDDIR)/%.o: %.S
	@echo "AS  $(<)"
	@$(MKDIR) $(dir $(@))
	@$(AS) $(ASFLAGS) -c $(<) -o $(@)

-include $(DEPS)
