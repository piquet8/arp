#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <errno.h> 
#include <time.h> 
#include <fcntl.h>


void logFile(char *msgStatus) {
    FILE *f;
    f = fopen("logFileEs1.log", "a+");
    time_t currentTime;
    char* timeString;
    currentTime = time(NULL);
    timeString = ctime(&currentTime);
    fprintf(f, "%sPID %d. %s %s\n",timeString, getpid(), msgStatus, strerror(errno));
    fclose(f);
} //function for the creation of the log file


struct message {  //declaration struct
 time_t timestamp;  
 char g;  //process identifier
 int x;
};

int main(int argc, char *argv[])
{

 char *pipe1 = "/tmp/pipe1"; // named pipe1 path
 char *pipe2 = "/tmp/pipe2"; // named pipe2 path

 pid_t G1, G2, R, M;

 FILE *f;
 f = fopen("logFileEs1.log", "w");
 fclose(f);

 //creating pipe1
 logFile("Creation of the first pipe: ");
 if(mkfifo(pipe1,00700) != 0){
     perror("ERROR in master cannot create named pipe gen-receiver");
 }
 
//creating pipe2
 logFile("Creation of the second pipe: ");
 if(mkfifo(pipe2,00700) != 0){
     perror("ERROR in master cannot create named pipe gen-receiver");
     unlink(pipe1);
 }

 //string for function execve 
 char *function1[] = {"g1", NULL};
 char *function2[] = {"g2", NULL};
 char *function3[] = {"R", NULL};
 char *function4[] = {"M",NULL};

  

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
		unlink(pipe1);  //delete pipe1
		unlink(pipe2);  //delete pipe2
		kill(G1, SIGUSR1);
		kill(G2, SIGUSR1);
		kill(R, SIGUSR1);
		kill(M, SIGUSR1);
		exit (EXIT_SUCCESS);
	logFile("Stop: ");
	}	
 sleep(5);	
 } 
return 0;
}
