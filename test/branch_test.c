#define UART ((volatile char*)0x10000000)

int main()
{
    int a = 5;
    int b = 5;

    if (a == b)
        *UART = 'P';
    else
        *UART = 'F';

    asm("ebreak");
    return 0;
}