#define UART_DATA   (*(volatile char*)0x10000000)
#define UART_STATUS (*(volatile int*)0x10000004)

int main()
{
    while (!(UART_STATUS & (1<<5)))
        ;

    UART_DATA = 'A';

    asm("ebreak");
    return 0;
}