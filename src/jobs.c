#include "jobs.h"

JOB_LIST create_job_list() {
    return NULL;
}

bool is_empty_job_list(JOB_LIST jobs) {
    if (jobs==NULL)
        return true;
    return false;
}

unsigned int add_job (JOB_LIST * pjobs, pid_t pid, char * command) {
    JOB_LIST cur=*pjobs;
    unsigned int i=2;

    if (is_empty_job_list(*pjobs)) {
        *pjobs=malloc(sizeof(**pjobs));
        (*pjobs)->pid=pid;
        strcpy((*pjobs)->name, command);
        (*pjobs)->nxt=create_job_list();
        return 1;
    }

    while (!is_empty_job_list(cur->nxt)) {
        ++i;
        cur=cur->nxt;
    }

    cur->nxt=malloc(sizeof(*cur));
    cur=cur->nxt;
    cur->pid=pid;
    strcpy(cur->name, command);
    cur->nxt=create_job_list();

    return i;
}

void print_job_list (JOB_LIST jobs) {
    JOB_LIST cur=jobs;

    while (!is_empty_job_list(cur)) {
        printf("%s\n", cur->name);
        cur=cur->nxt;
    }
}

