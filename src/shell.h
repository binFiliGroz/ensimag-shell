#ifndef __SHELL_H
#define __SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "readcmd.h"
#include "jobs.h"

pid_t launch_command (struct cmdline *l);

#endif
