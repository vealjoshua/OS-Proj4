Joshua Veal-Briscoe
Proj 4

/*******WARNING some times running make gave me an error but there was no such error, and if I ran make agian the program compiled and ran */
/******* If you can explain why, it would be much appreaciated. */


Sites used to help with project:
http://stackoverflow.com/questions/26237419/faster-than-rand ->fastRand
http://www.learn-c.org/en/Linked_lists -> creating Linked_lists

-----commands-----
-h or -help  : shows steps on how to use the program 
-s x         : x is the maximum number of slave processes spawned (default 8) 
-l filename  : change the log file name (default test.out)
-t z         : parameter z is the time in seconds when the master will terminate itself (default 20) 

-------------------Locations to change default values--------------------

X # of Process : oss.h contains the x value to change initial number of process to run | or | use command -s  | -s will not take anythig higer than 19
Z # Time restriction : oss.h contains the z value for time limit for program | or | use command -t

filename: out put file name can be changed by command -l filename or it is located in the begning of the main file


TO CHANGE ANYTHING ABOUT PCB INITIALIZATION (full run time | % chance of Priority)
GO TO function (in main file) -> void setUpPCB(int i, pid_t childPid); 

------> TIME QUANTUM CHANGE FOR Q1 - Q3  TAKES PLACE IN FUCTNIO runQ1 | runQ2 | runQ3 ------- (I know I could of made this easier but I did not have time)
------> OVER HEAD TIME FOR Q0 - Q3  TAKES PLACE IN FUCTNIO run Q0| runQ1 | runQ2 | runQ3 ------- (I know I could of made this easier but I did not have time)

I/O % CHANCE IN (in main file FUCNTION ) ->  getIO(pid_t childPid) 

USER wait time in fuction AddWaitTime in user.c


---------------------------ABOUT THE PROGRAM---------------------------
share.h contains the clock struct, pcb struct , queue struct 

OSS.H contains | includes | fucntion prototypes (with explanations of each fucntion) | Global variables | for oss.c 

user.h contains | includes | fucntion prototypes (with explanations of each fucntion) | for user.c


---------------------HOW PROGRAM WORKS --------------------------------------
The times I have chosen were to make it run most of the time up to 20 seconds of my Struct clock (I call computerTime in oss and user) //As writing this I realized is confusing haha sorry

Whlie the programm is runnning I display # of process running and the total # of process made

Stastistics are printed to both log file and to the screen

User wait time is low : 1 nanaSecond beacuse I didn't want to fiddle around withmore numbers

Time quantum in Q3 is large to actually finish of programs -> and So it doesn't get over filled and your stuck with just almost round robin 

I/O seems to trigger a lot in Q0 which also has high time quantum

I recommend using printQueue function to see whats in each queue if you like (queue always have 1000,1001,1002,1003 for me to have like a head all the time)

FastRand is used for a lot of stuff except FullRunTime beacuse it was easier to user pid to make bigger numbers and I didn't want to mess which something that worked perfectly. 

------------------------Termanology------------------------------------
FullRunTime -> total time processor needs to run | Always showed in Nano seconds
timeINsys -> time the user has spend in sys
computerTime -> the time for the systemClock | NOT ACTUALY COMPUTER TIME (hehe sorry about that realized now)
Am I valid -> if 1 then user is in Queue to run | if 0 then it is been reset 
TimeQuantum -> SHOWN IN NANOSEC | also timequantum for Queue

/__ I did not print a few things such as process in Q3 staying in Q3 beacuse it was making log file too long -||


