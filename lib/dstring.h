#ifndef DSTRING_H
#define DSTRING_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "datatype.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef char *dstring;

dstring ds_new_string(int length);

dstring ds_random(int length);

dstring ds_new_string_initialize(const char *s);

dstring ds_concat(dstring str1, dstring str2);

int ds_truncate(dstring *dest, unsigned int truncated_len);

void ds_print(dstring s, FILE *file);

void ds_delete(dstring *del);

int ds_copy(dstring* dest, dstring src);

dstring ds_substring(dstring string, uint start, uint end);

int ds_find_character(dstring string, char character);

int ds_find_character_start_at(dstring string, uint start, char character);

//TODO:
// Fix this implementation
// create better tests
//functions to make:
// string.split
// string.replace

#endif
