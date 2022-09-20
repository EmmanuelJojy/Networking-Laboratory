#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 6543
#define LOST "127.0.0.1"

int size = 4, csd;

void onlyread() {
	printf("\n\tRE-RECIEVE\n");
	char buff[128];
	for(int i = 0; i < size; i++) {
		read(csd, buff, 128);
		printf("\tRECV: %s\n", buff);
	}
}

void gobackn(int csd) {
	char buff[128];
	for(int i = 0; i < size; i++) {
		read(csd, buff, 128);
		printf("\tRECV: %s\n", buff);
	}
	int cur = 0;
	while(1) {
		sleep(1);
		if(cur == 4) {
			sprintf(buff, "%dNACK", cur);
			write(csd, buff, sizeof(buff));
			printf("\n\tSEND: %s\n", buff);
			onlyread();
		}
		sprintf(buff, "%dACK", cur);
		write(csd, buff, sizeof(buff));
		printf("\tSEND: %s\n", buff);
		read(csd, buff, 128);
		printf("\tRECV: %s\n", buff);
		cur++;
	}
}
void main() {
	int sd;
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Socket Creation Error.\n");
		return;
	}
	
	int ret;
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(LOST);
	if((ret = bind(sd, (struct sockaddr *)&server, sizeof(server))) < 0) {
		printf("Socket Binding Error.\n");
		return;
	}
	
	listen(sd, 1);
	if((csd = accept(sd, NULL, NULL)) < 0) {
		printf("Could not accept an incoming connection.\n");
		return;
	}
	
	gobackn(csd);
	close(csd);
	close(sd);
}
