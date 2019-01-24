#ifndef _WAYMASKS_H_
#define _WAYMASKS_H_

#define WM_NUM_MASTERS 21
#define WM_NUM_WAYS 16
#define WM_NUM_CORES 4

#define WM_MIN_FREE_WAY WM_NUM_CORES
#define WM_MAX_FREE_WAY WM_NUM_WAYS-2 /* We allocate 1 way for all non core masters */

typedef uint64_t waymask_t;

waymask_t enclave_allocated_ways;

// Waymask master IDs
#define WM_Core_0_DCache_MMIO           0
#define WM_Core_0_ICache                1
#define WM_Core_1_DCache                2
#define WM_Core_1_ICache                3
#define WM_Core_2_DCache                4
#define WM_Core_2_ICache                5
#define WM_Core_3_DCache                6
#define WM_Core_3_ICache                7
#define WM_Core_4_DCache                8
#define WM_Core_4_ICache                9
#define WM_DMA                          10
#define WM_Chiplink_Domain_1_7_Prefetch 11
#define WM_ChipLink_Domain_0            12
#define WM_ChipLink_Domain_1            13
#define WM_ChipLink_Domain_2            14
#define WM_ChipLink_Domain_3            15
#define WM_ChipLink_Domain_4            16
#define WM_ChipLink_Domain_5            17
#define WM_ChipLink_Domain_6            18
#define WM_ChipLink_Domain_7            19
#define WM_GEMGXL_ID0                   20

// Registers are 8 bytes apart, starting at 0x800
#define WM_REG_ADDR(id) (0x800+(8*id))

#define WM_FLIP_MASK(mask) (!mask & 0xFFFF)

#define IS_WAY_ALLOCATED( waynum ) ( enclave_allocated_ways & (0x1 << waynum))

#define IS_MASTER_RUNNING_UNTRUSTED( master ) (/* TODO */ 0)

//NOTE: We ignore core0, its a special core and is disabled.
#define GET_CORE_WAY( corenum ) ( corenum - 1 )

int waymask_allocate_ways(size_t n_ways, unsigned int target_core);
int _wm_choose_ways_for_core(size_t n_ways, waymask_t* _mask, unsigned int target_core);
int _wm_lock_ways(waymask_t mask, unsigned int core);
int _wm_grant_ways(waymask_t mask, unsigned int core);

#endif /* _WAYMASKS_H_
