#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <asm/current.h>
#include <linux/pid.h>
#include <linux/pid_namespace.h>

asmlinkage int sys_get_process_session_group(unsigned int *result, int size){
  struct task_struct *t1, *t2;
  int curr_pid,local_pid,  exist = 0;
  struct pid_namespace *ns;
  
  t1 = current;


  struct pid *sid, *pid;

   sid = task_session(t1);

  for_each_process(t2){
    if(exist>size)
        break;
      if(task_session(t2) == sid){

         pid = task_pid(t2);
         local_pid = pid_vnr(pid);
         if(local_pid == 0){
          ns = task_active_pid_ns(t2);
          local_pid = pid_nr_ns(pid, ns);
         }
       
         result[exist++] = local_pid;
      }


  }
   return exist;
}


