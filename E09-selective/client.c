#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 6543
#define LOST "127.0.0.1"

void selective(int csd) {
	char data[128][128], buff[128];
	int curr, size = 4, i;
	for(curr = 0; curr < size; curr++) {
		printf("SEND << ");
		scanf("%s", data[curr]);
		write(csd, data[curr], 128);
	}
	
	while(1) {
		read(csd, buff, 128);
		printf("RECV >> %s\n", buff);
		if(buff[1] == 'N') {
			write(csd, data[curr - size], 128);
			printf("RESE: %s\n", data[curr - size]);
			continue;
		}
		printf("SEND << ");
		scanf("%s", data[curr]);
		write(csd, data[curr], 128);
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
	if(connect(sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		printf("Connection Error\n");
		return;
	}
	
	selective(sd);
	
	close(sd);
}
