#include "hal.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * STDOUT and STDERR write to serial interface (UART0).
 */

static void uart_init(void)
{
    float F_CPU = 16000000;
    float UART_BAUD_RATE = 115200;
    float UART_BAUD_REGISTERS = (((F_CPU / (16.0 * UART_BAUD_RATE))) - 1.0);

    UCSR0B = (1 << TXEN0);
    UBRR0 = UART_BAUD_REGISTERS;
}

int fputc(int ch, FILE *stream)
{
    (void)stream;

    while ((UCSR0A & (1 << UDRE0)) == 0)
    {
    }

    UDR0 = ch;

    return ch;
}

/*
 * HAL functions.
 */

void hal_init(void)
{
    uart_init();
}

/*
 * https://qemu-project.gitlab.io/qemu/system/target-avr.html
 *
 * qemu-system-avr -machine uno -bios build/main.elf -serial
 * tcp::5678,server=on,wait=on -s -S
 *
 * telnet localhost 5678
 *
 * gdb build/main.elf
 * target remote :1234
 */
