#include <stdio.h>
void main()
{
	char buffer[128];
	while(1)
	{
		scanf("%[^\n]s", buffer);
		printf("%s\n", buffer);
	}
}
