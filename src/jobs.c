#include "jobs.h"

JOB_LIST create_job_list() {
    return NULL;
}

bool is_empty_job_list(JOB_LIST jobs) {
    if (jobs==NULL)
        return true;
    return false;
}

void free_job(JOB_LIST pjob) {
    free(pjob);
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

void update_print_job_list (JOB_LIST * pjobs) {
    JOB_LIST pre=*pjobs, cur=*pjobs;
    pid_t return_pid, pid;
    char name[255], etat[255];
    int status;

    while (!is_empty_job_list(cur)) {
        return_pid = waitpid(cur->pid, &status, WNOHANG);
        strcpy(name, cur->name);
        pid=cur->pid;
        switch (return_pid) {
            case -1:
                // erreur
                perror("Erreur de status du processus en arrière-plan");
                break;
            case 0:
                // processus en cours d'execution
                strcpy(etat, "en cours");
                cur=cur->nxt;
                break;
            default:
                // processus termine
                strcpy(etat, "terminé");
                if (cur==*pjobs) {
                    *pjobs=cur->nxt;
                    pre=cur->nxt;
                    free_job(cur);
                    cur=pre;
                }
                else {
                    pre->nxt=cur->nxt;
                    free_job(cur);
                    cur=pre->nxt;
                }
                break;
        }
        printf("%d %s [%s]\n", pid, name, etat);
    }
}

