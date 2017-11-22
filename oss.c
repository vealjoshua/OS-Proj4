#include "oss.h"

int main(int argc, char **argv)
{
	signal(SIGALRM, on_alarm);
	signal(SIGQUIT,INThandler);
	signal(SIGINT, INThandler);

	char *filename = "test.out";

	if (argc < 2)
	{
		printf("Running Program without any Commands, default options will be apply.\n");
	}
	else
	{
		comOptions(argc,argv,&x,&z,&filename);
	}
	test(x,z,filename);
	
	filePtr = fopen (filename, "w");
    if (filePtr == 0)
    {
      perror("Could not open file ");
      exit(0);
    }
	
	if((clockID = shmget(Clockkey, sizeof(systemClock), IPC_CREAT | 0666)) < 0)
        {
		releaseMem();
		perror("shmget in parent computerTime");
		exit(1);
	}
	computerTime = (systemClock*)shmat(clockID, NULL, 0);
	if(computerTime->nanoSec == (int)-1)
	{
		releaseMem();
		perror("Shmat error in Main computerTime");
		exit(1);
	}
	
	
	computerTime->nanoSec = 0;
	computerTime->second  = 0;
	
	computerTime->idleTimeNano = 0;
	computerTime->idleTimeSec = 0;
	
	computerTime->overHeadNano = 0;
	computerTime->overHeadSec = 0;
	
	computerTime->userWaitNano = 0;
	computerTime->userWaitSec = 0;
	
    if((pcbID = shmget(PCBkey, sizeof(pcb[x]), IPC_CREAT | 0666)) == -1)
	{
		perror("shmget parent pcb");
		releaseMem();
		exit(1);
	}
	pcb = (PCB*)shmat(pcbID, NULL, 0);
	if((void*)pcb == (void*)-1)
	{
		perror("shmat parent pcb");
		releaseMem();
		exit(1);
	}

	
	char* pcbArrLocationPass = malloc(sizeof(int));
	char* pcbKeypass = malloc(sizeof(int));
	char* pcbIDpass = malloc(sizeof(int));
	char* clockIDpass = malloc(sizeof(int));
	sprintf(clockIDpass, "%d", clockID);
	sprintf(pcbKeypass, "%d", PCBkey);
	sprintf(pcbIDpass, "%d", pcbID);
	
	int forkAtRand = 0;
	int i = 0;
	int getPosition = 0;
	
	queue0= malloc(sizeof(queue));
	queue0->index = 1000;
	queue0->queuePlace = queue0Location; 
	queue0->next = NULL;
	
	q1 = malloc(sizeof(queue));
	q1->index = 1001;
	q1->queuePlace = q1Location; 
	q1->next = NULL;
	
	q2 = malloc(sizeof(queue));
	q2->index = 1002;
	q2->queuePlace = q2Location; 
	q2->next = NULL;
	
	q3 = malloc(sizeof(queue));
	q3->index = 1003;
	q3->queuePlace = q3Location; 
	q3->next = NULL;


	queue *current = queue0;
	
	int count = 0;
	for(count; count < x; count++)
	{
		pcb[count].valid = 0;
	}

	count = 0;
	int ha = 0;
	alarm(z);

	while (1)
	{
		if (count == 0)
		{
			AddTime(1,10000);
		}
		
		if(computerTime->second >= 20)
		{
			printf("\nProgram Has Finished Correctly\n");
			releaseMem();
			int k = 0;
			for (k=0; i<x;k++)
			{
				kill(pcb[k].userPid, SIGTERM);
			}
			exit(0);
		}
		
		if(forkAtRand == computerTime->second)
		{
			if (count < x)
			{	
				makeNewProcess(pcbArrLocationPass,pcbKeypass,pcbIDpass,clockIDpass);
				count++;
				printf("Number of Process Running: %d | Total Number of Process Created: %d\n", count, processMade);
				forkAtRand = getRandFork();
			}
			else
			{
				forkAtRand = getRandFork();
			}
		}
		else if(forkAtRand <= computerTime->second || forkAtRand >= computerTime->second )
		{
			forkAtRand=getRandFork();
		}
		
		if(checkIfQueueEmpty(queue0))
			runQ0(&count,current);
		else if(checkIfQueueEmpty(q1))
			runQ1(&count,1);
		else if(checkIfQueueEmpty(q2))
			runQ2(&count,2);
		else if(checkIfQueueEmpty(q3))
			runQ3(&count,3);
	}
	
	printQueue(queue0);
	releaseMem();
	wait(NULL);
	printf("exiting\n");

	return 0;
}
void comOptions (int argc, char **argv , int *x, int *z, char **filename)
{ 
	int c = 0;
	int temp = 0;
	static struct option long_options[] = 
	{ 
		{"help", no_argument, 0, 'h'},
		{ 0,     0          , 0,  0 } 
	};
	int long_index = 0;
	while((c = getopt_long_only(argc, argv, "hs:t:l:", long_options, &long_index)) != -1)
	{
		switch (c)
		{
			case 'h':
				displayHelpMesg();
			break;
	  
			case 's':
				temp = *x;
				*x = atoi(optarg);
				if (*x > 20)
				{
					printf("Inputed: %d is to big. (Limit 20). Reverting back to default 5.\n", *x);
					*x = temp;
				}
				validate(x,temp,'x');
			break;

			case 't':
				temp = *z;
				*z = atoi(optarg);
				validate(z,temp,'z');
			break;

			case 'l':
				if (optopt == 'n')
				{
					printf("Please enter a valid filename.");
					return;
				}
				*filename = optarg;
			break;
      
			case '?':
				if (optopt == 'l')
				{
					printf("Command -l requires filename. Ex: -lfilename.txt | -l filename.txt.\n");
					exit(0);
				}
				else if (optopt == 's')
				{
					printf("Commands -s requires int value. Ex: -s213| -s 2132\n");
					exit(0);
				}
				else if (optopt == 'i')
				{
					printf("Command -y requires int value. Ex: -i213| -i 2132\n");
					exit(0);
				}
				else if (optopt == 't')
				{
					printf("Command -z requires int value. Ex: -t13| -t 2132\n");
					exit(0);	
				}
				else
				{
					printf("You have used an invalid command, please use -h or -help for command options, closing program.\n"); 
					exit(0);
				}
				return;
	  
			default :
				if (optopt == 'l')
				{
					printf ("Please enter filename after -l \n");
					exit(0);
				}
				else if (optopt == 'n')
				{ 
					printf ("Please enter integer x after -n \n");
					exit(1);
				}
				printf("Running Program without Commands.\n");
			break;
		}
	}
  
}
void displayHelpMesg()
{
	printf ("---------------------------------------------------------------------------------------------------------\n");
	printf ("Please run oss or oss -arguemtns.\n");
	printf ("----------Arguments---------------------------------------------\n");
	printf (" -h or -help  : shows steps on how to use the program \n");
	printf (" -s x         : x is the maximum number of slave processes spawned (default 5) \n");
	printf (" -l filename  : change the log file name \n");
	printf (" -t z         : parameter z is the time in seconds when the master will terminate itself (default 20) \n"); 
	printf ("---------------------------------------------------------------------------------------------------------\n");
	printf ("\nClosing Program.............\n\n");
	exit(0);
}
void validate(int *x,int temp,char y)
{
  char *print;
  char *print2;
  if (y == 'z')
  {
    print = "z";
    print2 = "-t";	  
  }
  else if (y == 'x')
  {
    print = "x";
    print2 = "-s";	  
  }
  
  
  if (*x == 0)
  {
    printf("Intput invalid for %s changing %s back or default.\n",print2,print);
    *x = temp;
  }
  else if (*x < 0)
  {
    printf("Intput invalid for %s changing %s back or default.\n",print2,print);
    *x = temp; 
  }
}
void test (int x,int z, char *file)
{
	printf ("--------------------------------\n");
	printf ("Number of Slaves (x): %d\n", x);
	printf ("Time limit       (z): %d\n", z);
	printf ("Filename            : %s\n", file);
	printf ("--------------------------------\n\n");
	printf("Running Program Now.\n");
	return;
}
void INThandler(int sig)
{
	signal(sig, SIG_IGN);
	printf("\nCtrl^C Called. Closing All Process.\n");
	fflush(stdout);
	releaseMem();
	int i =0;
	for (i=0; i<x;i++)
	{
		kill(pcb[i].userPid, SIGTERM);
	}

	exit(0);
}
void on_alarm(int signal)
{
	printf("Out of time killing all slave processes.\n", z);
	printf("Time: %d.%d\n", computerTime->second,computerTime->nanoSec);
	releaseMem();
	int i = 0;
	for (i=0; i<x;i++)
	{
		kill(pcb[i].userPid, SIGTERM);
	}
    exit(0);
}
void releaseMem()
{
	printToFile(99,0);
	fclose(filePtr);
	if((shmctl(pcbID , IPC_RMID, NULL)) == -1)
	{
		perror("Error in shmdt in Parent");
	}
	if((shmctl(clockID, IPC_RMID, NULL)) == -1)
	{ 
		printf("Error in shmclt"); 
	}
}

