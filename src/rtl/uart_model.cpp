#include "Vuart_top.h"
#include "verilated.h"
#include "uart_model.h"
#include "uart_monitor.h"
#include "sim.h"

static int uart_state = 0;
static int bit_count = 0;
static uint8_t rx_byte = 0;
static int last_tx = 1;

static int counter = 0;


static Vuart_top *uart = nullptr;

#define BIT_TIME 1   // подобрать

void uart_monitor1()
{
    int tx = uart->stx_pad_o;

    switch(uart_state)
    {
        case 0: // WAIT START
            if(tx == 0)
            {
                printf("[RTL UART] WAIT\n");
                uart_state = 1;
                counter = BIT_TIME/2;
                bit_count = 0;
                rx_byte = 0;
            }

            break;

        case 1: // WAIT HALF BIT

            if(--counter == 0)
            {
                printf("[RTL UART] WAIT HALF\n");
                uart_state = 2;
                counter = BIT_TIME;
            }

            break;

        case 2: // READ DATA
            if(--counter == 0)
            {
                printf("[RTL UART] READ\n");
                rx_byte |= (tx << bit_count);
                bit_count++;

                counter = BIT_TIME;

                if(bit_count == 8)
                    uart_state = 3;
            }

            break;

        case 3: // STOP BIT

            if(--counter == 0)
            {
                if(tx == 1)
                    printf("[RTL] UART RX '%c'\n", rx_byte);
                else
                    printf("[RTL] framing error\n");

                uart_state = 0;
            }

            break;
    }
}

static void tick()
{
    uart->wb_clk_i = 0;
    uart->eval();

    uart->wb_clk_i = 1;
    uart->eval();

    if(last_tx == 1 && uart->stx_pad_o == 0)
        printf("[RTL] UART START BIT\n");


    last_tx = uart->stx_pad_o;
    // uart_monitor(uart);
    
}

void uart_tick()
{
    tick();          // один такт UART
    uart_monitor1();  // посмотреть TX линию
    uart_monitor(uart);
    // printf("cyc=%d stb=%d ack=%d\n",
    //    uart->wb_cyc_i,
    //    uart->wb_stb_i,
    //    uart->wb_ack_o);
    // printf("rst=%d clk=%d\n", uart->wb_rst_i, uart->wb_clk_i);
}

void uart_write_reg(uint32_t addr, uint8_t val)
{
    uint32_t word = addr;

    uart->wb_adr_i = word;
    uart->wb_dat_i = val;
    uart->wb_sel_i = 1;

    uart->wb_we_i  = 1;
    uart->wb_stb_i = 1;
    uart->wb_cyc_i = 1;

    printf("WB write addr=%u word=%u  val=%02x\n",
           addr, word,  val);

    
    while (!uart->wb_ack_o)
        uart_tick();

    uart->wb_stb_i = 0;
    uart->wb_cyc_i = 0;
    uart->wb_we_i  = 0;

    uart_tick(); 
    uart_tick();   // завершение
}

uint8_t uart_read_reg(uint32_t addr)
{
    uint32_t word = addr;

    uart->wb_adr_i = word;
    uart->wb_sel_i = 1 ;

    uart->wb_we_i  = 0;
    uart->wb_stb_i = 1;
    uart->wb_cyc_i = 1;

    
    while (!uart->wb_ack_o)
        uart_tick();

    uint32_t v = uart->wb_dat_o;

    uart->wb_stb_i = 0;
    uart->wb_cyc_i = 0;

    uart_tick();

    uint8_t val = (v) & 0xff;

    printf("WB read addr=%u word=%u val=%02x\n",
           addr, word, val);

    return val;
}

static void uart_tick_wrapper(void *opaque)
{
    uart_tick();
}

void uart_register_clock()
{

    printf("UART clock registered\n");

    clocked_device_t dev;
    dev.name = "uart";
    dev.tick = uart_tick_wrapper;
    dev.opaque = nullptr;
    dev.divider = 1; 

    scheduler_register(dev);
}

void uart_init()
{
    uart = new Vuart_top;

    uart->wb_clk_i = 0;
    uart->wb_rst_i = 1;

    uart->wb_adr_i = 0;
    uart->wb_dat_i = 0;
    uart->wb_we_i  = 0;
    uart->wb_stb_i = 0;
    uart->wb_cyc_i = 0;
    uart->wb_sel_i = 1 << 3;

    uart->srx_pad_i = 1;

    for(int i=0;i<10;i++)
        uart_tick();

    uart->wb_rst_i = 0;

    for(int i=0;i<10;i++)
        uart_tick();

    uart_write_reg(3, 0x80); // LCR
    uart_write_reg(0, 0x01); // DLL
    uart_write_reg(1, 0x00); // DLM
    
    uart_write_reg(3, 0x03); // 8N1

    for(int i=0;i<10;i++)
        uart_tick();
    printf("LCR=%02x\n", uart_read_reg(3));
}