#include "user.h"

PCB *pcb;
systemClock *computerTime;
int pcbID = 0;
int clockID = 0;

int main(int argc, char *argv[])
{
	signal(SIGALRM, TimeHandler);
	signal(SIGQUIT,INThandler);
	signal(SIGINT, INThandler);
	signal(SIGUSR1, sigDie);
	
	int pcbLocation = atoi(argv[0]);
	key_t PCBkey =  atoi(argv[1]);
	pcbID = atoi(argv[2]);
	clockID = atoi(argv[3]);
	
	computerTime = (systemClock*)shmat(clockID, NULL, 0);
	if(computerTime->nanoSec == (int)-1)
	{
		releaseMem();
		perror("Shmat error in Main computerTime");
		exit(1);
	}
	
	pcb = (PCB*)shmat(pcbID, NULL, 0);
	if((void*)pcb == (void*)-1)
	{
		perror("shmat child pcb");
		releaseMem();
		exit(1);
	}
	int temp = 0;
	
	while(1)
	{
		if(pcb[pcbLocation].turn == 1)
		{
			if(pcb[pcbLocation].fullRunTime <= pcb[pcbLocation].timeInSys)
			{
				computerTime->TotalNanoInSys += pcb[pcbLocation].timeInSys;
				if(computerTime->TotalNanoInSys >= NANOSECOND)
				{
					if(computerTime->TotalNanoInSys > NANOSECOND)
						temp = computerTime->TotalNanoInSys - NANOSECOND;
					
					computerTime->TotalSecInSys += 1;
					computerTime->TotalNanoInSys = temp;
					temp = 0;
				}
				pcb[pcbLocation].turn = 0;
				pcb[pcbLocation].valid = 0;
				releaseMem();
				break;
			}
			else
			{
				if(pcb[pcbLocation].intIO)
				{
					AddTime(pcb[pcbLocation].timeQuantum);
					pcb[pcbLocation].timeInSys+=pcb[pcbLocation].randIOTime;
					pcb[pcbLocation].randIOTime = 0;
					pcb[pcbLocation].intIO = 0;
				}
				else
				{
					AddTime(pcb[pcbLocation].timeQuantum);
					pcb[pcbLocation].timeInSys+=pcb[pcbLocation].timeQuantum;
				}
			}
			pcb[pcbLocation].turn = 0;
		}
		else
		{
			AddTimeWait();
		}
	}
	releaseMem();
	exit(1);
	return 0;
}
void INThandler(int sig)
{ 
  signal(sig, SIG_IGN);
  fprintf(stderr, "\nCtrl^C Called, Process Exiting\n");
  releaseMem();
  kill(getpid(), SIGKILL);
}
void TimeHandler(int sig)
{
  releaseMem();
  fprintf(stderr, "\nOut of Time, Process %d Exiting\n", getpid());
  kill(getpid(), SIGKILL);
}
void AddTime(int change)
{
	computerTime->nanoSec +=change;
	if(computerTime->nanoSec >= NANOSECOND)
	{
		computerTime->second +=1;
		computerTime->nanoSec = 0;
	}	
}
void AddTimeWait()
{
	int temp = 0;
	computerTime->nanoSec +=1;
	computerTime->userWaitNano +=1;
	if(computerTime->nanoSec >= NANOSECOND)
	{
		computerTime->second +=1;
		if(computerTime->nanoSec > NANOSECOND)
			temp = computerTime->nanoSec - NANOSECOND;
		computerTime->nanoSec = temp;
		temp = 0;
	}
	
	if(computerTime->userWaitNano >= NANOSECOND)
	{
		computerTime->userWaitSec +=1;
		if(computerTime->userWaitNano > NANOSECOND)
			temp = computerTime->userWaitNano - NANOSECOND;
		computerTime->userWaitNano = temp;
		temp = 0;
	}
}
void sigDie(int sig)
{ 
	printf("/t/t/tExiting pid: %d\n", getpid());
	releaseMem();
	exit(0);
}
void releaseMem()
{
	if((shmctl(pcbID , IPC_RMID, NULL)) == -1)
	{
		perror("Error in shmdt in Parent");
	}
	if((shmctl(clockID, IPC_RMID, NULL)) == -1)
	{ 
		printf("Error in shmclt"); 
	}
}

