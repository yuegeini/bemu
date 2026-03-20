CC = gcc
CFLAGS = -Iinclude -O2

BUILD = build
SRC = src

SRCS = \
$(SRC)/main.c \
$(SRC)/debugger.c \
$(SRC)/cpu/cpu.c \
$(SRC)/cpu/decode.c \
$(SRC)/cpu/memory.c \
$(SRC)/devices/bus.c \
$(SRC)/devices/ram.c \
$(SRC)/devices/uart.c

OBJS = $(SRCS:$(SRC)/%.c=$(BUILD)/%.o)

TARGET = emu

all: clean program $(TARGET)
	./emu

program:
	riscv64-unknown-elf-gcc test/program.c -nostdlib -march=rv32i -mabi=ilp32 -Ttext=0x0 -o program.elf
	riscv64-unknown-elf-objcopy -O binary program.elf program.bin

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

$(BUILD)/%.o: $(SRC)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD) $(TARGET)
	rm -rf *.bin
	rm -rf *.elf

