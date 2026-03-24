CC = g++
CFLAGS = -Iinclude -Iobj_dir -I/usr/share/verilator/include -O2

BUILD = build
SRC = src

VERILATOR = verilator
UART_RTL = rtl/uart_top.v

UART_VERILATED = obj_dir/Vuart_top__ALL.cpp

VERILATOR_LIB = \
/usr/share/verilator/include/verilated.cpp \
/usr/share/verilator/include/verilated_threads.cpp

SRCS = \
$(SRC)/main.c \
$(SRC)/sim/sim.c \
$(SRC)/sim/scheduler.c \
$(SRC)/cosim/transaction.c \
$(SRC)/cosim/uart_ref.c \
$(SRC)/cosim/uart_rx.cpp \
$(SRC)/cosim/uart_rtl.cpp \
$(SRC)/rtl/uart_model.cpp \
$(SRC)/rtl/uart_monitor.c \
$(SRC)/common/debugger.c \
$(SRC)/common/trace.c \
$(SRC)/cpu/cpu.c \
$(SRC)/cpu/decode.c \
$(SRC)/cpu/memory.c \
$(SRC)/devices/bus.c \
$(SRC)/devices/ram.c \
$(SRC)/devices/uart.c

OBJS = $(SRCS:$(SRC)/%.c=$(BUILD)/%.o)
OBJS := $(OBJS:$(SRC)/%.cpp=$(BUILD)/%.o)

TARGET = emu

all: clean verilated program $(TARGET)
	./emu

run: program $(TARGET)
	./emu

program:
	riscv64-unknown-elf-gcc start.S test/program.c  -nostdlib -march=rv32i -mabi=ilp32 -Ttext=0x0 -o program.elf
	riscv64-unknown-elf-objcopy -O binary program.elf program.bin

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(UART_VERILATED) $(VERILATOR_LIB) -o $@

$(BUILD)/%.o: $(SRC)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: $(SRC)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

verilated:
	$(VERILATOR) \
	    --cc $(UART_RTL) \
	    --top-module uart_top \
	    -Irtl \
	    --no-timing \
	    -Wno-fatal \
		--build

clean:
	rm -rf $(BUILD) $(TARGET)
	rm -rf obj_dir
	rm -rf *.bin *.elf