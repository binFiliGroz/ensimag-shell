#include "shell.h"

pid_t launch_command (struct cmdline *l){
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
            // si on utilise un pipe
            if (l->seq[1]) {
                pipe(tuyau);
                // commande d'entrée du pipe
                if ((res=fork())==0) {
                  // branchement de l'entrée du pipe
                  // sur la sortie du processus
                  dup2(tuyau[1], 1);
                  close(tuyau[0]);
                  // si fichier en entrée
                  if (l->in) {
                      in=open(l->in, O_RDONLY);
                      dup2(in, 0);
                  }
                  execvp(l->seq[0][0], l->seq[0]);
                }
                // commande de sortie du pipe

                // branchement de la sortie du pipe
                // sur l'entrée du processus
                dup2(tuyau[0], 0);
                close(tuyau[1]);

                cmd=l->seq[1];
            }
            else {
              // si fichier en entrée
              if (l->in) {
                  in=open(l->in, O_RDONLY);
                  dup2(in, 0);
              }
            }
            // si sortie vers fichier
            if (l->out) {
                out=open(l->out, O_WRONLY|O_TRUNC|O_CREAT, 0666);
                dup2(out, 1);
            }
            // si processus en arrière plan
            // alors fermeture de l'entrée standard
            if (l->bg) 
                fclose(stdin);
            // execution de la commande
            execvp(cmd[0], cmd);
         }
     	 //si on se trouve dans le processus père
         default:
         {
            // si le processus est au premier plan
            // alors on attends la fin de l'éxecution de la commande
            if (!(l->bg)) 
                waitpid(pid, &status, 0);
            break;
         }
     }

     return pid;
}

joker_etendu(){

    int i = 0;
    glob_t globbuf;
    globbuf.gl_offs = 0;
    glob(l->seq[0][0], GLOB_DOOFFS, NULL , &globbuf);
    while(l->seq[i] != NULL) {
            i++;
            glob(l->seq[i][j], GLOB_DOOFFS | GLOB_APPEND, NULL, &globbuf);
    }
}

