#include<linux/kernel.h>
#include<linux/syscalls.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/proc_fs.h>
#include<linux/sched.h>
#include<linux/mm.h>
#include<linux/mm_types.h>
#include<linux/highmem.h>
#include<linux/string.h>
#include<asm/current.h>
struct mm_struct *mm_;



unsigned long trans_(unsigned long vaddr) {
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	unsigned long paddr = 0;
	unsigned long page_addr = 0;
	unsigned long page_offset = 0;
        const struct page *page;
	pgd = pgd_offset(mm_, vaddr);
        if (pgd_none(*pgd)) {
	    printk("not mapped in pgd\n");
	    return -1;
	}
        
	pud = pud_offset(pgd, vaddr);
        if (pud_none(*pud)) {
            printk("not mapped in pud\n");
            return -1;
        }

	pmd = pmd_offset(pud, vaddr);
        if (pmd_none(*pmd)) {
            printk("not mapped in pmd\n");
            return -1;
        }
       
	if(!(pte = pte_offset_map(pmd, vaddr)))
                return 0;
        if(!(page = pte_page(*pte)))
                return 0;
        paddr = page_to_phys(page);
        pte_unmap(pte);
        return paddr;
}


asmlinkage long sys_linux_survey_TT(unsigned long *result) {
	int curr_pid, exist = 0, valid_len = 0, total_len = 0;
	struct task_struct *task;
	struct vm_area_struct *mmap;
	unsigned long vm_start, vm_end;
	unsigned long pm_start, pm_end;
	unsigned long start_tmp, phy_tmp;
	curr_pid = task_pid_nr(current);
	printk("current pid : %d", curr_pid);
	task = find_task_by_vpid(curr_pid);
	mm_ = task->active_mm;
	mmap = mm_->mmap;
	printk("__________PID : %d__________", curr_pid);
	while(mmap != NULL){
		vm_start = mmap->vm_start;
		vm_end = mmap->vm_end;
		start_tmp = vm_start;
		while(start_tmp != vm_end){
			total_len++;
			phy_tmp = trans_(start_tmp);
			if(phy_tmp != 0)
				valid_len++;
			start_tmp += 0x00001000;
		}
                unsigned long i;
                for(i=vm_start; i<vm_end; i+=4096){
			result[exist++] = i;
                        result[exist++] = i+4096;
			result[exist++] = trans_(i);
              		result[exist++] = trans_(i+4096);
		}
	
		
	
		if(mmap->vm_next == NULL){
			printk("valid_len : %d\n", valid_len);
			printk("total_len : %d\n", total_len);
			printk("Percentage : %d\n", valid_len*100/total_len);
		}
		mmap = mmap->vm_next;
	}
	return 0;
}
