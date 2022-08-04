#include "embedded.h"
#include "hal.h"
#include <stdio.h>

static uint32_t loop_count = 0;

uint32_t get_loop_count(void)
{
    return loop_count;
}

void embedded_setup(void)
{
    hal_init();
    printf("setup\n");

    loop_count = 0;
}

void embedded_loop(void)
{
    printf("loop %" PRIu32 "\n", loop_count);

    loop_count++;
}
