#ifndef _RT_UTIL_H_
#define _RT_UTIL_H_

#include "regs.h"
#include <stddef.h>

#ifdef INTERNAL_STRACE
#define print_strace printf
#else
#define print_strace(...)
#endif


#define FATAL_DEBUG

int rt_util_getrandom(void* vaddr, size_t buflen);
void not_implemented_fatal(struct encl_ctx_t* ctx);
void rt_util_misc_fatal();

#endif /* _RT_UTIL_H_ */
