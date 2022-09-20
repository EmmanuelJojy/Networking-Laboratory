#include <sys/socket.h>
#include <arpa/inet.h> // sockaddr_in, inet_addr, INADDR_ANY
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define PORT 6543
#define LOST "127.0.0.1"

void main()
{
	printf("TCP CLIENT\n");

	// Step 1
	// Create socket
	int sd;
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Socket Creation Failed (Error: %d)\n", sd);
		exit(0);
	}
	printf("(Step 1 of 4) Socket Created. Socket ID = %d\n", sd);
	
	// Step 2
	// Connect to socket
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(LOST);
	int ret;
	if((ret = connect(sd, (struct sockaddr *)&server, sizeof(server))) < 0) 
	{
		printf("Server Connection Failed (Error: %d)\n", ret);
		exit(0);
	}

	char buffer[128];
	printf("(Step 2 of 4) Server connected\n");


	printf("\tSEND >> ");
	scanf("%s", buffer);
	write(sd,  buffer, sizeof(buffer));
	printf("(Step 3 of 4) Write to socket complete\n");

	read(sd, buffer, sizeof(buffer));
	printf("\tRECV << %s", buffer);
	printf("\n(Step 4 of 4) Read from socket complete\n");

	close(sd);
}