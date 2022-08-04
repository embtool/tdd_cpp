#ifndef EMBEDDED_H
#define EMBEDDED_H

#ifdef __cplusplus
extern "C"
{
#endif

/** @file embedded.h
 * @brief Embedded setup and loop functions.
 */

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

uint32_t get_loop_count(void);

void embedded_setup(void);
void embedded_loop(void);

#ifdef __cplusplus
}
#endif

#endif /* EMBEDDED_H */
