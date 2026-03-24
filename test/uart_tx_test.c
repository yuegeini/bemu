#define uart ((volatile unsigned char *)0x10000000)

void uart_putc(char c)
{
    uart[0] = c;
}

void uart_print_hex(unsigned char v)
{
    const char *hex = "0123456789ABCDEF";

    uart_putc(hex[(v >> 4) & 0xF]);
    uart_putc(hex[v & 0xF]);
}

void uart_print_str(const char *s)
{
    while (*s)
        uart_putc(*s++);
}

int main()
{
    unsigned char v;

    uart_print_str("UART TEST\n");

    // LCR test
    uart[3] = 0x80;
    v = uart[3];

    uart_print_str("LCR=");
    uart_print_hex(v);
    uart_putc('\n');

    uart[0] = 0x12;
    uart[1] = 0x34;

    uart_print_str("DLL=");
    uart_print_hex(uart[0]);
    uart_putc('\n');

    uart_print_str("DLM=");
    uart_print_hex(uart[1]);
    uart_putc('\n');

    uart[3] = 0x03;

    uart_print_str("LCR=");
    uart_print_hex(uart[3]);
    uart_putc('\n');

    uart_print_str("LSR=");
    uart_print_hex(uart[5]);
    uart_putc('\n');

    uart_print_str("SEND\n");

    uart_putc('A');
    uart_putc('B');
    uart_putc('C');
    uart_putc('\n');

    asm("ebreak");
    return 0;
}
