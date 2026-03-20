volatile int x;

int main()
{   
    int i = 0;
    *(volatile int*)0x10000000 = 'A';
    x = 42;

    while (1) {
        i++;
        if (i == 2) {
            break;
        }
    }
    asm("ebreak");
    return 0;
}