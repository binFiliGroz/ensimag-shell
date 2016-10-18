#ifndef __JOBS_H
#define __JOBS_H

/*
 *  Structure de liste chaînée pour le stockage 
 *  des processus lancés en arrière-plan
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct job JOB;
struct job 
{
    pid_t pid;
    char name[255];
    struct job *nxt;
};
 
typedef JOB* JOB_LIST;

// crée liste vide
JOB_LIST create_job_list();
// teste si la liste est vide
bool is_empty_job_list(JOB_LIST jobs);
// ajoute un "job" et renvoie le nombre de jobs dans la liste
unsigned int add_job (JOB_LIST * jobs, pid_t pid, char * command);
// parcours la liste et affiche les jobs
void update_print_job_list (JOB_LIST * pjobs);

#endif
