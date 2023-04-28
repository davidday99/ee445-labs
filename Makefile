PROJECT = main
DEV = /dev/ttyACM0
FLASHER = lm4flash
SRCS = $(wildcard src/*.c) \
	   $(wildcard src/**/*.c) \
	   $(wildcard src/*.s) \
	   $(wildcard src/**/*.s) \
	   $(wildcard src/*.S) \
	   $(wildcard src/**/*.S)

OBJ = obj/
OBJS = $(addprefix $(OBJ),$(filter-out %.c %.S,$(notdir $(SRCS:.s=.o))) $(filter-out %.s %.S,$(notdir $(SRCS:.c=.o))) $(filter-out %.c %.s,$(notdir $(SRCS:.S=.o))))
INC = inc/
LD_SCRIPT = TM4C123GH6PM.ld 

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld 
DEBUGGER = arm-none-eabi-gdb
OBJCOPY = arm-none-eabi-objcopy
RM = rm -rf
MKDIR = @mkdir -p $(@D)

CFLAGS = -ggdb3 -mcpu=cortex-m4 -mfloat-abi=soft 
CFLAGS += -nostdlib -nostartfiles -MD -std=c99 -Wextra -Wall -Wno-missing-braces
DEPFLAGS = -MT $@ -MMD -MP

all: bin/$(PROJECT).elf

$(OBJ)%.o: src/%.c        
	$(MKDIR)             
	$(CC) -o $@ $< -c -I$(INC) $(CFLAGS) $(DEPFLAGS)

$(OBJ)%.o: src/**/%.c        
	$(MKDIR)             
	$(CC) -o $@ $< -c -I$(INC) $(CFLAGS) $(DEPFLAGS)
	
$(OBJ)%.o: src/%.s       
	$(MKDIR)             
	$(CC) -o $@ $< -c -I$(INC) $(CFLAGS) $(DEPFLAGS)

$(OBJ)%.o: src/**/%.s       
	$(MKDIR)             
	$(CC) -o $@ $< -c -I$(INC) $(CFLAGS) $(DEPFLAGS)

$(OBJ)%.o: src/%.S       
	$(MKDIR)             
	$(CC) -o $@ $< -c -I$(INC) $(CFLAGS) $(DEPFLAGS)

$(OBJ)%.o: src/**/%.S       
	$(MKDIR)             
	$(CC) -o $@ $< -c -I$(INC) $(CFLAGS) $(DEPFLAGS)

bin/$(PROJECT).elf: $(OBJS) 
	$(MKDIR)           
	$(CC) -o $@ $^ $(CFLAGS) $(DEPFLAGS) -Wl,-T$(LD_SCRIPT) -Wl,-eReset_Handler
	$(OBJCOPY) -O binary $@ bin/$(PROJECT).bin 

flash:
	$(FLASHER) -S $(DEV) bin/$(PROJECT).bin

debug:
	$(DEBUGGER) --tui bin/$(PROJECT).elf -ex "target remote :3333" -ex "monitor reset halt"

-include $(OBJS:.o=.d)

clean:
	-$(RM) obj
	-$(RM) bin

