#define UART ((volatile char*)0x10000000)

int main()
{
    int i = 0;
    int sum = 0;

    while (i < 5)
    {
        sum += i;
        i++;
    }

    if (sum == 10)
        *UART = 'P';
    else
        *UART = 'F';

    asm("ebreak");
    return 0;
}