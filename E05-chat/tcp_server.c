#include <sys/socket.h>
#include <arpa/inet.h> // sockaddr_in, inet_addr, INADDR_ANY
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define PORT 6543
#define LOCA "127.0.0.1"

void handle(int sd)
{
	while(1)
	{
		// Step 5
		// Read from socket/ file descriptor data
		char buffer[128];
		read(sd, &buffer, sizeof(buffer));
		if(!strcmp(buffer, "STOP")) break;
		
		// Step 6
		// Write to socket/ file descriptor
		write(sd, &buffer, sizeof(buffer));
	}
	printf("Disconnected: %d\n", sd);
	close(sd);
	exit(0);
}
	

void main()
{
	printf("TCP SERVER\n\n");

	// Step 1
	// Create socket
	int sd;
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Socket Creation Failed (Error: %d)\n", sd);
		exit(0);
	}
	
	// Step 2
	// Bind Socket
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to ALL interfaces
	//server.sin_addr.s_addr = inet_addr(LOCA); // Bind to ONLY localhost
	int ret;
	if((ret = bind(sd, (struct sockaddr *)&server, sizeof(server))) < 0)
	{
		printf("Socket Binding Failed (Error: %d)\n", ret);
		exit(0);
	}
	
	// Step 3
	// Set socket to listen mode
	listen(sd, 5);
	
	// Step 4
	// Wait for new connection. (Blocking call).
	struct sockaddr_in client;
	int client_length, data, pid;
	while(1)
	{
		
		if((data = accept(sd, (struct sockaddr *)&client, &client_length)) < 0)
		{
			printf("Could not accept connection (Error: %d)\n", data);
			exit(0);
		}
		printf("Connected   : %d\n", data);
		pid = fork();
		if(!pid) handle(data);
		else continue;
	}		
	close(data);
	close(sd);
}

			
