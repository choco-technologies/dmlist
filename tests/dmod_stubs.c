/**
 * @file dmod_stubs.c
 * @brief DMOD stubs for testing
 */

#include "dmod.h"

// These symbols are normally defined by the linker script
// For tests, we define them as empty pointers
void* __dmod_inputs_start __attribute__((weak)) = NULL;
size_t __dmod_inputs_size __attribute__((weak)) = 0;
void* __dmod_outputs_start __attribute__((weak)) = NULL;
size_t __dmod_outputs_size __attribute__((weak)) = 0;
