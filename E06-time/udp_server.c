#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <errno.h>

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
	printf("(Step 1 of 4) Socket Created\n");

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
		// dummy recieve
		printf("Request Recieve Error (Error: %d)\n", n);
		exit(0);
	}
	printf("(Step 3 of 4) Request recieved: %s\n", buffer);
	
	// TIME PROCESSING
	// man 2 time
	// man ctime
	time_t epoch_seconds = time(NULL);
	struct tm tm = *localtime(&epoch_seconds);
	sprintf(buffer, "%d/%d/%d %d:%d:%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
	
	if((ret = sendto(sd, buffer, strlen(buffer), 0, (struct sockaddr *)&client, client_len)) < 0)
	{
		printf("Time Send Error (Error: %s)\n", strerror(errno));
		exit(0);
	}
	printf("(Step 4 of 4) Time send (%s)\n", buffer);

	close(sd);
}
