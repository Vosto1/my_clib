#ifndef TEXT_ANALYSIS_H
#define TEXT_ANALYSIS_H

#include "../hash_dictionary/hashtable.h"
#include "../utils/file.h"


hashtable letter_occurances(char *string, int strsize);

#endif