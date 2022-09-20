#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main() {
	int bsize, current, rate, cycle, temp, n, i;
	printf("Bucket Size: ");
	scanf("%d", &bsize);
	printf("Output Rate: ");
	scanf("%d", &rate);
	printf("Num of pkts: ");
	scanf("%d", &n);
	printf("Packet Size: ");
	int *packets = malloc(n * sizeof(int));
	for(i = 0; i < n; i++) {
		scanf("%d", &packets[i]);
	}
	
	printf("\nLeaky Bucket Simulation\n");
	i = 0;
	while(current != 0 || i != n) {
		sleep(1);
		printf("\nCycle %d [Current Queue Size = %d]:\n", cycle, current);
		// Incoming
		if(i < n) {
			printf("Incoming: %d\n", packets[i]);
			if(packets[i] <= bsize - current) {
				current += packets[i];
				printf("Accepted\n");
			}
			else 
				printf("Rejected\n");
		}
		else {
			printf("Incoming: NA\n");
		}
		// Outgoing
		temp = current - rate;
		if(temp <= 0) {
			printf("Outgoing: %d\n", current);
			current = 0;
		}
		else {
			current -= rate;
			printf("Outgoing: %d\n", rate);
		}
		// counter updation
		i = i < n ? i + 1 : i;
		cycle++;	
	}	
	
	close(packets);
}
