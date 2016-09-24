#include "shell.h"

pid_t launch_command (struct cmdline *l) {
     pid_t pid;

     switch(pid = fork()) {
         case -1:
                 perror("fork:");
                 break;
    	 // si on se trouve dans le processus fils
         case 0:
         {
            char **cmd = l->seq[0];
            // fermeture des entrées/sorties standards si 
            // processus en tâche de fond
            if (l->bg) {
                fclose(stdin);
                fclose(stdout);
                fclose(stderr);
            }
            // execution de la commande
            execvp(cmd[0], cmd);
            break;
         }
     	 //si on se trouve dans le processus père
         default:
         {
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
