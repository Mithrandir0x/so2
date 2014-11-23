#include <stdio.h>

int main()
{
    char c;
    signed char sc;
    unsigned char uc;

    // Oh boy, here comes the pain...

    short s;
    short int si;
    signed short ss;
    signed short int ssi;

    int i;
    signed int sint;

    long l;
    long int li;
    signed long sl;
    signed long int sli;

    unsigned long ul;
    unsigned long int uli;

    long long ll;
    long long int lli;
    signed long long sll;
    signed long long int slli;

    // What the actual fuck C!?!?!

    float f;

    double d;

    long double ld;

    // Here's how the shit's done:
    //
    //  Search: ^(.*)\s+([a-z])*;
    //
    // Replace: printf("%20s: %lu\\n", "$1", sizeof($2));

    printf("%32s: %lu\n", "char", sizeof(c));
    printf("%32s: %lu\n", "signed char", sizeof(sc));
    printf("%32s: %lu\n", "unsigned char", sizeof(uc));

    printf("%32s: %lu\n", "short", sizeof(s));
    printf("%32s: %lu\n", "short int", sizeof(si));
    printf("%32s: %lu\n", "signed short", sizeof(ss));
    printf("%32s: %lu\n", "signed short int", sizeof(ssi));

    printf("%32s: %lu\n", "int", sizeof(i));
    printf("%32s: %lu\n", "signed int", sizeof(sint));

    printf("%32s: %lu\n", "long", sizeof(l));
    printf("%32s: %lu\n", "long int", sizeof(li));
    printf("%32s: %lu\n", "signed long", sizeof(sl));
    printf("%32s: %lu\n", "signed long int", sizeof(sli));

    printf("%32s: %lu\n", "unsigned long", sizeof(ul));
    printf("%32s: %lu\n", "unsigned long int", sizeof(uli));

    printf("%32s: %lu\n", "long long", sizeof(ll));
    printf("%32s: %lu\n", "long long int", sizeof(lli));
    printf("%32s: %lu\n", "signed long long", sizeof(sll));
    printf("%32s: %lu\n", "signed long long int", sizeof(slli));

    printf("%32s: %lu\n", "float", sizeof(f));

    printf("%32s: %lu\n", "double", sizeof(d));
    printf("%32s: %lu\n", "long double", sizeof(ld));

    return 0;
}
