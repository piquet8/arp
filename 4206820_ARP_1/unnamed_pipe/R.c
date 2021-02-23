#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <errno.h> 
#include <time.h> 

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
	 int wP,rP, wP2, rP2, wP3, rP3;
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


	 //conversion of argv from char to integer 
	 rP = atoi(argv[1]);
	 wP = atoi(argv[2]);
	 rP2 = atoi(argv[3]);
	 wP2 = atoi(argv[4]);
	 rP3 = atoi(argv[5]);
	 wP3 = atoi(argv[6]);

	 close(wP); //close file descriptor writing pipe 1
	 close(wP2); //close file descriptor writing pipe 2
	 close(rP3); //close file reading reading pipe 3
	 int number=0;

	while(stop!=1) 
	{
	FD_ZERO(&rfds);
	FD_SET(rP, &rfds);
	FD_SET(rP2, &rfds);

	retval=select(rP2+1, &rfds, NULL, NULL, NULL);  //selecting which pipe is ready to be read

	if (retval==-1) 
		perror("Select()");

	else if (retval) {

		
		if(FD_ISSET(rP, &rfds) && FD_ISSET(rP2, &rfds)) { //if pipe1 and pipe2 are ready at the same time
		logFile("pipe 1 and 2 are ready: ");
			random=rand_r(&v);

				if(random%2==0) {
					byte1=read(rP, &buffer[number], sizeof(buffer)); //read pipe 1
					if(byte1>0) 
 						number++;
 					if (byte1 < 0) { //if read value is not valid
						perror("Read error\n");
						return -1;
					}
					byte2=read(rP2, &buffer[number], sizeof(buffer)); //then read pipe 2
					if(byte2>0) 
 						number++;
 					if (byte2 < 0) { //if read value is not valid
						perror("Read error\n");
						return -1;
					}
				}
				else {
					byte2=read(rP2, &buffer[number], sizeof(buffer)); //read pipe 2
					
					if(byte2>0) 
 						number++;
 					if (byte2 < 0) { //if read value is not valid
						perror("Read error\n");
						return -1;
					}
					byte1=read(rP, &buffer[number], sizeof(buffer)); // read pipe 1
					
					if(byte1>0) 
 						number++;
 					if (byte1 < 0) { //if read value is not valid
						perror("Read error\n");
						return -1;
					}
				}
		}

		else {

			if (FD_ISSET(rP, &rfds)) {        //if only pipe 1 is ready
				byte1=read(rP, &buffer[number], sizeof(buffer)); //read pipe 1
				logFile("only pipe 1 ready: ");
				if(byte1>0) 
					number++;
	 			if (byte1 < 0) {       
					perror("Read error\n");
					return -1;
				}
			}

			if (FD_ISSET(rP2, &rfds)) { //if is ready only pipe 2
				byte2=read(rP2, &buffer[number], sizeof(buffer)); //read pipe 2
				logFile("only pipe 2 ready: ");
				if(byte2>0)
	 				number++;
	 			if (byte2 < 0) {
					perror("Read error\n");
					return -1;
				}
			}
		}
	}


 	if(number==DIM) 
		{ //the buffer is full

 		data = write(wP3, &buffer, sizeof(buffer)); //writing on pipe 3
		logFile("R sends one package to M: ");
		number=0;       //restart to fill the buffer
 		if (data < 0) { //if written value is not valid
			perror("Write error\n");
			return -1;
		}	

	}
 }
 close(rP); //close file descriptor reading pipe 1
 close(rP2); //close file descriptor reading pipe 2
 close(wP3); //close file descriptor writing pipe 3
 printf("R File descriptors closed\n");
 fflush(stdout);
 logFile("R closes file descriptors: ");

exit(0);

}