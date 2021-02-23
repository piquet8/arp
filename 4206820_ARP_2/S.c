/*Authors: Gianluca Piquet 4206820
	   Maria Luisa Aiachini 4375373
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/select.h>

int main(int argc, char *argv[]){    
                
	int sockfd, newsockfd, portno, cmd = 1;
	socklen_t clilen;
	int buffer, n;
	struct sockaddr_in serv_addr, cli_addr;       
	if (argc < 2){
		fprintf(stderr,"ERROR, no port provided\n");
	exit(1);
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	//socket creation
	if (sockfd < 0){
		perror("ERROR opening socket");
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){ 
		perror("ERROR on binding");
	}
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0){
		perror("ERROR on accept");
	}
	
	int f, b;
	int sh[2], hs[2];   //pipe to comunication S->H and S<-H
	int ans = 1;
	pid_t H;
	
	if(pipe(sh) < 0){
		perror("Error pipe\n");
		exit(EXIT_FAILURE);
	}
   	 if(pipe(hs) < 0){
		perror("Error pipe H->S\n");
		exit(EXIT_FAILURE);
	}	

	f = fork();
	
	if (f < 0 ){
		perror("Error fork\n");
		exit(EXIT_FAILURE);
	}
	else if(f == 0){
		char* args[2];
		args[1]= (char*) malloc(sizeof(char)*8);
		args[2]= (char*) malloc(sizeof(char)*8);

		sprintf(args[1], "%d", sh[0]);
		sprintf(args[2], "%d", hs[1]);
		args[3] = NULL;
	
		execve("./H", args, NULL);
		perror("Error execve\n");
		exit(EXIT_FAILURE);
	}
				
	close(hs[1]);
	close(sh[0]);
	printf("Simualation: on \n");
	
	while (1){
     
		n = read(newsockfd,&buffer,255);  //read socket from R 
		int cmd = write(sh[1], &buffer, sizeof(buffer));  //write pipe from S to H 
		if (cmd == 0) {
			perror("Error in write\n");
			exit(EXIT_FAILURE);
		}
		
		ans = read(hs[0], &b, sizeof(b));  //read pipe from H 
		n = write(newsockfd, &b,sizeof(b));  //write socket from S to R
		sleep(1);
	}		
	
	waitpid(f, NULL, 0);

	close(hs[0]);
	close(sh[1]);	
}