int runQ0(int *count,queue *current)
{
	while(checkIfQueueEmpty(queue0))
	{

		current = queue0;
		current = current->next;
		if(current == NULL)
		{
			if (checkIfQueueEmpty(queue0) == 0)
			{
				return 0;
			}
		}
				
		pcb[current->index].intIO = getIO(pcb[current->index].userPid);
		if(pcb[current->index].intIO)
		{
			pcb[current->index].randIOTime = random_number(0,pcb[current->index].timeQuantum);
			printToFile(5,current->index);
		}
		pcb[current->index].turn = 1;
		while(pcb[current->index].turn == 1){}
		AddTime(0,random_number(500,10000));

		if(pcb[current->index].valid == 0)
		{
			processFinished++;
			int temp = current->index;
			*count -= 1;
			printToFile(0,temp);
			
			kill(pcb[temp].userPid,SIGKILL);
			wait(NULL);
			
			remove_by_index(&queue0, current->index);
			resetPCB(temp);
		}
		else if(pcb[current->index].valid  == 1)
		{
			int temp = current->index;
			remove_by_index(&queue0, current->index);
			insertINqueue(queue0, temp);
		}
	}
	return 0;
}
void runQ1(int *count,int qnum)
{
	queue *current = q1;
	
	int ifIO = 0;
	
	while(checkIfQueueEmpty(q1))
	{
		current = q1;
		current = current->next;
		if(current == NULL)
		{
			if (checkIfQueueEmpty(q1) == 0)
			{
				break;
			}
		}
		
		pcb[current->index].timeQuantum = 3000000;
		pcb[current->index].timeQuantum *=qnum+1;
		pcb[current->index].priority = qnum;
		
		pcb[current->index].intIO = getIO(pcb[current->index].userPid);
		if(pcb[current->index].intIO)
		{
			pcb[current->index].randIOTime = random_number(0,pcb[current->index].timeQuantum);
			printToFile(5,current->index);
			ifIO = 1;
		}
		pcb[current->index].turn = 1;
		while(pcb[current->index].turn == 1){}
		AddTime(0,random_number(500,10000));

		if(pcb[current->index].valid == 0)
		{
			processFinished++;
			int temp = current->index;
			printToFile(0,temp);
			*count -= 1;

			kill(pcb[temp].userPid,SIGKILL);
			wait(NULL);
					
			remove_by_index(&q1, current->index);
			resetPCB(temp);
		}
		else if(pcb[current->index].valid  == 1)
		{
			int temp = current->index;
			remove_by_index(&q1, current->index);
			if (ifIO == 1)
			{
				fprintf(filePtr,"\t\t\tChild Pid %d Had IO Moving to Q1 priority %d\n", pcb[temp].userPid,pcb[temp].priority);
				insertINqueue(q1, temp);
				ifIO = 0;
			}
			else
			{
				fprintf(filePtr,"\t\t\t\tChild Pid %d Moving to Q2\n", pcb[temp].userPid);
				insertINqueue(q2, temp);
			}
		}
	}
	return;
}
void runQ2(int *count,int qnum)
{
	queue *current = q1;
	
	int ifIO = 0;
	
	while(checkIfQueueEmpty(q2))
	{
		current = q2;
		current = current->next;
		if(current == NULL)
		{
			if (checkIfQueueEmpty(q2) == 0)
			{
				break;
			}
		}
		
		pcb[current->index].timeQuantum = 3000000;
		pcb[current->index].timeQuantum *=qnum+2;
		pcb[current->index].priority = qnum;
				
		pcb[current->index].intIO = getIO(pcb[current->index].userPid);
		if(pcb[current->index].intIO)
		{
			pcb[current->index].randIOTime = random_number(0,pcb[current->index].timeQuantum);
			printToFile(5,current->index);
			ifIO = 1;
		}
		pcb[current->index].turn = 1;
		while(pcb[current->index].turn == 1){}
		AddTime(0,random_number(500,10000));

		if(pcb[current->index].valid == 0)
		{
			processFinished++;
			int temp = current->index;
			*count -= 1;

			printToFile(0,temp);
			kill(pcb[temp].userPid,SIGKILL);
			wait(NULL);
					
			remove_by_index(&q2, current->index);
			resetPCB(temp);
		}
		else if(pcb[current->index].valid  == 1)
		{
			int temp = current->index;
			remove_by_index(&q2, current->index);
			if (ifIO == 1)
			{
				printToFile(3, temp);
				insertINqueue(q1, temp);
				ifIO = 0;
				break;
			}
			else
			{
				printToFile(4, temp);
				insertINqueue(q3, temp);
			}
		}
	}
	return;
}
void runQ3(int *count,int qnum)
{
	queue *current = q3;
	int ifIO = 0;
	
	while(checkIfQueueEmpty(q3))
	{
		current = q3;
		current = current->next;
		if(current == NULL)
		{
			if (checkIfQueueEmpty(q3) == 0)
			{
				break;
			}
		}
		
		pcb[current->index].timeQuantum = 3000000;
		pcb[current->index].timeQuantum *=qnum+3;
		pcb[current->index].priority = qnum;
				
		pcb[current->index].intIO = getIO(pcb[current->index].userPid);
		if(pcb[current->index].intIO)
		{
			pcb[current->index].randIOTime = random_number(0,pcb[current->index].timeQuantum);
			printToFile(5,current->index);
			ifIO = 1;
		}
		pcb[current->index].turn = 1;
		while(pcb[current->index].turn == 1){}
		AddTime(0,random_number(500,10000));

		if(pcb[current->index].valid == 0)
		{
			processFinished++;
			int temp = current->index;
			*count -= 1;
			
			printToFile(0,temp);

			kill(pcb[temp].userPid,SIGKILL);
			wait(NULL);
					
			remove_by_index(&q3, current->index);
			resetPCB(temp);
		}
		else if(pcb[current->index].valid  == 1)
		{
			int temp = current->index;
			remove_by_index(&q3, current->index);
			if (ifIO == 1)
			{
				printToFile(2, temp);
				insertINqueue(q1, temp);
				ifIO = 0;
				break;
				
			}
			else
			{
				insertINqueue(q3, temp);
			}
		}
	}
	return;
}
void AddTime(int choice, int change)
{
	int temp = 0;
	int c = choice;
	switch(c)
	{
		case 0:
			computerTime->nanoSec +=change;
			computerTime->overHeadNano +=change;
			if(computerTime->nanoSec >= NANOSECOND)
			{
				computerTime->second +=1;
				if(computerTime->nanoSec > NANOSECOND)
					temp = computerTime->nanoSec - NANOSECOND;
				computerTime->nanoSec = temp;
				temp = 0;
			}
			
			if(computerTime->overHeadNano >= NANOSECOND)
			{
				computerTime->overHeadSec +=1;
				if(computerTime->overHeadNano > NANOSECOND)
					temp = computerTime->overHeadNano - NANOSECOND;
				computerTime->overHeadNano = temp;
				temp = 0;
			}
			
			break;
		case 1:
			computerTime->nanoSec +=change;
			computerTime->idleTimeNano +=change;
			if(computerTime->nanoSec >= NANOSECOND)
			{
				computerTime->second +=1;
				if(computerTime->nanoSec > NANOSECOND)
					temp = computerTime->nanoSec - NANOSECOND;
				computerTime->nanoSec = temp;
				temp = 0;
			}

			
			if(computerTime->idleTimeNano >= NANOSECOND)
			{
				computerTime->idleTimeSec +=1;
				if(computerTime->idleTimeNano > NANOSECOND)
					temp = computerTime->idleTimeNano - NANOSECOND;
				computerTime->nanoSec = temp;
				temp = 0;
			}
			
			break;
	}
}
void makeNewProcess(char * pcbArrLocationPass,char *pcbKeypass,char *pcbIDpass,char *clockIDpass)
{
	int i = 0;
	if ((i=findValidPCB()) > -1)
	{	
		pid_t childPid;
		childPid = fork();
		
		if (childPid < 0)
		{
			perror("Fork() Parent Failed"); 
			printf("Q0\n");
			printQueue(queue0);
			printf("Q1\n");
			printQueue(q1);
			printf("Q2\n");
			printQueue(q2);
			printf("Q3\n");
			printQueue(q3);
			int p  = 0;
			for (p; p<x; p++)
			{
				fprintf(filePtr,"Child Death Pid: %d | valid: %d | Queue %d | timeInSys %d\n",pcb[p].userPid,pcb[p].valid,pcb[p].priority,pcb[p].timeInSys);
			}
			kill(getpid(), SIGALRM);	
			releaseMem();
			exit(1); 
		}
		else if (childPid == 0)
		{
			sprintf(pcbArrLocationPass, "%d" , i);
			execl("./user",pcbArrLocationPass, pcbKeypass, pcbIDpass,clockIDpass,NULL);
				perror("Child failed to execl");
		}
		setUpPCB(i, childPid);
		processMade++;
		printToFile(1, i);
	}
}

