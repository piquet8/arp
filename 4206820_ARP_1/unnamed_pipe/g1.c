#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <errno.h> 
#include <time.h> 
#define OFFSET 2

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


int stop=0;
void sig_handler(int n_signal) 
{
	stop=1;
} 
struct timeval times;
struct message
{  //struct declaration 
	time_t timestamp;  
	char g;  //process identifier
	int x;
};

int main(int argc, char *argv[])
{
	 int j=0;
	 long int delay;
	 int wP,rP;
	 int data;
	 long int offset=OFFSET;
	 unsigned int seed=7; //for rand_r

	 struct sigaction act;
	 memset(&act, 0, sizeof(act));
	 act.sa_handler=&sig_handler;
	 sigaction(SIGUSR1, &act, NULL);  //function to handle the SIGUSR1 signal

	 logFile("Starting g1: ");
	 //conversion of argv from char to integer 
	 wP = atoi(argv[2]);
	 rP = atoi(argv[1]);
 
	 struct message message1;

	 while (stop!=1)
	 {
		 gettimeofday(&times,NULL);
		 message1.timestamp=times.tv_sec*1000000+ times.tv_usec;
		 message1.x=j;
		 message1.g='1';
		
		 data = write(wP, &message1, sizeof(message1)); //writing on the first pipe
	 
		 if (data < 0)
			 { 
				 perror("Write error\n");
				 return -1;
			 }

		j++;
		//computation of the delay
		delay=offset+rand_r(&seed)%offset;
		usleep(delay);
	}
	close(rP); //close file descriptor for reading
	close(wP); //close file descriptor for writing
	printf("G1 File descriptors closed\n");
	fflush(stdout);
	logFile("G1 closes file descriptors: ");
	exit(0);

}
