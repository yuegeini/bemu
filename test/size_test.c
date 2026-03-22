volatile unsigned char  b8;
volatile unsigned short b16;
volatile unsigned int   b32;

int main()
{
    b8 = 0x11;
    b16 = 0x2233;
    b32 = 0x44556677;

    unsigned char  r8  = b8;
    unsigned short r16 = b16;
    unsigned int   r32 = b32;

    asm("ebreak");
    return 0;
}