void insertINqueue(queue *head, int index)
{
	queue * current = head;
	int locate = 0;
	
	if(current->index == 1000)
		locate = ++queue0Location;
	else if(current->index == 1001)
		locate = ++q1Location;
	else if(current->index == 1002)
		locate = ++q2Location;
	else if(current->index == 1003)
		locate = ++q3Location;

	while(current->next != NULL)
	{
		current = current->next;
	}
	
	current->next = malloc(sizeof(queue));
	current->next->index = index;
	current->next->queuePlace = locate;
	current->next->next = NULL;
}
void printQueue(queue *head)
{
	queue *current = head;
	
	while(current != NULL)
	{
		printf("Place: %d , index: %d\n", current->queuePlace, current->index);
		current = current->next;
	}
}
int checkIfQueueEmpty(queue *head)
{
	int count = 0;
	queue *current = head;
	while(current != NULL)
	{
		count++;
		current = current->next;
	}
	
	if(count == 1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void remove_by_index(queue **head, int n)
{
	int i = 0;
	int locate = 0; 
	int retval = -1;
	
	queue * current = *head;
	queue * temp_node = NULL;

	while (current != NULL)
	{
		if(current->index == n)
		{
			locate = current->queuePlace;
			break;
		}
		current = current->next;
	}

	int count = 0;
	current = *head;
	for (count; count < locate-1; count++)
	{
		current = current->next;
	}
	
	temp_node = current->next;
	retval = temp_node->queuePlace;
	current->next = temp_node->next;
	free(temp_node);
	
	current = current->next;
	while(current != NULL) //
	{
		current->queuePlace -=1;
		current = current->next;
	}

	current = *head;
	if(current->index == 1000)
		--queue0Location;
	else if(current->index == 1001)
		--q1Location;
	else if(current->index == 1002)
		--q2Location;
	else if(current->index == 1003)
		--q3Location;

	return;
}
int random_number(int min_num, int max_num)
{
  if (max_num > 10000000)
  {
	  int randNum = min_num;
	  int result =0,low_num=0,hi_num=0;
	  if(min_num < max_num)
	  {
		low_num=min_num;
		hi_num=max_num+1;
	  }
	  else
	  {
		low_num=max_num+1;
		hi_num=min_num;
	  }
	  srand(time(NULL) - (getpid()*2*randNum));
	  result = (rand()%(hi_num-low_num))+low_num;
	  return result;  
  }
  else
  {
	  int randNum = min_num;
	  int result =0,low_num=0,hi_num=0;
	  if(min_num < max_num)
	  {
		low_num=min_num;
		hi_num=max_num+1;
	  }
	  else
	  {
		low_num=max_num+1;
		hi_num=min_num;
	  }
	  srand(time(NULL) - (getpid()*2*randNum));
	  result = (fast_rand()%(hi_num-low_num))+low_num;
	  return result;
  }
}
int findValidPCB()
{
	int i = 0;
	for (i; i < x; i++)
	{
		if(pcb[i].valid == 0)
		{
			if(pcb[i].userPid > 0)
			{
				printf("Please don't Happen\n");
				kill(pcb[i].userPid,SIGUSR1);
			}
			return i;
		}
	}
	return -1;
}
int getRandFork()
{
	int forkAtRand = 0;
	forkAtRand = computerTime->second + random_number(0,2);	

	return forkAtRand;
}
void setUpPCB(int i, pid_t childPid)
{
	pcb[i].userPid = childPid;
	pcb[i].valid = 1;
	pcb[i].fullRunTime = random_number(childPid+100000,childPid+100000000000000);
	pcb[i].timeInSys = 0;
	pcb[i].turn = -1;

	int chance = 0;
	chance = random_number(0,99);
	
	if(chance < 15)
	{
		pcb[i].priority = 0;
		pcb[i].timeQuantum = 5000000;
		insertINqueue(queue0, i);
	}
	else
	{
		pcb[i].priority = 1;
		pcb[i].timeQuantum = 3000000;
		insertINqueue(q1, i);
	}
}
void resetPCB(int i)
{
	kill(pcb[i].userPid,SIGUSR1);
	pcb[i].userPid = 0;
	pcb[i].valid = 0;
	pcb[i].fullRunTime = 0;
	pcb[i].timeInSys = 0;
	pcb[i].turn = -1;
	pcb[i].timeQuantum = 0;
	pcb[i].priority = 0;
	pcb[i].intIO = 0;
}
int getIO(pid_t childPid)
{
	int chance = 0;
	chance = random_number(0,99);
	if(chance <=9)
	{
		ioCount++;
		return 1;
	}
	else 
	{
		return 0;
	}
}

void printToFile(int choice, int index)
{
	if(printLineCount >= 1000 && choice != 99)
		return;
	else
		printLineCount++;
	
	
	int c = choice;
	switch (c)
	{
		case 0:
			fprintf(filePtr,"\tComputerTime: %d.%010d | ", computerTime->second,computerTime->nanoSec);
			fprintf(filePtr,"Child Death Pid: %d | valid: %d | Queue %d | timeQuantum %d | ",pcb[index].userPid,pcb[index].valid,pcb[index].priority,pcb[index].timeQuantum);
			fprintf(filePtr,"timeInSys %d | FullRunTime %d\n", pcb[index].timeInSys, pcb[index].fullRunTime);
			break;
		case 1:
			fprintf(filePtr,"->");
			fprintf(filePtr,"Child Created: %d.%010d | ", computerTime->second,computerTime->nanoSec);
			fprintf(filePtr,"Process #%d Pid: %d", index,pcb[index].userPid);
			fprintf(filePtr," Am I valid: %d ", pcb[index].valid);
			fprintf(filePtr," Queue: %d ", pcb[index].priority);
			fprintf(filePtr," FullRunTime: %d\n", pcb[index].fullRunTime);
			break;
		case 2:
			fprintf(filePtr, "\t\t\tChild Pid %d in Q3 Had IO Moving to Q1 \n", pcb[index].userPid);
			break;
		case 3:
			fprintf(filePtr, "\t\t\tChild Pid %d in Q2 Had IO Moving to Q1 \n", pcb[index].userPid);
			break;
		case 4:
			fprintf(filePtr,"\t\t\t\tChild Pid %d Moving form Q2 to Q3\n", pcb[index].userPid);
			break;
		case 5:
			fprintf(filePtr,"\t\t\tI/O called Priority Q%d | Pid %d | at Time During Execution: %d\n", pcb[index].priority, pcb[index].userPid, pcb[index].randIOTime);
			break;
		case 99:
			fprintf(filePtr, "\n------Statistics------------\n");
			printf("\n------Statistics------------\n");
			
			fprintf(filePtr, "Program End time------------------:%d.%010d\n", computerTime->second, computerTime->nanoSec);
			printf("Program End time------------------:%d.%010d\n", computerTime->second, computerTime->nanoSec);
			
			fprintf(filePtr, "\nTotal Process Made----------------:%d\n",processMade);
			printf("\nTotal Process Made----------------:%d\n",processMade);
			
			fprintf(filePtr, "\nTotal Process Finished Execution--:%d\n", processFinished);
			printf("\nTotal Process Finished Execution--:%d\n", processFinished);
			
			
			fprintf(filePtr, "\nTotal IO count -------------------:%d\n", ioCount);
			printf("\nTotal IO count -------------------:%d\n", ioCount);
			
			
			fprintf(filePtr, "\nIdle Time-------------------------:%d.%010d\n", computerTime->idleTimeSec,computerTime->idleTimeNano);
			printf("\nIdle Time-------------------------:%d.%010d\n", computerTime->idleTimeSec,computerTime->idleTimeNano);
			
			
			fprintf(filePtr, "\nTotal OverHead Time---------------:%d.%010d\n", computerTime->overHeadSec,computerTime->overHeadNano);
			printf("\nTotal OverHead Time---------------:%d.%010d\n", computerTime->overHeadSec,computerTime->overHeadNano);
			
			fprintf(filePtr, "\nTotal In System time--------------:%d.%010d\n", computerTime->TotalSecInSys,computerTime->TotalNanoInSys);
			printf("\nTotal In System time--------------:%d.%010d\n", computerTime->TotalSecInSys,computerTime->TotalNanoInSys);
			
			
			fprintf(filePtr, "\nTotal user wait time--------------:%d.%010d\n", computerTime->userWaitSec,computerTime->userWaitSec);
			printf("\nTotal user wait time--------------:%d.%010d\n", computerTime->userWaitSec,computerTime->userWaitSec);
			
			calc(processMade,computerTime->TotalSecInSys,computerTime->TotalNanoInSys);
			calc2(processMade,computerTime->userWaitSec,computerTime->userWaitSec);
			break;
	}
}
void calc (int pm, int sec, int nsec)
{
	int ans = 0;
	int ans2 = 0;
	int remainder = 0;
	remainder = sec % pm;
	ans = sec / pm;
	ans2 = (nsec+ remainder) / pm;	
	fprintf(filePtr, "\nAverage TurnAroundTime------------:%d.%010d\n", ans,ans2);
	printf("\nAverage TurnAroundTime------------:%d.%010d\n", ans,ans2);
}
void calc2 (int pm, int sec, int nsec)
{
	int ans = 0;
	int ans2 = 0;
	int remainder = 0;
	remainder = sec % pm;
	ans = sec / pm;
	ans2 = (nsec+ remainder) / pm;	
	fprintf(filePtr, "\nAverage Wait time-----------------:%d.%010d\n",ans,ans2);
	printf("\nAverage Wait time-----------------:%d.%010d\n",ans,ans2);
}

