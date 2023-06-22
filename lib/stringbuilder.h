#ifndef STRINGBUILDER_H
#define STRINGBUILDER_H

#include <stdbool.h>
#include "dstring.h"

typedef struct {
    char* string;
    uint used;
    uint size;
}stringBuilder;

uint sb_length(stringBuilder* sb);
stringBuilder sb_new_string_builder();
bool sb_add(stringBuilder* sb, char c);
bool sb_append(stringBuilder* sb, dstring string, uint length);
void sb_remove(stringBuilder*sb, uint amount);
dstring sb_to_string(stringBuilder* sb);
void sb_delete(stringBuilder* sb);

#endif