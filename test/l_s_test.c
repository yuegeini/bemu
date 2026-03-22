volatile int a = 0;

int main()
{
    a = 5;          // STORE
    int b = a;      // LOAD

    a = 42;         // STORE
    b = a;          // LOAD
    
    asm("ebreak");
    return 0;
}