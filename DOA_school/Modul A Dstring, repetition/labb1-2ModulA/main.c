#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <assert.h>

int sum(int n);

int main(void)
{
	int num;
	printf("Enter number: ");
	scanf("%d", &num);
	int res = sum(num);
	
	printf("\n\n\nAsserts:\n");

	assert(sum(1) == 1);
	assert(sum(2) == 3);
	assert(sum(3) == 6);
	assert(sum(4) == 10);
	assert(sum(5) == 15);
	assert(sum(20) == 210);
	assert(sum(0) == 0);
	assert(sum(-1) == 0);
	return 0;
}

int sum(int n)
{
	int res;
	printf("sum(%d) anropas\n", n);
	if (n == 1)
	{
		res = 1;
		printf("sum(%d) returnerar %d\n", n, res);
		return res;
	}
	if (n <= 0)
	{
		res = 0;
		printf("sum(%d) returnerar %d\n", n, res);
		return res;
	}
	res = n + sum(n - 1);;
	printf("sum(%d) returnerar %d\n", n, res);
	return res;
}