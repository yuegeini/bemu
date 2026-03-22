#define UART ((volatile char*)0x10000000)

int main()
{
    int a = 5;
    int b = 7;
    int c = a + b;

    if (c == 12)
        *UART = 'P';
    else
        *UART = 'F';

    asm("ebreak");
    return 0;
}