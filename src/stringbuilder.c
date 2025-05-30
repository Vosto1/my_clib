#include "stringbuilder.h"

#define QUARTER 0.25

typedef enum
{                               // decrease/increase memory results
    MEM_INCREASED = 0,            // doubled memory
    MEM_HALVED = 1,             // memory halved
    MEM_OK = 2,                 // memory min size reached or no need
    ERR_MEM = 3,        // ERRor MEMory DECREASE
} MEM;

static MEM sb_memory_decrease(stringBuilder *sb);
static MEM sb_memory_increase(stringBuilder *sb);
static MEM sb_memory_increase_manual(stringBuilder* sb, uint new_size);

uint sb_length(stringBuilder* sb)
{
    return sb->used;
}

stringBuilder sb_new_string_builder()
{
    stringBuilder sb;
    sb.size = 1;
    sb.used = 0;
    char* temp = (char*)malloc(sizeof(char));

    temp[0] = '\0';

    if (temp != NULL)
        sb.string = temp;
    else
        sb.string = NULL;

    return sb;
}

bool sb_add(stringBuilder* sb, char c)
{
    if (sb == NULL)
    {
        return false;
    }

    MEM m = sb_memory_increase(sb);
    assert(m == MEM_OK || m == MEM_INCREASED);

    sb->string[sb->used] = c;
    sb->used += 1;
    sb->string[sb->used] = '\0';
    return true;
}

bool sb_append(stringBuilder* sb, dstring string)
{
    if (sb == NULL || sb->string == NULL || string.string == NULL)
    {
        return false;
    }

    MEM m = sb_memory_increase(sb);
    assert(m == MEM_OK || m == MEM_INCREASED);

    if (sb->used + string.length >= sb->size)
    {
        MEM m = sb_memory_increase_manual(sb, sb->used + string.length);
        assert(m == MEM_INCREASED);
    }

    for (uint i = sb->used, j = 0; i < sb->used + string.length; i++, j++)
    {
        sb->string[i] = string.string[j];
    }

    sb->used += string.length;
    sb->string[sb->used] = '\0';
    return true;
}

bool sb_appends(stringBuilder* sb, char* string)
{
    if (sb == NULL || sb->string == NULL || string == NULL)
    {
        return false;
    }

    MEM m = sb_memory_increase(sb);
    assert(m == MEM_OK || m == MEM_INCREASED);

    int length = strlen(string);
    if (sb->used + length >= sb->size)
    {
        MEM m = sb_memory_increase_manual(sb, sb->used + length);
        assert(m == MEM_INCREASED);
    }

    for (uint i = sb->used, j = 0; i < sb->used + length; i++, j++)
    {
        sb->string[i] = string[j];
    }

    sb->used += length;
    sb->string[sb->used] = '\0';
    return true;

}

void sb_remove(stringBuilder*sb, uint amount)
{
    sb->used = sb->used - amount;
    sb->string[sb->used] = '\0';
    MEM m = sb_memory_decrease(sb);
    assert(m == MEM_HALVED || m == MEM_OK);
}

dstring sb_to_string(stringBuilder* sb)
{
    dstring string = ds_new_string_initialize(sb->string);
    return string;
}

void sb_delete(stringBuilder* sb)
{
    free(sb->string);
    sb->string = NULL;
    sb->size = 0;
    sb->used = 0;
}

static MEM sb_memory_increase(stringBuilder* sb)
{
    // memory increase
    if (sb->used == sb->size - 1) // -1 because \0
    {   
        // when array is full on insert, double the size
        return sb_memory_increase_manual(sb, sb->size * 2);
    }
    return MEM_OK;
}

static MEM sb_memory_increase_manual(stringBuilder* sb, uint new_size)
{
    // memory increase
    char* temp = (char*)realloc(sb->string, sizeof(char) * new_size);
    if (temp != NULL)
    {
        sb->size *= 2;
        sb->string = temp;
        return MEM_INCREASED;
    }
    else
    {
        return ERR_MEM;
    }
}

// memory check + increase is done in arrayInsert function
static MEM sb_memory_decrease(stringBuilder *sb)
{
    double ratio = (double)sb->used / (double)sb->size;
    // if 1/4 of the allocated space is used, halve it
    if (ratio <= QUARTER && sb->size != 1)
    {
        sb->size /= 2;
        char*temp = (char*)realloc(sb->string, sizeof(char) * sb->size);
        if (temp != NULL)
        {
            sb->string = temp;
            return MEM_HALVED;
        }
        else
        {
            return ERR_MEM;
        }
    }
    return MEM_OK;
}