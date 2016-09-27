#include "shell.h"

pid_t launch_command (struct cmdline *l) {
     pid_t pid;
     int tuyau[2];

     if (l->seq[1]!=NULL) {
         pipe(tuyau);
     }

     switch(pid = fork()) {
         case -1:
                 perror("fork:");
                 break;
    	 // si on se trouve dans le processus fils
         case 0:
         {
            char **cmd = l->seq[0];
            
            if (l->seq[1]) {
                dup2(tuyau[0], 0);
                close(tuyau[1]); close(tuyau[0]);
            }
            if (l->bg) {
                fclose(stdin);
            }
            // execution de la commande
            execvp(cmd[0], cmd);
            break;
         }
     	 //si on se trouve dans le processus père
         default:
         {
            if (l->seq[1]) {
                dup2(tuyau[1], 1);                                                 
                close(tuyau[0]); close(tuyau[1]);  
            }
            // si le processus est au premier plan
            // alors on attends la fin de l'éxecution de la commande
            if (!(l->bg)) {
                int status;
                waitpid(pid, &status, 0);
            }
            break;
         }
     }

     return pid;
}
