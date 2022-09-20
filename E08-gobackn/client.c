#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 6543
#define LOST "127.0.0.1"

int sd, size = 4;
char data[128][128], buff[128];

void noack(int cur, int flag) {
	// Initial window packets
	for(int i = cur; i < cur + size; i++) {
		if(flag) {
			printf("\tInput: ");
			scanf("%s", data[i]);
			getchar();
		}
		write(sd, data[i], 128);
		printf("\tSEND: %s\n", data[i]);
	}
}

void gobackn() {
	noack(0, 1);
	
	int cur = size;
	while(1) {
		read(sd, buff, 128);
		printf("\n\tRECV: %s\n", buff);
		// Check if negative ack
		if(buff[1] == 'N') {
		printf("\n||%d||\n", cur);
			noack(cur - size, 0);
			continue;
		}
		printf("\tInput: ");
		scanf("%s", data[cur]);
		write(sd, data[cur], 128);
		printf("\tSEND: %s\n", data[cur]);
		cur++;
	}
}		

void main() {
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Socket Creation Error.\n");
		return;
	}
	
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(LOST);
	if(connect(sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		printf("Client Connection Error.\n");
		return;
	}
	
	gobackn();
	
	close(sd);
}
