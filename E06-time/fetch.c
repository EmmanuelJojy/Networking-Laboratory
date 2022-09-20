#include <stdio.h>
#include <time.h>

void main() {
	time_t now = time(0);
	char *buff = ctime(&now);
	printf("%s\n", buff);
}
