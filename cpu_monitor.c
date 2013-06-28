/*
 *============================================================
 *
 *       Filename:  cpu_monitor.c
 *
 *    Description:  Monitoring the CPU utilization realtimely.
 *
 *        Version:  1.0
 *        Created:  06/28/2013 10:22:35 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ixiami
 *
 *============================================================
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFSIZE 256

typedef struct {
    unsigned long user,
                  nice,
                  sys,
                  idle,
                  iowait,
                  irq,
                  softirq;
}cpu_t;

cpu_t* cpu_check(cpu_t *cpus)
{
    if (!cpus) {
        return NULL;
    }
    int fd = open("/proc/stat", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open /proc/stat");
        return NULL;
    }
    char buf[BUFSIZE];
    read(fd, buf, BUFSIZE);
    close(fd);

    sscanf(buf, "cpu %lu %lu %lu %lu %lu %lu %lu",
            &cpus->user, &cpus->nice, &cpus->sys ,&cpus->idle,
            &cpus->iowait, &cpus->irq, &cpus->softirq);
    return cpus;
}

static void print_cpu(cpu_t* cpu1, cpu_t* cpu2)
{
    if (!cpu1 || !cpu2) {
        return;
    }
    unsigned long jiff_0 = cpu1->user
                         + cpu1->sys
                         + cpu1->nice
                         + cpu1->idle
                         + cpu1->iowait
                         + cpu1->irq
                         + cpu1->softirq;
    unsigned long jiff_1 = cpu2->user
                         + cpu2->sys
                         + cpu2->nice
                         + cpu2->idle
                         + cpu2->iowait
                         + cpu2->irq
                         + cpu2->softirq;
    float sys_idle = 100 * (float)(cpu2->idle - cpu1->idle) /
                            (jiff_1 - jiff_0);
    float sys_usage = 100 - sys_idle;
    printf("%.2f\n", sys_usage);
}
int main(void)
{
    cpu_t cpu1, cpu2;
    while (1) {
    cpu_t *p_cpu1 = cpu_check(&cpu1);
        sleep(1);
        cpu_t *p_cpu2 = cpu_check(&cpu2);
        print_cpu(p_cpu1, p_cpu2);
    }
    return 0;
}
