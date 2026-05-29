#include <unistd.h>
#include <stdio.h> 
int main()
{
	printf("1111\n");
	fork();
	printf("2222\n");
	fork();
	printf("3333\n");
	fork();
	printf("4444\n");

	return 0;
}