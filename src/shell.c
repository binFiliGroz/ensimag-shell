#include "shell.h"

pid_t launch_command (struct cmdline *l, struct rlimit * rl, JOB_LIST * pjobs){
     pid_t pid, res;
     int tuyau[2], status, in, out;
     char **cmd=l->seq[0];

     switch(pid = fork()) {
         case -1:
                 perror("fork:");
                 break;
    	 // si on se trouve dans le processus fils
         case 0:
         {
            if (rl) {
                setrlimit(RLIMIT_CPU, rl);
            }
            // si on utilise un pipe
            if (l->seq[1]) {
                pipe(tuyau);
                // commande de sortie du pipe
                if ((res=fork())==0) {
                   // branchement de la sortie du pipe 
                   // sur l'entrée du processus
                   dup2(tuyau[0], 0);
                   close(tuyau[1]);
                   if (l->out) {
                       out = open(l->out, O_WRONLY|O_TRUNC|O_CREAT, 0666); 
                       dup2(out, 1);
                   }
                   execvp(l->seq[1][0], l->seq[1]);
                }
                // commande d'entrée du pipe

                // branchement de l'entrée du pipe
                // sur la sortie du processus
                dup2(tuyau[1], 1);
                close(tuyau[0]);
            }
            else {
                // si sortie vers fichier 
                if(l->out) {
                    out = open(l->out, O_WRONLY|O_TRUNC|O_CREAT, 0666); 
                    dup2(out, 1); 
                }
            }                
            // si processus en arrière plan
            // alors fermeture de l'entrée standard
            if (l->bg) {
                fclose(stdin);
            }
            // si fichier en entrée
            if (l->in) {
                in=open(l->in, O_RDONLY);
                dup2(in, 0);
            }
            // execution de la commande
            execvp(cmd[0], cmd);
         }
     	 //si on se trouve dans le processus père
         default:
         {
            // si le processus est au premier plan
            // alors on attends la fin de l'éxecution de la commande
            if (!(l->bg)) {
                waitpid(pid, &status, 0);
            }
            else {
                add_job(pjobs, pid, l->seq[0][0]);
            }
            break;
         }
     }

     return pid;
}
