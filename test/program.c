#define UART ((volatile unsigned char *)0x10000000)

void delay()
{
    for (volatile int i = 0; i < 1000; i++);
}

void uart_wait_tx()
{
    while (!(UART[5] & 0x20)); // LSR bit 5 = THR empty
}

void uart_wait_rx()
{
    while (!(UART[5] & 0x01)); // LSR bit 0 = data ready
}

void uart_putc(char c)
{
    UART[0] = c;
}

char uart_getc()
{
    uart_wait_rx();
    return UART[0];
}

int main()
{
    // --- init ---
    UART[3] = 0x80; // DLAB=1
    UART[0] = 0x02; // DLL
    UART[1] = 0x00; // DLM
    UART[3] = 0x03; // 8N1

    // --- test ---
    char c;

    
    UART[0] = 'A';

    c = uart_getc();   // ждём ввод
    uart_putc(c);      // отправляем обратно

    asm("ebreak");
}