#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define PORT 6543
#define LOST "127.0.0.1"

void main() 
{
	printf("UDP SERVER\n");
	int sd;
	if((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("Socket Creation Failed (Error: %d)\n", sd);
		exit(0);
	}
	printf("(Step 1 of 4) Socket Craeted\n");

	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(LOST);
	int ret;
	if((ret = bind(sd, (struct sockaddr *)&server, sizeof(server))) < 0)
	{
		printf("Socket bind failed (Error: %d)\n", ret);
		exit(0);
	}
	printf("(Step 2 of 4) Socket binded\n");
	
	char buffer[128];
	struct sockaddr_in client;
	int client_len = sizeof(client);
	memset(&client, 0, sizeof(client));
	int n;
	if((n = recvfrom(sd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &client_len)) < 0)
	{
		printf("Message Recieve Error (Error: %d)\n", n);
		exit(0);
	}
	buffer[n] = '\0';
	printf("\tRECV << %s\n", buffer);
	printf("(Step 3 of 4) Message recieved (%d bytes)\n", n);
	
	if((ret = sendto(sd, buffer, strlen(buffer), 0, (struct sockaddr *)&client, client_len)) < 0)
	{
		printf("Message Send Error (Error: -1)\n");
		exit(0);
	}
	printf("(Step 4 of 4) Message send (%ld bytes)\n", strlen(buffer));

	close(sd);
}
