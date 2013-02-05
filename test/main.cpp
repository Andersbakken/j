#include "lib.h"
#include <stdio.h>

int main()
{
    extern void balle();
    balle();
    printf("%d\n", libfunc());
    return 0;
}
