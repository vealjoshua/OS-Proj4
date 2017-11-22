#ifndef OSS_H
#define OSS_H

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

#define  NANOSECOND    1000000000

int z = 20;
int x = 18;

static int ioCount = 0;
static int processMade = 0;
static int processFinished = 0;
int printLineCount = 0;
FILE *filePtr;

int queue0Location = 0;
int q1Location = 0;
int q2Location = 0;
int q3Location = 0;
int pcbArr[18];
queue *queue0 = NULL;
queue *q1 = NULL; 
queue *q2 = NULL;
queue *q3 = NULL;
queue qArray [2];

systemClock *computerTime;
PCB *pcb;
int pcbID = 0;
int clockID = 0;
key_t PCBkey = 1994;
key_t Clockkey = 1991;

void comOptions (int argc, char **argv , int *x, int *z, char **filename);
void displayHelpMesg();
void validate(int *x,int temp,char y);
void test(int x, int z,char*file);

void INThandler(int sig);
void on_alarm(int signal);
void releaseMem();

int runQ0(int *count,queue *current);
void runQ1(int *count,int qnum);
void runQ2(int *count,int qnum);
void runQ3(int *count,int qnum);

void AddTime(int choice, int change);

int random_number(int min_num, int max_num);
void makeNewProcess(char * pcbArrLocationPass,char *pcbKeypass,char *pcbIDpass,char *clockIDpass);
void insertINqueue(queue *head, int index);
void printQueue(queue *head);
int checkIfQueueEmpty(queue *head);
void remove_by_index(queue **head, int n);
int findValidPCB ();
int getRandFork();
void setUpPCB(int i, pid_t childPid);
void resetPCB(int i);
int getIO(pid_t childPid);

void printToFile(int choice, int index);
void calc (int pm, int sec, int nsec);
void calc2 (int pm, int sec, int nsec);

#endif
