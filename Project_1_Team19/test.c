#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <linux/kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#define MEMORY_SIZE	100000


void display(unsigned long *result)
{
    int i, num_tmp = 0;
    for (i = 0; i < MEMORY_SIZE; i++)
        if (result[i])
            num_tmp++;
    for (i = 0; i < num_tmp; i+=4)
    {
        printf("virt : %lx - %lx\n", result[i], result[i+1]);
        printf("phys : %lx - %lx\n", result[i+2], result[i+3]);
    }
}

void fWrite(unsigned long *result, char *str)
{
    int i, num_tmp = 0;
    FILE *fp = fopen(str, "w");
    for (i = 0; i < MEMORY_SIZE; i++)
        if (result[i])
            num_tmp++;
    for (i = 0; i < num_tmp; i+=4)
    {
        fprintf(fp, "virt : %lx - %lx\n", result[i], result[i+1]);
        fprintf(fp, "phys : %lx - %lx\n", result[i+2], result[i+3]);
    }
    fclose(fp);
}

int main()
{
    struct sysinfo sys;
    int err = sysinfo(&sys);
    unsigned long result_1[MEMORY_SIZE]= {};
    unsigned long result_2[MEMORY_SIZE]= {};
    unsigned long result_3[MEMORY_SIZE]= {};
    int exit_status;

	int *pte = (int*)malloc(sizeof(int)*1000);
    printf("123");
    if(fork())
    {

        syscall(351,result_1);
        wait(&exit_status);
        printf("---------result_1----------\n");
        display(result_1);
        fWrite(result_1, "result_1.txt");
    }
    else
    {
        int pid = getpid();
        printf("pid: %d\n",pid);
        syscall(351, result_2);
        result_1[0] = 123;
        syscall(351, result_3);
        printf("---------result_2----------\n");
        display(result_2);
        printf("---------result_3----------\n");
        display(result_3);
        fWrite(result_2, "result_2.txt");
        fWrite(result_3, "result_3.txt");
	while(1)
		sleep(1);

    }

    printf("\nfree mem:\t%ld\n", sys.freeram);
    printf("\ntotal mem:\t%ld\n", sys.totalram);
    printf("\nbuffer mem:\t%ld\n", sys.bufferram);

    return 0;
}
