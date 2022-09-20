#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 6543
#define LOST "127.0.0.1"

void selective(int csd) {
	int size = 4, i, curr = 0;
	char buff[128];
	for(i = 0; i < size; i++) {
		read(csd, buff, 128);
		printf("RECV << %s\n", buff);
	}
	
	while(1) {
		if(curr == 4) {
			sprintf(buff, "%dNACK", curr);
			write(csd, buff, 128);
			printf("SEND >> %s\n", buff);
			read(csd, buff, 128);
			printf("RECV << %s\n", buff);
		}
		sprintf(buff, "%dACK", curr);
		write(csd, buff, 128);
		printf("SEND >> %s\n", buff);
		read(csd, buff, 128);
		printf("RECV << %s\n", buff);
		curr++;
	}
}

void main() {
	int sd;
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Socket creation error.\n");
		return;
	}
	
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(LOST);
	if(bind(sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		printf("Binding error.\n");
		return;
	}
	
	listen(sd, 1);
	
	int csd;
	if((csd = accept(sd, NULL, NULL)) < 0) {
		printf("Could not accept incoming connection.\n");
		return;
	}
	
	selective(csd);
	
	close(csd);
	close(sd);
}
