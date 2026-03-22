#define UART ((volatile char*)0x00100000)

int main()
{
    *UART = 'A';
    *UART = 'B';
    *UART = 'C';

    asm("ebreak");
    return 0;
}