#ifndef DSTRING_H
#define DSTRING_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "datatype.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct dynamic_string
{
    char* string;
    uint length;
    uint size;
}dstring;

dstring ds_new_string(int length);

dstring ds_random(int length);

dstring ds_new_string_initialize(const char *s);

dstring ds_concat(dstring str1, dstring str2);

dstring ds_bcstrconcat(dstring str, const char* cstr);

dstring ds_bcharconcat(dstring str, char ch);

dstring ds_fcstrconcat(const char* cstr, dstring str);

dstring ds_fcharconcat(char ch, dstring str);

int ds_truncate(dstring *dest, unsigned int truncated_len);

void ds_print(dstring s, FILE *file);

void ds_delete(dstring *del);

int ds_copy(dstring* dest, dstring src);

dstring ds_substring(dstring string, uint start, uint end);

int ds_find_character(dstring string, char character);

int ds_find_character_start_at(dstring string, uint start, char character);

char ds_at(dstring string, uint index);

//TODO:
// create better tests
//functions to make:
// string.split // ... char_replace(...) (need regex for more complicated replace)
// string.replace

////////////////////////////////////////////////////////////
// const char* functions
////////////////////////////////////////////////////////////

dstring s_substring(char* string, uint start, uint end);

int s_find_character(char* message, char character);

int s_find_character_start_at(char* message, uint start, char character);

char s_at(char* string, uint index);


#endif
