/*
 * Simulation on QEMU:
 * https://www.qemu.org/docs/master/system/target-arm.html
 *
 * cmake .. --toolchain ../cmake/toolchains/arm.cmake -DCPPUTEST_COMPILE_TESTS=OFF -DMACHINE_CPU="-mcpu=cortex-m3" -DLINKER_SCRIPT="../linker/cortex-m3.ld"
 * qemu-system-arm -machine stm32vldiscovery -nographic -semihosting -kernel main.bin -S -s
 *
 * cmake .. --toolchain ../cmake/toolchains/arm.cmake -DMACHINE_CPU="-mcpu=arm926ej-s" -DLINKER_SCRIPT="../linker/arm926ej-s.ld"
 * qemu-system-arm -machine versatilepb -m 128M -nographic -semihosting -kernel main.bin -S -s
 *
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

#if (__ARM_ARCH == 7)
/* Cortex-M3 -mcpu=cortex-m3 */
SECTION(".heap") uint8_t _heap_buffer[2 * 1024];
SECTION(".stack") uint8_t _stack_buffer[2 * 1024];

#elif (__ARM_ARCH == 5)
/* ARM926EJ-S -mcpu=arm926ej-s */
SECTION(".heap") uint8_t _heap_buffer[64 * 1024];
SECTION(".stack") uint8_t _stack_buffer[64 * 1024];

#endif

/*
 * Print to UART.
 */

#if (__ARM_ARCH == 7)
/* Cortex-M3 -mcpu=cortex-m3 */

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

#elif (__ARM_ARCH == 5)
/* ARM926EJ-S -mcpu=arm926ej-s */

const uint32_t UART0_BASE = 0x101F1000;
volatile uint32_t *const UART0DR = (uint32_t *)(UART0_BASE + 0x00);
volatile uint32_t *const UART0FR = (uint32_t *)(UART0_BASE + 0x18);
const uint32_t TXFF = 5;
const uint32_t RXFE = 4;

void uart0_init(uint32_t baud_rate)
{
    /* TODO Not implemented. */
    (void)baud_rate;
}

void uart0_send(uint8_t ch)
{
    while (*UART0FR & (1 << TXFF))
    {
        /* Wait TX buffer to be emtpy */
    }

    *UART0DR = ch;
}

int16_t uart0_recv(void)
{
    if (*UART0FR & (1 << RXFE))
    {
        /* Nothing received */
        return -1;
    }

    return *UART0DR & 0xFF;
}

#endif /* __ARM_ARCH */

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
void Default_Handler(void);
void Semihosting_Exit(void);

#if (__ARM_ARCH == 7)
/* Cortex-M3 -mcpu=cortex-m3 */

void NMI_Handler(void) ALIAS("Default_Handler");
void HardFault_Handler(void) ALIAS("Default_Handler");
void MemManage_Handler(void) ALIAS("Default_Handler");
void BusFault_Handler(void) ALIAS("Default_Handler");
void UsageFault_Handler(void) ALIAS("Default_Handler");
void SVC_Handler(void) ALIAS("Default_Handler");
void DebugMonitor_Handler(void) ALIAS("Default_Handler");
void PendSV_Handler(void) ALIAS("Default_Handler");
void SysTick_Handler(void) ALIAS("Default_Handler");

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

#elif (__ARM_ARCH == 5)
/* ARM926EJ-S -mcpu=arm926ej-s */

void Undefined_Handler(void) ALIAS("Default_Handler");
void SWI_Handler(void) ALIAS("Default_Handler");
void PrefetchAbort_Handler(void) ALIAS("Default_Handler");
void DataAbort_Handler(void) ALIAS("Default_Handler");
void IRQ_Handler(void) ALIAS("Default_Handler");
void FIQ_Handler(void) ALIAS("Default_Handler");

NAKED SECTION(".isr_vector") void vectors(void)
{
    __asm volatile(
        "B Reset_Handler         \n\t"
        "B Undefined_Handler     \n\t"
        "B SWI_Handler           \n\t"
        "B PrefetchAbort_Handler \n\t"
        "B DataAbort_Handler     \n\t"
        "B .                     \n\t" /* reserved */
        "B IRQ_Handler           \n\t"
        "B FIQ_Handler           \n\t");
}

#endif /* __ARM_ARCH */

NAKED void Reset_Handler(void)
{
    uint32_t size;
    uint32_t i;
    uint32_t *dest;
    uint32_t *src;

#if (__ARM_ARCH == 7)
    /* Cortex-M3 -mcpu=cortex-m3
     * The stack pointer is defined in vectors[0]. The processor loads
     * the stack pointer and jumps to the reset handler defined in
     * vectors[1].
     */
#elif (__ARM_ARCH == 5)
    /* ARM926EJ-S -mcpu=arm926ej-s
     * The processor executes the instruction defined in vectors[0],
     * which jumps to the reset handler. The stack pointer must be
     * explicitly set in the reset handler.
     */
    __asm volatile("LDR sp, =__StackTop" ::: "memory");
#endif

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
#if (__ARM_ARCH == 7)
    /* Cortex-M3 -mcpu=cortex-m3 */
    __asm volatile("bkpt 0xAB");
#elif (__ARM_ARCH == 5)
    /* ARM926EJ-S -mcpu=arm926ej-s */
    __asm volatile("svc 0x00123456");
#else
    /* Unknown */
    __asm volatile("svc 0xAB");
#endif
}

NAKED void Default_Handler(void)
{
    while (1)
    {
    }
}

/* Defined to allow building for ARM926EJ-S. */
void __sync_synchronize()
{
}
