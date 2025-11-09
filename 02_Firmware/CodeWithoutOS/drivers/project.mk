
include $(PROJECT_ROOT)/Drivers/gcc.mk
include $(PROJECT_ROOT)/Drivers/libsources.mk

BUILD_DIR =output

OBJS+=$(addprefix $(BUILD_DIR)/, $(notdir $(SRC_C:.c=.o)))
OBJS+=$(addprefix $(BUILD_DIR)/, $(notdir $(SRC_S:.S=.o))) 
OBJS+=$(addprefix $(BUILD_DIR)/, $(notdir $(SRC_CPP:.cpp=.o)))

CFLAGS+=-march=armv5te -mtune=arm926ej-s -mfloat-abi=soft -marm -mno-thumb-interwork -g -ggdb -Wall -fdata-sections -ffunction-sections -ffreestanding 
CFLAGS+=-D__ARM32_ARCH__=5 -D__ARM926EJS__
CFLAGS+=-DPRINTF_ALIAS_STANDARD_FUNCTION_NAMES=1 -DPRINTF_ALIAS_STANDARD_FUNCTION_NAMES_HARD=1
CFLAGS+=-fomit-frame-pointer -Wall -fverbose-asm -Os 
 
# DRAM size: 32M for F1C100s, 64M for F1C200s
DRAM_SIZE = 64M

LDFLAGS+=-Xlinker --gc-sections -Wl,--defsym=DRAM_SIZE=$(DRAM_SIZE),-Map=$(BUILD_DIR)/$(TARGET).map,--cref,--no-warn-mismatch

LIBS+= -lgcc -lm -lc_nano

ELF =$(BUILD_DIR)/$(TARGET).elf
BIN =$(BUILD_DIR)/$(TARGET).bin
DIS =$(BUILD_DIR)/$(TARGET).dis
vpath %.c $(sort $(dir $(SRC_C)))
vpath %.S $(sort $(dir $(SRC_S)))
vpath %.cpp $(sort $(dir $(SRC_CPP)))

MKSUNXI=mksunxi

.PHONY : all clean test download d

all: $(BUILD_DIR) $(OBJS) $(ELF) $(PROJECT_ROOT)/mksunxi/mksunxi $(BIN) 

$(BUILD_DIR):
	mkdir $@

$(PROJECT_ROOT)/mksunxi/mksunxi:
	@echo compiling mksunxi tool  
	@gcc $(PROJECT_ROOT)/mksunxi/mkboothead.c -o $@

download:$(BIN)
    ifeq ($(TARGET),bootloader)
		xfel spinor write 0 $(BIN)
    else
		xfel spinor write 0x10000 $(BIN)
    endif

$(BUILD_DIR)/%.o:%.c
	@echo CC .c $@  
	@$(CC) -c $(CFLAGS) $< $(INCLUDE) -o $@

$(BUILD_DIR)/%.o:%.cpp
	@echo CXX $@  
	@$(CXX) -c $(CFLAGS) $< $(INCLUDE) -o $@

$(BUILD_DIR)/%.o:%.S
	@echo AS $<
	@$(CC) $(CFLAGS) -c  $< $(INCLUDE) -o $@

$(ELF):$(OBJS)
	@echo linking $@
	@$(CC) $(LDFLAGS) $^ $(LIBS) -T$(LDSCRIPT) -o $@
	$(SZ) $@


$(BIN):$(ELF) 
	@echo cp $(BIN)
	@$(CP) -O binary -S $< $@ 
    ifeq ($(TARGET),bootloader)
		$(PROJECT_ROOT)/mksunxi/mksunxi $(BIN)
    endif
	@echo use "make dis" to get asm file in output dir. 
	

dis: $(DIS)
#make 使用tools中的make工具，msys的make报错 'no such file'
$(DIS):$(ELF) 
	@echo objdump    
	$(OD) -D $< > $@


clean:
	rm -rf $(BUILD_DIR)


test :$(BUILD_DIR)
	@echo .c= $(SRC_C)  
	@echo .S=$(SRC_S)
	@echo inc=$(INCLUDE)
	@echo objs=$(OBJS)
	@echo cflgs=$(CFLAGS)
	@echo ld=$(LDCRIPT)
	@echo ldflag=$(LDFLAGS)

 
d:
	make clean
	make all -j12 
	make dis
	make download
	