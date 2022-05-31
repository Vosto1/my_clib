#include "Header.h"

int main(void)
{
	node* head = NULL;
	node* write;
	int i = 0;
	for (i; i < 20; i++)
	{
		createNode(&head);
	}
	write = head;
	while (write != NULL)
	{
		write->data = i;
		write = write->next;
		i++;
	}
	print(head);
	printf("\n\n");
	node* prev = head;
	write = head;
	while(prev!=NULL)
	{
		write = write->next;
		free(prev);
		prev = write;
	}
	return 0;
}