#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if(argv[1] == NULL)
	{
		printf("Usage: ./hex [decimal number]\n");
		exit(EXIT_FAILURE);
	}
	unsigned long long deci, remainder;
	int i, j = 0;
	deci = strtol(argv[1], (char**)NULL, 10);
	//printf("%lld", deci);
	char* hex = (char*)malloc(sizeof(char)*strlen(argv[1]));
	while (deci != 0)
	{
		remainder = deci % 16;
		printf("%lld", remainder);
		if (remainder < 10)
		{
			hex[j++] = 48 + remainder;
		}
		else
		{
			hex[j++] = 55 + remainder;
		}
		deci = deci / 16;
	}

	for (i = j; i >= 0; i--)
		printf("%c", hex[i]);
	printf("\n");
	exit(EXIT_SUCCESS);
}
