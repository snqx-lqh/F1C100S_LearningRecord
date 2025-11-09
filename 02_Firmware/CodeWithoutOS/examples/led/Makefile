
TARGET=led

PROJECT_ROOT=../..
SRC_C+=$(wildcard src/*.c)
INCLUDE +=-Iinc
#SRAM RUN BROM AREA
LDSCRIPT=$(PROJECT_ROOT)/drivers/f1c100s_dram.ld

# add eGON.BT0 header bootloader need
CFLAGS+=-DLOAD_HEADER

# printf size optimization
CFLAGS+=-DPRINTF_SUPPORT_DECIMAL_SPECIFIERS=0 -DPRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS=0

include $(PROJECT_ROOT)/drivers/project.mk