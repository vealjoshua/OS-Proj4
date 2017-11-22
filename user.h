#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>
#include <error.h>
#include <assert.h>
#include <sys/msg.h>
#include <errno.h>
#include "share.h"
#include <stdio.h>

#define  NANOSECOND    1000000000

void INThandler(int sig);
void TimeHandler(int sig);
void sigDie(int sig);
int random_number(int min_num, int max_num);
void AddTime(int change);
void AddTimeWait();
void releaseMem();

#endif