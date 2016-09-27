#ifndef __JOBS_H
#define __JOBS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct job JOB;
struct job 
{
    pid_t pid;
    struct job *nxt;
};
 
typedef JOB* JOB_LIST;

JOB_LIST create_job_list();
bool is_empty_job_list(JOB_LIST jobs);
unsigned int add_job (JOB_LIST * jobs, pid_t pid);
void print_job (pid_t pid, unsigned int i);
void print_job_list (JOB_LIST jobs);
#endif
