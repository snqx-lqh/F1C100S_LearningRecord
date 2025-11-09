
CROSS_COMPILE ?=arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
CXX=$(CROSS_COMPILE)g++
AS=$(CROSS_COMPILE)gcc -x assembler-with-cpp
CP=$(CROSS_COMPILE)objcopy
SZ=$(CROSS_COMPILE)size
OD=$(CROSS_COMPILE)objdump
BIN=$(CP) -O binary -S
