#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <errno.h> 
#include <time.h> 


void logFile(char *msgStatus) {
    FILE *f;
    f = fopen("logFileEs1.log", "a+");
    time_t currentTime;
    char* timeString;
    currentTime = time(NULL);
    timeString = ctime(&currentTime);
    fprintf(f, "%sPID %d. %s %s\n",timeString, getpid(), msgStatus, strerror(errno));
    fclose(f);
} //creation of the log file


struct message {  //dichiarazione struct
 time_t timestamp; //actual time 
 char g;  //process identifier
 int x;  //sequence number of the message
};

int main(int argc, char *argv[])
{
 char readPipe[5]; //string for reading pipe1
 char writePipe[5]; //string for writing pipe1
 char readPipe2[5]; //string for reading pipe2
 char writePipe2[5]; //string for writing pipe2
 char readPipe3[5]; //string for reading pipe3
 char writePipe3[5]; //string for writing pipe3
 int fd[2]; //file descriptor pipe1
 int fd1[2]; //file descriptor pipe2
 int fd2[2]; //file descriptor pipe3
 int pipeState, pipeState2, pipeState3;
 pid_t G1, G2, R, M;

 FILE *f;
 f = fopen("logFileEs1.log", "w");
 fclose(f);

 pipeState = pipe (fd);	//creation first pipe
 logFile("Creation of the first pipe: ");
 if (pipeState < 0) {
	perror("Pipe error");
	return -1;
 }

 pipeState2 = pipe (fd1); //creation second pipe
 logFile("Creation of the second pipe: ");
 if (pipeState2 < 0) {
	perror("Pipe error");
	return -1;
 }

 pipeState3 = pipe (fd2); //creation second pipe
 logFile("Creation of the third pipe: ");
 if (pipeState2 < 0) {
	perror("Pipe error");
	return -1;
 }

 //fd conversion integer --> string
 sprintf(readPipe, "%d", fd[0]);
 sprintf(writePipe, "%d", fd[1]);

 //fd1 conversion integer --> string
 sprintf(readPipe2, "%d", fd1[0]);
 sprintf(writePipe2, "%d", fd1[1]);

 //fd1 conversion integer --> string
 sprintf(readPipe3, "%d", fd2[0]);
 sprintf(writePipe3, "%d", fd2[1]);


 //string for function execve
 char *function1[] = {"g1", readPipe, writePipe, NULL};
 char *function2[] = {"g2", readPipe2, writePipe2, NULL}; 
 char *function3[] = {"R", readPipe, writePipe, readPipe2, writePipe2, readPipe3, writePipe3, NULL}; 
 char *function4[] = {"M", readPipe3, writePipe3, NULL}; 

  

 G1= fork(); //creation first child

 if (G1 != 0) {
	logFile("Creation of the g1: ");
 }

 if (G1 < 0) {
	perror 	("Fork error in g1");
	return -1;
 }

 else if (G1 == 0) {
	

	execve("g1", function1,NULL);
	
	exit (EXIT_SUCCESS);
 }
	
 G2 = fork();  //creation second child

 if (G2 != 0) {
	logFile("Creation of the g2: ");
 }

 if (G2 < 0) {
	perror ("Fork error in g2");
	return -1;
 }

 else if (G2 == 0) { 

	execve("g2", function2, NULL);

	exit (EXIT_SUCCESS);
 }

 R=fork(); //creation third child

 if (R != 0) {
	logFile("Creation of the R: ");
 }

 if (R < 0) {
	perror ("Fork error in R");
	return -1;
 }

 else if (R == 0) { 

	execve("R", function3, NULL);

	exit (EXIT_SUCCESS);
 }

 M=fork(); //creation fourth child

 if (M != 0) {
	logFile("Creation of the M: ");
 }

 if (M < 0) {
	perror ("Fork error in M");
	return -1;
 }

 else if (M == 0) { 

	execve("M", function4, NULL);

	exit (EXIT_SUCCESS);
 }
		
 else { 

	int stop=0;
	printf("PRESS 1 TO TERMINATE THE EXECUTION\n");
	scanf("%d", &stop);

	if (stop==1) {
		kill(G1, SIGUSR1);
		kill(G2, SIGUSR1);
		kill(R, SIGUSR1);
		kill(M, SIGUSR1);
		exit (EXIT_SUCCESS);
	logFile("Stop: ");
	}	
 sleep(1);	
 }
 return 0;
}

