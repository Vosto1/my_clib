#include "dstring.h"

static void copy(dstring d, dstring s);
static void chpcopy(char* d, char* s);

dstring ds_new_string(int length)
{
    dstring temp;
    temp.length = 0;
    temp.size = length + 1;
	temp.string = (char*)malloc(sizeof(char) * temp.size);
    temp.string[length] = '\0';
	assert(temp.string != NULL);
	return temp;
}

dstring ds_random(int length)
{
	static int min = 32;
	static int max = 125;
	dstring s = ds_new_string(length);
	for (int i = 0; i <= length; i++)
	{
		s.string[i] = rand() % (max - min) + min;
	}
	s.string[length] = '\0';
    s.length = length;
	return s;
}

dstring ds_new_string_initialize(const char* s)
{
	assert(s != NULL);
	dstring temp;
    int length = strlen(s);
	temp = ds_new_string(length);
	strcpy(temp.string, s);
    temp.length = length;
	assert(strlen(temp.string) == strlen(s));
	assert(!(strcmp(temp.string, s)));
	return temp;
}

dstring ds_concat(dstring str1, dstring str2)
{
	assert(str1.string != NULL);
	assert(str2.string != NULL);

	uint length = str1.length + str2.length;
	dstring temp = ds_new_string(length);
	copy(temp, str1);
	uint len2 = str1.length;
	char* tp = temp.string+len2;
	chpcopy(tp, str2.string);
	temp.string[length] = '\0';
    temp.length = length;
	assert(temp.string != NULL);
	assert(temp.length == length);
	return temp;
}

int ds_truncate(dstring* dest, unsigned int truncated_len)
{
	assert(dest != NULL);
	assert((*dest).string != NULL);
	assert(truncated_len > 0);
	
    dstring temp;
	temp.string = realloc((*dest).string, sizeof(char)*truncated_len+1);
	if (temp.string == NULL)
	{
		return -1;
	}
	*dest = temp;
	(*dest).string[truncated_len] = '\0';
    (*dest).length = truncated_len;
    (*dest).size = truncated_len + 1;
	return 1;
}

void ds_print(dstring s, FILE* file)
{
	assert(file != NULL);
	for (uint i = 0; i < s.length; i++)
	{
		fprintf(file, "%c", s.string[i]);
	}
}

void ds_delete(dstring* del)
{
	assert((*del).string != NULL);
	free((*del).string);
	(*del).string = NULL;
    (*del).size = 0;
    (*del).length = 0;
}

static void chpcopy(char* d, char* s)
{
	strcpy(d, s);
}

static void copy(dstring d, dstring s)
{
	strcpy(d.string, s.string);
    d.length = s.length;
}

int ds_copy(dstring* dest, dstring src)
{
	if (src.string == NULL || src.length == 0)
	{
		// error
		return -1;
	}
	dstring tmp;
    tmp.string = malloc(src.length + 1);
	if (!tmp.string)
		return 0;
    tmp.length = src.length;
    tmp.size = src.length + 1;
	*dest = tmp;
	copy(*dest, src);
	return 1;
}

dstring ds_substring(dstring string, uint start, uint end)
{
	assert(start <= end);
	assert(string.length > end);


	int len = end - start+2; // plus 2 because they are index and 1 space for the null terminator
	dstring s = ds_new_string(len);
	uint i, j;
	for (i = start, j = 0; i <= end; i++, j++)
	{
		s.string[j] = string.string[i];
	}
	s.string[j] = '\0';
	return s;
}

// return index of first occurance of the character
int ds_find_character(dstring message, char character)
{
    return ds_find_character_start_at(message, 0, character);
}

int ds_find_character_start_at(dstring message, uint start, char character)
{
	assert(message.string != NULL);
	assert(start < message.length);
    uint i;
    uint len = message.length;
    for (i = start; i < len; i++)
    {
        if (message.string[i] == character)
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

char ds_at(dstring string, uint index)
{
    assert(index < string.length);
    return string.string[index];
}