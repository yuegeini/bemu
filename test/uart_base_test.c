volatile unsigned int x;

int main()
{
    x = 0x11223344;

    unsigned char *p = (unsigned char*)&x;

    unsigned char a = p[0];
    unsigned char b = p[1];
    unsigned char c = p[2];
    unsigned char d = p[3];

    asm("ebreak");
    return 0;
}