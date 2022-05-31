#pragma once
#ifndef _HEADER_H_
#define _HEADER_H_
#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
struct NODE {
	int data;
	struct NODE* next;
	struct NODE* prev;
};
typedef struct NODE node;
void createNode(node** List);
void print(node* subList);
#endif // !_HEADER_H_
