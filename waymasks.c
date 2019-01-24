#include "waymasks.h"

/* Waymasking is part of the dynamic way-allocation system.
 * 
 * A Way is in one of 3 global logical states (W_n):
 *  - Free (in use on any master using untrusted code) - F
 *  - Free-reserved (as Free, but may not be allocated in all circumstances) - FR
 *  - Allocated (in use for one specific enclave) - A
 * 
 * A way is in one of 2 states on each master (MW_n):
 *  - Granted (1 in mask, available to be used) - G
 *  - Locked (0 in mask, not available to be used) - L
 * 
 * A way 
 * 
 */

int waymask_allocate_ways(size_t n_ways, unsigned int target_core){

  waymask_t mask = 0;
  size_t remaining = _wm_choose_ways(n_ways, &mask, target_core);

  if(remaining  == n_ways){
    //fatal, should never happen
    return -1;
  }

  // Mark these ways as used by an enclave
  enclave_allocated_ways |= mask;
    

  // TODO intelligently handle having remaining > 0

  // Lockout/assign  masters from these ways
  unsigned int master;
  for(master=0;master<WM_NUM_MASTERS;master++){
    if(master == GET_CORE_WAY(target_core)){
      _wm_grant_ways(mask, master);
    }
    else{
      _wm_lockout_ways(mask, master);
    }
  }
  
}


int _wm_free_ways(waymask_t _mask){

  for(master=0; master < WM_NUM_MASTERS; master++){
    if(IS_MASTER_RUNNING_UNTRUSTED( master )){
      // Grant it
      _wm_grant_ways(mask, master);
    }
    // We don't enable new free ways on cores running trusted code
  }
  
}

// Simplest possible way choosing, with reserved way for each core
// Returns number of ways it couldn't allocate
int _wm_choose_ways_for_core(size_t n_ways, waymask_t* _mask, unsigned int target_core){
  
  waymask_t mask = 0;
  // Best effort, we may not be able to allocate n_ways
  if(n_ways == 0)
    return 0;
  
  // Always allocate our 'reserved' way if available
  if(! IS_WAY_ALLOCATED( GET_CORE_WAY(target_core) )){
    mask &= (0x1 << GET_CORE_WAY(target_core));
    n_ways--;
  }

  // Find ways to allocate as available, ignoring reserved core ways
  int way = WM_MIN_FREE_WAY
  while(n_ways > 0 && way < WM_MAX_FREE_WAY){
    if( !IS_WAY_ALLOCATED(way) ){
      mask |= (0x1 << way);
      n_ways--;
    }
    way++;
  }

  *_mask = mask;

  // Return number of UNallocated ways
  return n_ways;
}

// This will DISABLE the given ways
int _wm_lockout_ways(waymask_t mask, unsigned int master){

  if(master > WM_NUM_MASTERS){
    return  -1;
  }
  
  //Note that we DO allow entirely locking out a master
  // Supposedly this isn't allowed, we'll see
  // "At least one cache way must be enabled. "
  waymask_t* master_mask = WM_REG_ADDR(master);

  *master_mask &= WM_FLIP_MASK(mask);
  return 0;
}

// This will GRANT ACCESS to the given ways
int _wm_grant_ways(waymask_t mask, unsigned int master){

  if(master > WM_NUM_MASTERS){
    return  -1;
  }
  
  waymask_t* master_mask = WM_REG_ADDR(master);

  *master_mask |= mask;
  return 0;
}

// Just hard set ways for a master
int _wm_assign_mask(waymask_t mask, unsigned int master){

  if(master > WM_NUM_MASTERS){
    return  -1;
  }
  
  waymask_t* master_mask = WM_REG_ADDR(master);

  *master_mask = mask;
  return 0;
}

void waymask_init(){
  enclave_allocated_ways = 0;
}
