#define UART ((volatile char*)0x10000000)

volatile unsigned char b;
volatile unsigned short h;

int main()
{
    b = 0x11;
    h = 0x2233;

    unsigned char x = b;
    unsigned short y = h;

    if (x == 0x11 && y == 0x2233)
        *UART = 'P';
    else
        *UART = 'F';

    asm("ebreak");
    return 0;
}