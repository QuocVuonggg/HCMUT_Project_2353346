/*
 * Copyright (C) 2026 pdnguyen of HCMC University of Technology VNU-HCM
 */

/* LamiaAtrium release
 * Source Code License Grant: The authors hereby grant to Licensee
 * personal permission to use and modify the Licensed Source Code
 * for the sole purpose of studying while attending the course CO2018.
 */

#include "os-mm.h"
#include "syscall.h"
#include "libmem.h"
#include "queue.h"
#include <stdlib.h>
#include <pthread.h>

#ifdef MM64
#include "mm64.h"
#else
#include "mm.h"
#endif

// Helper để tìm PCB từ PID trong running_list
struct pcb_t *get_proc_by_pid(struct krnl_t *krnl, uint32_t pid) {
    if (krnl == NULL || krnl->running_list == NULL) return NULL;
    
    struct queue_t *q = krnl->running_list;
    for (int i = 0; i < q->size; i++) {
        struct pcb_t *proc = (struct pcb_t *)q->proc[i];
        if (proc != NULL && proc->pid == pid) {
            return proc;
        }
    }
    return NULL;
}

int __sys_memmap(struct krnl_t *krnl, uint32_t pid, struct sc_regs* regs)
{
   int memop = regs->a1;
   int ret = 0;
   BYTE value;
   
   /* 1. FIX: Lookup process thật từ running_list */
   struct pcb_t *caller = get_proc_by_pid(krnl, pid);

   if (caller == NULL) {
       return -1; // Process not found
   }

   /* 2. Implement operations & Synchronization */
   switch (memop) {
   case SYSMEM_MAP_OP:
            /* Mapping memory needs lock on mm structure */
            if (caller->mm != NULL) {
                pthread_mutex_lock(&caller->mm->pgd_lock);
                ret = vmap_pgd_memset(caller, regs->a2, regs->a3); // a2: addr, a3: value/size depending on impl
                pthread_mutex_unlock(&caller->mm->pgd_lock);
            } else {
                ret = -1;
            }
            break;

   case SYSMEM_INC_OP:
            /* Increasing heap/stack limit needs lock */
            if (caller->mm != NULL) {
                pthread_mutex_lock(&caller->mm->pgd_lock);
                ret = inc_vma_limit(caller, regs->a2, regs->a3);
                pthread_mutex_unlock(&caller->mm->pgd_lock);
            } else {
                ret = -1;
            }
            break;

   case SYSMEM_SWP_OP:
            /* Swap page needs lock */
            if (caller->mm != NULL) {
                pthread_mutex_lock(&caller->mm->pgd_lock);
                ret = __mm_swap_page(caller, regs->a2, regs->a3);
                pthread_mutex_unlock(&caller->mm->pgd_lock);
            } else {
                ret = -1;
            }
            break;

   case SYSMEM_IO_READ:
            /* Read from Physical RAM (MRAM) */
            // a2: address, a3: return value container (register)
            if (MEMPHY_read(caller->krnl->mram, regs->a2, &value) == 0) {
                regs->a3 = (uint32_t)value; // Return value via register
                ret = 0;
            } else {
                ret = -1;
            }
            break;

   case SYSMEM_IO_WRITE:
            /* Write to Physical RAM (MRAM) */
            // a2: address, a3: value to write
            ret = MEMPHY_write(caller->krnl->mram, regs->a2, regs->a3);
            break;

   default:
            ret = -1;
            break;
   }
   
   return ret;
}