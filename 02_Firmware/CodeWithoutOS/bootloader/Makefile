 
TARGET = bootloader
PROJECT_ROOT=..

INCLUDE+=-Iinc

SRC_C+=$(wildcard src/*.c)

#SRAM RUN BROM AREA
LDSCRIPT=$(PROJECT_ROOT)/drivers/f1c100s_sram.ld

# add eGON.BT0 header bootloader need
CFLAGS+=-DEGON_HEADER

# printf size optimization
CFLAGS+=-DPRINTF_SUPPORT_DECIMAL_SPECIFIERS=0 -DPRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS=0

include $(PROJECT_ROOT)/drivers/project.mk
