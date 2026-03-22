#define UART_BASE 0x10000000

volatile unsigned char *uart = (void*)UART_BASE;

static char hex[] = "2";


int main()
{
    unsigned char lcr;

    uart[3] = 0x03;   // 8N1

    lcr = uart[3];    // READ LCR


    asm("ebreak");
    return 0;
}