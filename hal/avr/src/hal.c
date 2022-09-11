/*
 * Simulation on QEMU:
 * https://qemu-project.gitlab.io/qemu/system/target-avr.html
 *
 * qemu-system-avr -machine mega2560 -nographic -bios main.bin -S -s
 * avr-gdb -ex 'target remote :1234' main.elf
 */

#include "hal.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

#ifndef F_CPU
    #define F_CPU 160000000UL
#endif

#ifndef F_UART0
    #define F_UART0 9600UL
#endif

int uart0_fputc(char ch, FILE *stream);

FILE uart0_tx = FDEV_SETUP_STREAM(&uart0_fputc, NULL, _FDEV_SETUP_RW);

void uart0_init(uint32_t baud_rate)
{
    /* Enable trasmit */
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

    /* 8 bit character */
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);

    /* Baud rate */
    UBRR0 = (F_CPU + 8 * baud_rate) / (16 * baud_rate) - 1;

    stdout = &uart0_tx;
}

void uart0_send(uint8_t ch)
{
    while (!(UCSR0A & (1 << UDRE0)))
    {
        /* Wait TX buffer to be emtpy */
    }

    UDR0 = ch;
}

int16_t uart0_recv(void)
{
    if (!(UCSR0A & (1 << RXC0)))
    {
        /* Nothing received */
        return -1;
    }

    return UDR0;
}

int uart0_fputc(char ch, FILE *stream)
{
    (void)stream;
    uart0_send(ch);
    return 0;
}

/*
 * HAL functions.
 */

void hal_init(void)
{
    uart0_init(F_UART0);
}
