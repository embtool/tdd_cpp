#include "embedded.h"
#include <stdio.h>

int main(void)
{
    printf("main begin\n");

    embedded_setup();

    for (;;)
    {
        embedded_loop();
    }

    printf("main end\n");

    return 0;
}
