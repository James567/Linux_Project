
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <asm/current.h>
#include <linux/pid.h>

asmlinkage int sys_get_process_zero_session_group(unsigned int *result, int size){
  struct task_struct *t;
  int exist = 0;
  int i = 0;
  for_each_process(t){
    if(exist>size)
        break;
    if(pid_nr(task_session(t)) == 0){
       result[exist++] = t->pid;
    }

  }
   return exist;



}


