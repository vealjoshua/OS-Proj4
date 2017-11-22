#ifndef SHARE_H
#define SHARE_H

typedef struct Clock
{
  int nanoSec;
  int second; 
  
  int idleTimeNano;
  int idleTimeSec;
 
  int overHeadNano;
  int overHeadSec;
  
  int TotalNanoInSys;
  int TotalSecInSys;
  
  int userWaitNano;
  int userWaitSec;
}systemClock;

typedef struct PCB
{
	pid_t userPid; 
	int priority;
	int timeQuantum;
	int valid;
	int fullRunTime;
    int timeInSys;
	int turn;
	int intIO;
	int randIOTime;
}PCB;

typedef struct queue
{
	int index;
	int queuePlace;
	struct queue *next;
}queue;

static unsigned int g_seed;

inline void fast_srand(int seed) {
    g_seed = seed;
}

inline int fast_rand(void) {
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}

#endif
