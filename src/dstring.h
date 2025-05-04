#ifndef DSTRING_H
#define DSTRING_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "datatype.h"
#include "mdarray.h"
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

dstring ds_new_string_initialize(const char *s);

dstring ds_new_string_mem(void* mem, uint length);

dstring ds_random(int length);

dstring ds_concat(dstring str1, dstring str2);

dstring ds_cstrconcat(const char* str1, const char* str2);

dstring ds_bcstrconcat(dstring str, const char* cstr);

dstring ds_bcharconcat(dstring str, char ch);

dstring ds_fcstrconcat(const char* cstr, dstring str);

dstring ds_fcharconcat(char ch, dstring str);

int ds_truncate(dstring *dest, unsigned int truncated_len);

int ds_remove_last_n(dstring* str, unsigned int to_remove);

void ds_print(dstring s, FILE *file);

void ds_delete(dstring *del);

void ds_delete_dynamically_allocated_dstring(void* str);

int ds_copy(dstring* dest, dstring src);

dstring ds_substring(dstring string, uint start, uint end);

int ds_find_last_char_occurance(dstring string, char character);

int ds_find_character(dstring string, char character);

int ds_find_character_start_at(dstring string, uint start, char character);

char ds_at(dstring string, uint index);

mdarray ds_split(dstring str, char delim);

void ds_replace_char(dstring str, char old_char, char new_char);

int ds_count_char(dstring str, char target_char);

//TODO:
// create better tests
//functions to make:
// string.replace (need regex for more complicated replace)

////////////////////////////////////////////////////////////
// const char* functions
////////////////////////////////////////////////////////////

dstring s_remove_last_n(char* str, unsigned int to_remove);

dstring s_substring(char* string, uint start, uint end);

int s_find_last_char_occurance(char* string, char character);

int s_find_character(char* message, char character);

int s_find_character_start_at(char* message, uint start, char character);

char s_at(char* string, uint index);


#endif
