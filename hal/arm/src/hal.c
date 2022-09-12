/*
 * Simulation on QEMU:
 * https://www.qemu.org/docs/master/system/target-arm.html
 *
 * qemu-system-arm -machine stm32vldiscovery -nographic -semihosting -kernel main.bin -S -s
 * gdb-multiarch -ex 'target remote :1234' main.elf
 *
 * New-lib sycalls:
 *
 * https://sourceware.org/newlib/libc.html
 */

#include "hal.h"
#include <stdint.h>
#include <stdio.h>

#ifndef F_UART0
    #define F_UART0 9600UL
#endif

/*
 * Linker symbols.
 */

#define SECTION(x) __attribute__((section(x)))
#define ALIAS(x) __attribute__((weak, alias(x)))
#define WEAK __attribute__((weak))
#define NAKED __attribute__((naked))

/* Defined by the linker. */

extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __end__;
extern uint32_t __HeapLimit;
extern uint32_t __StackLimit;
extern uint32_t __StackTop;

/* User-defined heap and stack size. */

SECTION(".heap") uint8_t _heap_buffer[2 * 1024];
SECTION(".stack") uint8_t _stack_buffer[2 * 1024];

/*
 * Print to UART1.
 */

const uint32_t USART1_BASE = 0x40013800;
volatile uint32_t *const USART1_SR = (uint32_t *)(USART1_BASE + 0x00);
const uint32_t TXE = 7;
const uint32_t RXNE = 5;
volatile uint32_t *const USART1_DR = (uint32_t *)(USART1_BASE + 0x04);

void uart0_init(uint32_t baud_rate)
{
    /* TODO Not implemented. */
    (void)baud_rate;
}

void uart0_send(uint8_t ch)
{
    while (!(*USART1_SR & (1 << TXE)))
    {
        /* Wait TX buffer to be emtpy */
    }

    *USART1_DR = ch;
}

int16_t uart0_recv(void)
{
    if (!(*USART1_SR & (1 << RXNE)))
    {
        /* Nothing received */
        return -1;
    }

    return *USART1_DR & 0xFF;
}

/*
 * HAL functions.
 */

void hal_init(void)
{
    uart0_init(F_UART0);
}

/*
 * Newlib syscalls.
 */

#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>

#undef errno
extern int errno;

int _write(int file, char *ptr, int len);
int _read(int file, char *ptr, int len);
void *_sbrk(int incr);
void _exit(int status);
int _open(const char *name, int flags, int mode);
int _close(int file);
int _fstat(int file, struct stat *st);
int _isatty(int file);
int _lseek(int file, int ptr, int dir);
int _kill(int pid, int sig);
int _getpid(void);
int _gettimeofday(struct timeval *tp, struct timezone *tz);

int _write(int file, char *ptr, int len)
{
    int i;
    (void)file;

    for (i = 0; i < len; i++)
    {
        uart0_send(*ptr++);
    }

    return len;
}

int _read(int file, char *ptr, int len)
{
    int n = 0;
    (void)file;

    while (len > 0)
    {
        int16_t val = uart0_recv();
        if (val < 0)
            break;

        *ptr++ = val;
        ++n;
        --len;
    }

    return n;
}

static void heap_no_mem(void)
{
    errno = ENOMEM;

#if 1
    /* User may choose not to abort here. */
    /* abort(); */
    _exit(1);
#endif
}

void *_sbrk(int incr)
{
    static uint8_t *heap_end = (uint8_t *)&__end__;
    uint8_t *prev_heap_end;
    register uint8_t *stack_pointer __asm("sp");

    prev_heap_end = heap_end;

    if (heap_end + incr > (uint8_t *)&__HeapLimit)
    {
        _write(1, "Heap limit exceeded\n", 20);
        heap_no_mem();
        return (uint8_t *)-1;
    }
    else if (heap_end + incr > stack_pointer)
    {
        _write(1, "Heap and stack collision\n", 25);
        heap_no_mem();
        return (uint8_t *)-1;
    }

    heap_end += incr;

    return prev_heap_end;
}

void _exit(int status)
{
    (void)status;
    while (1)
    {
    }
}

int _open(const char *name, int flags, int mode)
{
    (void)name;
    (void)flags;
    (void)mode;
    return -1;
}

int _close(int file)
{
    (void)file;
    return -1;
}

int _fstat(int file, struct stat *st)
{
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    (void)file;
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

int _getpid(void)
{
    return 1;
}

int _gettimeofday(struct timeval *tp, struct timezone *tz)
{
    (void)tp;
    (void)tz;
    return -1;
}

/*
 * Vector table
 */

extern void __libc_init_array(void);
extern int main(int argc, char **argv);

void Reset_Handler(void);
void Default_Hadler(void);
void Semihosting_Exit(void);
void NMI_Handler(void) ALIAS("Default_Hadler");
void HardFault_Handler(void) ALIAS("Default_Hadler");
void MemManage_Handler(void) ALIAS("Default_Hadler");
void BusFault_Handler(void) ALIAS("Default_Hadler");
void UsageFault_Handler(void) ALIAS("Default_Hadler");
void SVC_Handler(void) ALIAS("Default_Hadler");
void DebugMonitor_Handler(void) ALIAS("Default_Hadler");
void PendSV_Handler(void) ALIAS("Default_Hadler");
void SysTick_Handler(void) ALIAS("Default_Hadler");

const uint32_t vectors[] SECTION(".isr_vector") = {
    (uint32_t)&__StackTop,
    (uint32_t)&Reset_Handler,
    (uint32_t)&NMI_Handler,
    (uint32_t)&HardFault_Handler,
    (uint32_t)&MemManage_Handler,
    (uint32_t)&BusFault_Handler,
    (uint32_t)&UsageFault_Handler,
    0,
    0,
    0,
    0,
    (uint32_t)&SVC_Handler,
    (uint32_t)&DebugMonitor_Handler,
    0,
    (uint32_t)&PendSV_Handler,
    (uint32_t)&SysTick_Handler,
};

NAKED void Reset_Handler(void)
{
    uint32_t size;
    uint32_t i;
    uint32_t *dest;
    uint32_t *src;

    /* Copy data section */
    src = &__etext;
    dest = &__data_start__;
    size = &__data_end__ - &__data_start__;
    for (i = 0; i < size; ++i)
        *dest++ = *src++;

    /* Zero bss section */
    dest = &__bss_start__;
    size = &__bss_end__ - &__bss_start__;
    for (i = 0; i < size; ++i)
        *dest++ = 0;

    /* Initialize standard library */
    __libc_init_array();

    main(0, NULL);

    Semihosting_Exit();

    while (1)
    {
    }
}

void Semihosting_Exit(void)
{
    /* r0 = 0x18 - angel_SWIreason_ReportException */
    __asm volatile("mov r0, #0x18");

    /* r1 = 0x20026 - ADP_Stopped_ApplicationExit */
    __asm volatile(
        "mov r1, #0x20000 \n\t"
        "orr r1, #0x26");

    /* Semihosting call */
    __asm volatile("bkpt 0xAB");
    /* __asm volatile("svc 0xAB"); */
    /* __asm volatile("svc 0x00123456"); */
}

NAKED void Default_Hadler(void)
{
    while (1)
    {
    }
}
