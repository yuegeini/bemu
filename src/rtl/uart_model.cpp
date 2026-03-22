#include "Vuart_top.h"
#include "verilated.h"
#include "uart_model.h"

static Vuart_top *uart = nullptr;

static void tick()
{
    uart->wb_clk_i = 0;
    uart->eval();

    uart->wb_clk_i = 1;
    uart->eval();
}

void uart_init()
{
    uart = new Vuart_top;

    uart->wb_rst_i = 1;

    for(int i=0;i<10;i++)
        tick();

    uart->wb_rst_i = 0;
}

void uart_write_reg(uint32_t addr, uint8_t val)
{
    uart->wb_adr_i = addr;
    uart->wb_dat_i = val;
    uart->wb_we_i  = 1;
    uart->wb_stb_i = 1;
    uart->wb_cyc_i = 1;

    tick();

    uart->wb_stb_i = 0;
    uart->wb_cyc_i = 0;
}

uint8_t uart_read_reg(uint32_t addr)
{
    uart->wb_adr_i = addr;
    uart->wb_we_i  = 0;
    uart->wb_stb_i = 1;
    uart->wb_cyc_i = 1;

    tick();

    uint8_t val = uart->wb_dat_o;

    uart->wb_stb_i = 0;
    uart->wb_cyc_i = 0;

    return val;
}