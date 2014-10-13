#include <stdio.h>

int main(int argc, char **argv)
{
    // I would love a world where I can use blocks in GCC...

    int n = 3;

    void (^hello)(void) = ^(void) {
        printf("Hello, block! [%d]\n", n);
    };
    
    hello();
    
    return 0;
}
