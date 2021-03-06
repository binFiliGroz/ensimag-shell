#include "shell.h"

void print_cmdline (struct cmdline * l) {
    unsigned int i, j;

    /* Display each command of the pipe */
    for (i=0; l->seq[i]!=0; i++) {
	char **cmd = l->seq[i];
	printf("seq[%d]: ", i);
	for (j=0; cmd[j]!=0; j++) {
	    printf("'%s' ", cmd[j]);
	}
	printf("\n");
    }
}

void free_seq (struct cmdline * l) {
    unsigned int i, j;

    for (i=0; l->seq[i]!=0; i++) {
	char **cmd = l->seq[i];
	for (j=0; cmd[j]!=0; j++) {
	    free(l->seq[i][j]);
	}
	free(l->seq[i][j]);
	free(l->seq[i]);
    }
    free(l->seq[i]);
    l->seq=NULL;
}

void jokers_etendus (struct cmdline * l) {
    char *** seq2=NULL;
    unsigned int i, nb_cmd;

    // calcul du nombres de commandes
    // separees par des pipes
    nb_cmd=0;
    while (l->seq[nb_cmd])
	nb_cmd++;

    seq2=malloc(2*sizeof(void *));

    // traitement de toutes les commmandes
    for (i=0; i<nb_cmd; i++) {
	traite_commande(l->seq[i], seq2+i);
    }
    // ajout de la valeur 'NULL' dans la derniere commande
    // pourt respecter la structure 'struct cmdline'
    seq2[i]=NULL;

    // liberation de l'ancienne liste de commandes
    free_seq(l);
    // attribution de la nouvelle liste de commandes
    l->seq=seq2;
}

void traite_commande(char ** commande, char *** commande2) {
    int i, nb_arg, flags;
    glob_t globbuf;
    globbuf.gl_offs =0;
    // drapeaux d'appel de la fonction glob()
    // voir 'man glob'
    flags = GLOB_DOOFFS | GLOB_NOMAGIC | GLOB_MARK | GLOB_BRACE | GLOB_TILDE;

    // recherche de 'matchs' sur chaque segment de la commande
    glob(commande[0], flags, NULL , &globbuf);
    i=1;
    while (commande[i] != NULL) {
        glob(commande[i], flags | GLOB_APPEND, NULL, &globbuf);
	i++;
    }

    nb_arg=globbuf.gl_pathc+globbuf.gl_offs;

    // allocation et construction de la nouvelle commande
    *commande2=malloc((nb_arg+1)*sizeof(void *));
    for(i=0; i<nb_arg; i++) {
	(*commande2)[i]=strdup(globbuf.gl_pathv[i]);
    }
    (*commande2)[i]=NULL;

}

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
		      if (in==-1) {
			  perror("erreur d'ouverture de fichier");
		      }
                      dup2(in, 0);
                  }
                  if (execvp(l->seq[0][0], l->seq[0])==-1)
		      perror("erreur d'exécution");
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
		  if (in==-1)
		      perror("erreur d'ouverture de fichier");
                  dup2(in, 0);
              }
            }
            // si sortie vers fichier
            if (l->out) {
                out=open(l->out, O_WRONLY|O_TRUNC|O_CREAT, 0666);
		if (out==-1)
		    perror("erreur d'ouverture de fichier");
                dup2(out, 1);
            }
            // si processus en arrière plan
            // alors fermeture de l'entrée standard
            if (l->bg) 
                fclose(stdin);
            // execution de la commande
            if (execvp(cmd[0], cmd)==-1)
		perror("erreur d'exécution");
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


