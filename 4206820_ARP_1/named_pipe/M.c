#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>
#include <errno.h> 
#include <fcntl.h>
#define DIM 10000  //dimension of buffer

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

time_t times[2];
int stop=0;
void sig_handler() 
{
 	stop=1;
}

struct message
{
	time_t timestamp;
	char g;
	int x;
};

int i=1;
struct message client_message[DIM];
void error(char *msg);
double find_throughput(int ng, time_t timestamp[2]);
void take_timestamp(struct message client_message[DIM],int dimension,long int timest[2]);

int main(int argc, char *argv[])
{
   char *pipe2 = "/tmp/pipe2";
   int rP;	
   int throughput;
   struct sigaction act;
   memset(&act, 0, sizeof(act));
   act.sa_handler=&sig_handler;
   sigaction(SIGUSR1, &act, NULL);  //function to handle the SIGUSR1 signal
   FILE * ft;
   rP = open(pipe2, O_RDONLY);
   
   while(stop!=1)
    {
       int n;
      	logFile("Start reading package in M: ");
       read (rP, &client_message, sizeof(client_message)); // read the buffer
	printf("\n--Package number %d\n",i);
       fflush(stdout);
       printf(".............M.............\n");
	fflush(stdout);

       take_timestamp(client_message,DIM,times); //find the minum and the max timestamp
      	logFile("Find Timestamp: ");
	for(int i = 0; i<DIM; i++)
	{
           printf("%d send by %c \n",client_message[i].x,client_message[i].g);
	}
      	logFile("Open Report: ");
	ft = fopen("report.txt", "a+");			// open the file for report the data
	fprintf(ft, "\n\n------------------------------------------------------\n\n" );
	fflush(ft);

	printf("\nTimestamp:  \nmin = %ld us\nmax = %ld us\n", times[0],  times[1]);
	fflush(stdout);

	fprintf(ft,"\nTimestamp: \nmin = %ld us\nmax = %ld us\n", times[0],  times[1]);
	fflush(ft);

	throughput= find_throughput(DIM, times); // calculate the throughput
	
	printf("\nThroughput package %d  = %d bytes/s \n",i,throughput);
	fflush(stdout);

	fprintf(ft,"\nThroughput package %d = %d bytes/s \n",i,throughput);
      	logFile("Print throughput: ");
	fflush(ft);
       i++;
}
  close(rP);
  fclose(ft);				// Close the file
  unlink(pipe2);
  return 0;
}



double find_throughput(int number, time_t timestamp[2])
{
   int throughput=0;
   throughput=number*1000000*sizeof(struct message)/(timestamp[1]-timestamp[0]);
   return throughput;
}

void take_timestamp(struct message client_message[DIM],int dimension,long int timest[2])
{
   
   int q;
   timest[1]=client_message[0].timestamp;
   
   for(q=0;q<dimension;q++)
    {
      if(timest[1]<client_message[q].timestamp)
           timest[1]=client_message[q].timestamp;
    }
 
   timest[0]=client_message[0].timestamp;

   for(q=0;q<dimension;q++)
    {
      if(timest[0]>client_message[q].timestamp)
           timest[0]=client_message[q].timestamp;
    }

}


void error(char *msg)
{
  perror(msg);
  exit(1);
}