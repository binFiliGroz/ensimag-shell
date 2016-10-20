#ifndef __SHELL_H
#define __SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <glob.h>

#include "readcmd.h"
#include "jobs.h"

// liberation de la liste de comanndes l->seq
void free_seq(struct cmdline * l);
// traitement de 'commande' a l'aide de glob 
// et ecriture de la nouvelle commande à l'adresse 'commande2'
void traite_commande(char ** commande, char *** commande2);
// traitement de la liste de commandes l->seq
void jokers_etendus(struct cmdline * l);
// affiche la liste de commandes l->seq
void print_cmdline(struct cmdline * l);
// lance la liste de commandes l->seq
// en tenant compte des entrées/sorties, pipes et arriere-plan
pid_t launch_command (struct cmdline *l);

#endif
