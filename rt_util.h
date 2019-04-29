#ifndef _RT_UTIL_H_
#define _RT_UTIL_H_

#include "regs.h"
#include <stddef.h>

#ifdef INTERNAL_STRACE
#define print_strace printf
#else
#define print_strace(...)
#endif

#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })

#define write_csr(reg, val) ({ \
  asm volatile ("csrw " #reg ", %0" :: "rK"(val)); })

#define swap_csr(reg, val) ({ unsigned long __tmp; \
  asm volatile ("csrrw %0, " #reg ", %1" : "=r"(__tmp) : "rK"(val)); \
  __tmp; })

#define set_csr(reg, bit) ({ unsigned long __tmp; \
  asm volatile ("csrrs %0, " #reg ", %1" : "=r"(__tmp) : "rK"(bit)); \
  __tmp; })

#define clear_csr(reg, bit) ({ unsigned long __tmp; \
  asm volatile ("csrrc %0, " #reg ", %1" : "=r"(__tmp) : "rK"(bit)); \
  __tmp; })


#define FATAL_DEBUG

int rt_util_getrandom(void* vaddr, size_t buflen);
void not_implemented_fatal(struct encl_ctx_t* ctx);
void rt_util_misc_fatal();

void breakonme();

#endif /* _RT_UTIL_H_ */
