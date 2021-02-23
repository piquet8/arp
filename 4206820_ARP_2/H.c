/*Authors: Gianluca Piquet 4206820
	   Maria Luisa Aiachini	4375373
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/select.h>

int main(int argc, char *argv[]) {

	int cmd;
	int z=0; //initialization of z
	
	char id;
	sscanf(argv[0], "%c", &id);

	int sh;
	sscanf(argv[1], "%d", &sh);

	int hs;
	sscanf(argv[2], "%d", &hs);

	while(1){
		int n = read(sh, &cmd, sizeof(cmd)); //pipe read from S
		if(cmd == 1){
			if(z<200){
				z=z+5;
				int m = write(hs, &z, sizeof(z)); //pipe write to S the Z's position
				if (m == 0) {
					perror("Error in write h->s\n");
					exit(EXIT_FAILURE);
				}	
			sleep(1);
			}
			else if(z>=200){
				int m = write(hs, &z, sizeof(z)); //pipe write to S the Z's position		
				if (m == 0){
					perror("Error in write h->s\n");
					exit(EXIT_FAILURE);
				}	
			sleep(1);
			}
		}
		else if(cmd == 2){
			if(z>0){
				z=z-5;
				int m = write(hs, &z, sizeof(z));
				if (m == 0) {
					perror("Error in write h->s\n");	
					exit(EXIT_FAILURE);
				}
			sleep(1);
			}
			else if(z <= 0){
				int m = write(hs, &z, sizeof(z));
				if (m == 0) {
					perror("Error in write h->s\n");	
					exit(EXIT_FAILURE);
				}
			sleep(1);
			}
			
		}
		else if(cmd == 0){
			int m = write(hs, &z, sizeof(z));
			sleep(1);
		}
		else if(cmd == 3){
			printf("simulation: off\n");
			break;
		}
		else{
			break;
		}
	}
	close(sh);
	close(hs);
}
