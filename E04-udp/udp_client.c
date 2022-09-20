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
	printf("UDP CLIENT\n");
	int sd;
	if((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("Socket Creation Failed (Error: %d)\n", sd);
		exit(0);
	}
	printf("(Step 1 of 1) Socket Craeted\n");

	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(LOST);
		
	char buffer[128];
	printf("\tSEND >> ");
	fflush(stdout);
	scanf("%[^\n]s", buffer);
	
	sendto(sd, buffer, strlen(buffer), 0, (struct sockaddr *)&server, sizeof(server));
       	printf("(Step 2 of 3) Message send (%ld bytes)\n", strlen(buffer));
	
	int n, server_len;
	if((n = recvfrom(sd, buffer, strlen(buffer), 0, (struct sockaddr *)&server, &server_len)) < 0)
	{
		printf("Message Recieve Error (Error: %d)\n", n);
		exit(0);
	}
	buffer[n] = '\0';
	printf("\tRECV << %s\n", buffer);
	printf("(Step 3 of 3) Message recieved (%d bytes)\n", n);
	
	close(sd);
}
