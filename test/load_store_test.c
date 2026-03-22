#define UART ((volatile char*)0x10000000)

volatile int mem;

int main()
{
    mem = 1234;

    int x = mem;

    if (x == 1234)
        *UART = 'P';
    else
        *UART = 'F';

    asm("ebreak");
    return 0;
}