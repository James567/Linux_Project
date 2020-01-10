#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <linux/kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#define SIZE	100


int main(){
   unsigned int results[SIZE];
   int j,k;
   k = syscall(351, results, SIZE);
   if(k)
   {
     printf("What follows are the PIDs of the porcesses that are in the same login session of this process \n");
     for(j=0; j<k && j<SIZE; j++)
       printf("[%d] %u ",j, results[j]);
     printf("\n");
    }
    else
    printf("There is an error when executing this system call.\n");

}
