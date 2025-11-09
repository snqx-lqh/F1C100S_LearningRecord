 
SRC_C+=$(wildcard $(PROJECT_ROOT)/drivers/src/*.c) 

SRC_C+=$(wildcard $(PROJECT_ROOT)/drivers/display_gfx/*.c) 

SRC_S+=$(wildcard $(PROJECT_ROOT)/drivers/arm926/src/*.S)
 

INCLUDE+=\
-I$(PROJECT_ROOT)/drivers/arm926/inc \
-I$(PROJECT_ROOT)/drivers/display_gfx \
-I$(PROJECT_ROOT)/drivers/inc 