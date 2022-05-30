#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdbool.h>

typedef unsigned long long size_t;
// these are needed for typing
typedef bool Data;
typedef int comparisonValue;


comparisonValue compareData(Data x, Data y);
void printData(size_t used, Data a[]);

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