#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <errno.h> 
#include <time.h> 
#include <fcntl.h>
#define DIM 10000


void logFile(char *msgStatus) 
{
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
void sig_handler() 
{
	 stop=1;
} 

struct message {  //declaration of the struct
 	time_t timestamp;  
 	char g;  //process identifier
 	int x;
};

int main(int argc, char *argv[])
{
	 char *pipe1 = "/tmp/pipe1"; // named pipe1 path
     char *pipe2 = "/tmp/pipe2"; // named pipe2 path
	 int wP,rP;
	 int data, data2;

	 struct message buffer[DIM];
 	 fd_set rfds;
	 int retval;
	 int byte1, byte2;
	 int random;
	 unsigned int v=4; //for rand_r

	 struct sigaction act;
	 memset(&act, 0, sizeof(act));
	 act.sa_handler=&sig_handler;
	 sigaction(SIGUSR1, &act, NULL);  //function to handle the SIGUSR1 signal


	 rP = open(pipe1, O_RDONLY); //open named pipe for read only
	 wP = open(pipe2, O_WRONLY); //open named pipe for write only
	 
	 int number=0;

while(stop!=1) 
	{
	FD_ZERO(&rfds);
	FD_SET(rP, &rfds); //una sol apipe

	retval=select(rP+1, &rfds, NULL, NULL, NULL);  //selecting which pipe is ready to be read

	if (retval==-1) 
		perror("Select()");

	else if (retval) {
		
		if(FD_ISSET(rP, &rfds) ) { 
    
            byte1=read(rP, &buffer[number], sizeof(buffer)); //read pipe 1
            if(byte1>0)
                number++;
                if (byte1 < 0) { //if read value is not valid
                    perror("Read error\n");
                    return -1;
                }
            }
    }

 	if(number==DIM) 
		{ //the buffer is full

 		data = write(wP, &buffer, sizeof(buffer)); //writing on pipe 2
		logFile("R sends one package to M: ");
		number=0;       //restart to fill the buffer
 		if (data < 0) { //if written value is not valid
			perror("Write error\n");
			return -1;
		}	

	}
 } //qua finisce il while giusto?
 close(wP); //close pipe 2
 close(rP); //close pipe 1
 unlink(pipe1); //delete pipe1
    
 printf("R File descriptors closed\n");
 fflush(stdout);
 logFile("R closes file descriptors: ");

exit(0);

}
