#include <sys/socket.h>
#include <arpa/inet.h> // sockaddr_in, inet_addr, INADDR_ANY
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define PORT 6543
#define LOST "127.0.0.1"

void stopwait(int sd)
{
	char frame[128], input[128], seq = '0', ack[32];
	int nextFlag = 1;
	while(1)
	{
		if(nextFlag)
		{
			printf("\tEnter message: ");
			scanf("%[^\n]s", input);
			getchar();
			nextFlag = 0;
		}
			
		sprintf(frame, "%c%s\0", seq, input);
		write(sd, frame, sizeof(frame));
		printf("\tFrame Sent: [%s]\n");
		sleep(1);
		
		read(sd, ack, sizeof(ack));
		printf("\tAcknowledgement: %s\n", ack);
		
		if(ack[1] == 'A')
		{
			printf("\tAcknowledged Seq: %c\n\n", seq);
			nextFlag = 1;
			seq = seq == '0' ? '1' : '0';
		}
		else
		{
			printf("\tNegative Acknowledged Seq: %c\n\tAttempt to resend!\n\n", seq);
		}
		
		if(frame[1] == 'E' && frame[2] == 'X' && frame[3] == 'I' && frame[4] == 'T')
			return;
	}
}
	

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
	printf("Socket Created. Socket ID = %d...\n", sd);
	
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


	printf("Server connected...\n");
	printf("Stop and wait sequence...\n");
	printf("Initial Sequence = 0\n\n");
	
	stopwait(sd);

	close(sd);
}

