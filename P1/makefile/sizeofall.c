
#include <stdio.h>

int sizeofall()
{
    int a;
    float b;
    double c;
    char d;

    printf("   int: %lu\n", sizeof(a));
    printf(" float: %lu\n", sizeof(b));
    printf("double: %lu\n", sizeof(c));
    printf("  char: %lu\n", sizeof(d));

    return 0;
}
