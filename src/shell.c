#include "shell.h"

pid_t launch_command (struct cmdline *l) {
     pid_t pid, res;
     int tuyau[2], status;
     char **cmd=l->seq[0];

     switch(pid = fork()) {
         case -1:
                 perror("fork:");
                 break;
    	 // si on se trouve dans le processus fils
         case 0:
         {
            // si on utilise un pipe
            if (l->seq[1]) {
                pipe(tuyau);
                // commande de sortie du pipe
                if ((res=fork())==0) {
                   // branchement de la sortie du pipe 
                   // sur l'entrée du processus
                   dup2(tuyau[0], 0);
                   close(tuyau[1]);
                   execvp(l->seq[1][0], l->seq[1]);
                }
                // commande d'entrée du pipe

                // branchement de l'entrée du pipe
                // sur la sortie du processus
                dup2(tuyau[1], 1);
                close(tuyau[0]);
            }
            // si processus en arrière plan
            // alors fermeture de l'entrée standard
            if (l->bg) {
                fclose(stdin);
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
            break;
         }
     }

     return pid;
}
