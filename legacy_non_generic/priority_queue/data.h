#ifndef DATA_H
#define DATA_H

#include <stdio.h>

typedef int priority;
typedef char element;

typedef struct {
    priority p;
    element e;
}Item;

typedef unsigned long long size_t;
// these are needed for typing
typedef priority Key; // heap
typedef Item Data; // dynamic array
typedef priority resultType; // dynamic array

// dynamic array
resultType compareData(Data x, Data y);
void printData(size_t used, Data a[]);
//heap
resultType decrementKey(Data data);

#endif


// gör ett implementation som tar hand om typer (currently data.h)
// så att det blir smidigare att definiera typer 
/*
exempel:
typedef struct {
    Key key;
    void* data_ptr;
}Data;


data_ptr ska kunna peka på vad som helst, och key är sorterings värdet

data.c?
ha compare print osv. funktioner i data.c
dessa funktioner ska anropas när heap eller dynamic array vill compare Data med en annan Data
*/