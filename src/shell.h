#ifndef __SHELL_H
#define __SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "readcmd.h"
#include "jobs.h"

pid_t launch_command (struct cmdline *l, struct rlimit * rl, JOB_LIST * pjobs);

#endif
