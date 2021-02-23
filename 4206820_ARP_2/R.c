/*Authors: Gianluca Piquet 4206820
	   Maria Luisa Aiachini 4375373
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>

int sockfd, buffer;

void cmd_handler(int signum){

	printf("\n");
	printf("-----------------------------------------------------\n");
	printf("Comandi: \n 0=STOP\n 1=UP\n 2=DOWN\n 3=OFF\n");
	printf("-----------------------------------------------------\n");
	printf("Please insert command: \n");
	scanf("%d",&buffer);
	printf("You choose this command --->");
	
    	if (buffer == 0){
     		printf(" STOP\n");
   	 }
    	else if(buffer == 1){
    	 	printf(" UP\n");
   	}
  	else if(buffer == 2){
     		printf(" DOWN\n");
    	}
     	else if(buffer == 3){
     		printf(" OFF\n");
    	}
     	else{
     		printf("Invalid command\n");
     	}
			 
	if ((write(sockfd, &buffer, sizeof(int))) < 0){   //write socket from R to S
		perror("ERROR in enter a command");
		exit(-1);
	}
}

int main(int argc, char *argv[]){

	int lettura, n, k;
	
	printf("-----------------------------------------------------\n");
	printf("PRESS CTRL + Z TO CHANGE COMMAND\n");
	printf("-----------------------------------------------------\n");
	printf("Command: \n 0=STOP\n 1=UP\n 2=DOWN\n 3=OFF\n");
	printf("-----------------------------------------------------\n");
	printf("Please insert command: \n");
	scanf("%d", &buffer);
	printf("You choose this command --->");
    
	if (buffer == 0){
		printf(" STOP\n");
	}
	else if(buffer == 1){
		printf(" UP\n");
	}
	else if(buffer == 2){
		printf(" DOWN\n");
	}
	else if(buffer == 3){
		printf(" OFF\n");
		printf("\n");
		exit(-1);
	}
	else{
		printf("Invalid Command\n");
	}
    
//-------------------------------------------------------------------------
    
	int portno;
    
	struct sockaddr_in serv_addr;
	struct hostent *server;

	if (argc < 3){
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}
    
	signal(SIGTSTP,cmd_handler);

	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		perror("ERROR opening socket");
	}
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
		perror("ERROR connecting");
	}

//-------------------------------------------------------------------------------

while (1){ 
	
	n = write(sockfd,&buffer,sizeof(buffer));  //write socket from R to S
    	if (n < 0){
        	perror("ERROR writing to socket");
      	}
	 
    	k = read(sockfd, &lettura, 255);  //read socket from S
     
     	printf("Z POSITION --> %d\n", lettura);
     	if ((lettura >= 200) && (buffer == 1)){
		printf("END RUN! You cannot continue in this direction\n");
		printf("PLEASE PRESS CTRL+Z AND CHANGE DIRECTION\n");
		pause();
	}
	else if((lettura<=0) && (buffer ==2)){
		printf("END RUN! You cannot continue in this direction\n");
		printf("PLEASE PRESS CTRL+Z AND CHANGE DIRECTION\n");
		pause();
	}
     	else if((buffer  != 0) && (buffer  != 1) && (buffer  != 2)){
     		break;
     	}
}
	return 0;
}

