#include "dstring.h"

static void copy(dstring d, dstring s);

dstring ds_new_string(int length)
{
	dstring temp = (char*)malloc(sizeof(char) * length);
	assert(temp != NULL);
	return temp;
}

dstring ds_random(int length)
{
	static int min = 32;
	static int max = 125;
	dstring s = ds_new_string(length+1);
	for (int i = 0; i < length; i++)
	{
		s[i] = rand() % (max - min) + min;
	}
	s[length] = '\0';
	return s;
}

dstring ds_new_string_initialize(const char* s)
{
	assert(s != NULL);
	char* temp;
	temp = ds_new_string(strlen(s)+1);
	strcpy(temp, s);
	assert(strlen(temp) == strlen(s));
	assert(!(strcmp(temp, s)));
	return temp;
}

dstring ds_concat(dstring str1, dstring str2)
{
	assert(str1 != NULL);
	assert(str2 != NULL);

	int len = (strlen(str1) + strlen(str2) + 1);
	dstring temp = ds_new_string(len);
	copy(temp, str1);
	int len2 = strlen(str1);
	dstring tp = temp+len2;
	copy(tp, str2);
	temp[len] = '\0';
	assert(temp != NULL);
	return temp;
}

int ds_truncate(dstring* dest, unsigned int truncated_len)
{
	assert(*dest != NULL);
	assert(dest != NULL);
	assert(truncated_len > 0);
	
	dstring temp = realloc(*dest, sizeof(char)*truncated_len+1);
	if (temp == NULL)
	{
		return -1;
	}
	*dest = temp;
	(*dest)[truncated_len] = '\0';
	return 1;
}

void ds_print(dstring s, FILE* file)
{
	assert(file != NULL);
	int length = strlen(s);
	for (int i = 0; i < length; i++)
	{
		fprintf(file, "%c", s[i]);
	}
}

void ds_delete(dstring* del)
{
	assert(*del != NULL);
	free(*del);
	*del = NULL;
}

static void copy(dstring d, dstring s)
{
	strcpy(d, s);
}

int ds_copy(dstring* dest, dstring src)
{
	if (src == NULL || strlen(src) == 0)
	{
		// error
		return -1;
	}
	dstring tmp = (dstring)malloc(strlen(src) + 1);
	if (!tmp)
		return 0;
	*dest = tmp;
	copy(*dest, src);
	return 1;
}

dstring ds_substring(dstring string, uint start, uint end)
{
	assert(start <= end);
	assert(strlen(string) > (size_t)end);


	int len = end - start+2; // plus 2 because they are index and 1 space for the null terminator
	dstring s = ds_new_string(len);
	uint i, j;
	for (i = start, j = 0; i <= end; i++, j++)
	{
		s[j] = string[i];
	}
	s[j] = '\0';
	return s;
}

// return index of first occurance of the character
int ds_find_character(dstring message, char character)
{
    return ds_find_character_start_at(message, 0, character);
}

int ds_find_character_start_at(dstring message, uint start, char character)
{
	assert(message != NULL);
	assert((size_t)start < strlen(message));
    uint i;
    uint len = (uint)strlen(message);
    for (i = start; i < len; i++)
    {
        if (message[i] == character)
        {
            break;
        }
    }

	if (i == len)
	{
		return -1;
	}

    return i;
}