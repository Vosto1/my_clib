#include "dstring.h"
#include "mdarray.h"

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

dstring ds_new_string_mem(void* mem, uint length)
{
    // add memory space for '\0',
    // and to make sure that [mem size == length] is not a lie
    char* tmp = (char*)realloc(mem, length + 1);

    tmp[length] = '\0';

    dstring str;
    str.string = tmp;
    str.length = strlen(str.string); // instead of len, because there might be a '\0' somewhere before str.string[length]
    str.size = length + 1;

    return str;
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

dstring ds_cstrconcat(const char* str1, const char* str2)
{
	assert(str1 != NULL);
	assert(str2!= NULL);

	uint length = strlen(str1) + strlen(str2);
	dstring temp = ds_new_string(length);
	chpcopy(temp.string, (char*)str1);
	uint len2 = strlen(str1);
	char* tp = temp.string+len2;
	chpcopy(tp, (char*)str2);
	temp.string[length] = '\0';
    temp.length = length;
	assert(temp.string != NULL);
	assert(temp.length == length);
	return temp;
}

dstring ds_bcstrconcat(dstring str, const char* cstr)
{
    dstring tmp = ds_new_string_initialize(cstr);
    dstring res = ds_concat(str, tmp);
    ds_delete(&tmp);
    return res;
}

dstring ds_bcharconcat(dstring str, char ch)
{
    char char2str[2] = { ch, '\0'};
    return ds_bcstrconcat(str, char2str);
}

dstring ds_fcstrconcat(const char* cstr, dstring str)
{
    dstring tmp = ds_new_string_initialize(cstr);
    dstring res = ds_concat(tmp, str);
    ds_delete(&tmp);
    return res;
}

dstring ds_fcharconcat(char ch, dstring str)
{
    char char2str[2] = { ch, '\0'};
    return ds_fcstrconcat(char2str, str);
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

int ds_remove_last_n(dstring* str, unsigned int to_remove)
{
    int len = str->length - to_remove;
    assert(ds_truncate(str, len) == 1);
    return len;
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

void ds_delete_dynamically_allocated_dstring(void* str)
{
    ds_delete(str);
    free(str);
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


	int len = end - start+1; // plus 1 because they are index
	dstring s = ds_new_string(len);
	uint i, j;
	for (i = start, j = 0; i <= end; i++, j++)
	{
		s.string[j] = string.string[i];
	}
	s.string[j] = '\0';
    s.length = j; // the index the null term is at == length of string
	return s;
}

int ds_find_last_char_occurance(dstring string, char character)
{
    assert(string.string != NULL);
    int i;
    for (i = string.length - 1; i >= 0; i--)
    {
        if (string.string[i] == character)
        {
            break;
        }
    }

    if (i == 0)
    {
        return -1;
    }

    return i;
}

// return index of first occurance of the character
int ds_find_character(dstring string, char character)
{
    return ds_find_character_start_at(string, 0, character);
}

int ds_find_character_start_at(dstring string, uint start, char character)
{
	assert(string.string != NULL);
	assert(start < string.length);
    uint i;
    uint len = string.length;
    for (i = start; i < len; i++)
    {
        if (string.string[i] == character)
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

mdarray ds_split(dstring str, char delim)
{
    dstring splitted;
    dstring* dynamic = NULL;
    int prev_index = 0;

    mdarray results = mda_create_empty();
    mda_init(&results, str.length / 4, &ds_delete_dynamically_allocated_dstring);

    int index = ds_find_character(str, delim);

    if (index == -1)
    {
        return results;
    }

    // edge case: first character is equal to delim
    if (index == 0)
    {
        // skip until find non-delim char
        index++;
        if (str.string[index] == delim)
        {
            while (str.string[index] == delim)
            {
                index++;
            }
        }

        // edge case: there was only delim in the string
        if (index >= (int)str.length)
        {
            return results;
        }

        // handle edge case
        prev_index = index;
        index = ds_find_character_start_at(str, index, delim);

        if (index == -1)
        {
            splitted = ds_substring(str, prev_index, str.length-1);

            dynamic = (dstring*)malloc(sizeof(dstring));
            dynamic->length = splitted.length;
            dynamic->size = splitted.size;
            dynamic->string = splitted.string;

            assert((uint)mda_insert(&results, dynamic) == results.used);

            return results;
        }
    }

    do
    {
        splitted = ds_substring(str, prev_index, index-1);

        dynamic = (dstring*)malloc(sizeof(dstring));
        dynamic->length = splitted.length;
        dynamic->size = splitted.size;
        dynamic->string = splitted.string;

        assert((uint)mda_insert(&results, dynamic) == results.used);
        uint next_index = index+1;
        // edge case: delim is in the end of the string
        if (next_index >= str.length)
        {
            return results;
        }

        // edge case: delim is beside each other
        while (str.string[next_index] == delim)
        {
            next_index++;
        }

        // edge case: rest of string was equal to delim
        if (next_index >= str.length)
        {
            return results;
        }

        prev_index = next_index;
        index = ds_find_character_start_at(str, next_index, delim);
    } while (index != -1);
    
    // todo: handle edge case: last char is equal to delim

    splitted = ds_substring(str, prev_index, str.length-1);

    dynamic = (dstring*)malloc(sizeof(dstring));
    dynamic->length = splitted.length;
    dynamic->size = splitted.size;
    dynamic->string = splitted.string;

    assert(mda_insert(&results, dynamic) == (int)results.used);

    return results;
}

void ds_replace_char(dstring str, char old_char, char new_char)
{
    for (uint i = 0; i < str.length; i++)
    {
        if (str.string[i] == old_char)
        {
            str.string[i] = new_char;
        }
    }
}

int ds_count_char(dstring str, char target_char)
{
    int count = 0;
    for (uint i = 0; i < str.length; i++)
    {
        if (str.string[i] == target_char)
        {
            count++;
        }
    }
    return count;
}

////////////////////////////////////////////////////////////
// constant string functions
////////////////////////////////////////////////////////////

dstring s_remove_last_n(char* str, unsigned int to_remove)
{
    int len = strlen(str) - to_remove;
    dstring tmp = ds_new_string_initialize(str);
    assert(ds_truncate(&tmp, len) == 1);
    return tmp;
}

dstring s_substring(char* string, uint start, uint end)
{
    uint len = strlen(string);
	assert(start <= end);
	assert(len > end);


	uint new_len = end - start+1; // plus 1 because they are index
	dstring s = ds_new_string(new_len);
	uint i, j;
	for (i = start, j = 0; i <= end; i++, j++)
	{
		s.string[j] = string[i];
	}
	s.string[j] = '\0';
    s.length = j; // the index the null term is at == length of string
	return s;
}

int s_find_last_char_occurance(char* string, char character)
{
    assert(string != NULL);
    int len = strlen(string);
    int i;
    for (i = len - 1; i >= 0; i--)
    {
        if (string[i] == character)
        {
            break;
        }
    }

    if (i == 0)
    {
        return -1;
    }

    return i;
}

// return index of first occurance of the character
int s_find_character(char* string, char character)
{
    return s_find_character_start_at(string, 0, character);
}

int s_find_character_start_at(char* string, uint start, char character)
{
	assert(string != NULL);
    uint len = strlen(string);
	assert(start < len);
    uint i;
    for (i = start; i < len; i++)
    {
        if (string[i] == character)
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

char s_at(char* string, uint index)
{
    assert(index < strlen(string));
    return string[index];
}