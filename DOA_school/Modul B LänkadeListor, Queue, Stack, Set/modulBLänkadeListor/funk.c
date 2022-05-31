#include "Header.h"

node* findEnd(node* List);

void createNode(node** List)
{
	if (*List == NULL)
	{
		node* newNode = (node*)malloc(sizeof(node));
		if (newNode != NULL)
		{
			newNode->next = NULL;
			newNode->prev = NULL;
			*List = newNode;
			return;
		}
		else
		{
			printf("Error\n");
		}
	}
	else
	{
		node* endAdress = findEnd(*List);
		node* newNode = (node*)malloc(sizeof(node));
		if (newNode != NULL)
		{
			newNode->prev = endAdress;
			endAdress->next = newNode;
			newNode->next = NULL;
		}
		else
		{
			printf("Error\n");
		}
	}
}
node* findEnd(node* List)
{
	node* prev = NULL;
	while (List != NULL)
	{
		prev = List;
		List = List->next;
	}
	return prev;
}
void print(node* subList)
{
	if (subList == NULL)
		return;
	printf("%d\n", (*subList).data);
	print(subList->next);
}