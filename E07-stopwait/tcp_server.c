#include <sys/socket.h>
#include <arpa/inet.h> // sockaddr_in, inet_addr, INADDR_ANY
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define PORT 6543
#define LOCA "127.0.0.1"

void stopwait(int sd, int n)
{
	char frame[128], seq = '0', ack[32];
	int count = 1, len;
	
	while(1)
	{
		len = read(sd, &frame, sizeof(frame));
		frame[len] = '\0';
		if(frame[0] == seq && count % n)
		{
			printf("\tSequence Match : %c\n", seq);
			printf("\tFrame Recieved : [%s]\n", frame);
			sprintf(ack, "%cACK\0", seq);
			seq = seq == '0' ? '1' : '0';
		}
		else if(frame[0] != seq)
		{
			printf("\tSequence Wrong : %c\n", seq);
			printf("\tFrame Recieved : [%s]\n", frame);
			sprintf(ack, "%cNACK\0", seq);
		}
		else
		{
			// Negative acknowledgement on purpose.
			printf("\tCycle Acknowle : %c\n", seq);
			printf("\tFrame Recieved : [%s]\n", frame);
			sprintf(ack, "%cNACK\0", seq);
		}
		
		write(sd, &ack, sizeof(ack));
		printf("\tAcknowledgement: [%s]\n\n", ack);
		count++;
		
		if(frame[1] == 'E' && frame[2] == 'X' && frame[3] == 'I' && frame[4] == 'T')
			return;
				
	}
}	

void main()
{
	printf("TCP SERVER\n");

	// Step 1
	// Create socket
	int sd;
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Socket Creation Failed (Error: %d)\n", sd);
		exit(0);
	}
	printf("Socket Created (sd = %d)...\n", sd);
	
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
	printf("Bind Complete...\n");
	
	// Step 3
	// Set socket to listen mode
	listen(sd, 1);
	printf("Socket set to listen mode...\n");	
	
	// Step 4
	// Wait for new connection. (Blocking call).
	struct sockaddr_in client;
	int client_length, data;
	memset(&client, 0, sizeof(client));
	if((data = accept(sd, (struct sockaddr *)&client, &client_length)) < 0)
	{
		printf("Could not accept connection (Error: %d)\n", sd);
		exit(0);
	}
	printf("Client connection accepted...\n");
	printf("Stop and wait sequence...\n");
	printf("Initial Sequence = 0, Cycle = 5\n\n");
	
	stopwait(data, 5);
	
	close(data);
	close(sd);
}

			
