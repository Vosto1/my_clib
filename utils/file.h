/*
write/read files
*/
#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

#define BYTE sizeof(unsigned char)
#define ELEMENT_SIZE 1
typedef unsigned long long dim_t;

/**
 * read contents of a file (binary)
 *
 * @param filepath path to the file
 * @param contents a pointer to the buffer where the contents of the file will be written to
 * @return bytes read
 */
dim_t read_file(const char *filepath, void **contents);

/**
 * write data to file (binary)
 *
 * @param filepath path to the file to write to
 * @param contents a pointer to the data that should be written to the file
 * @param size the size of the contents in bytes
 * @return bytes written
 */
dim_t write_file(const char *filepath, void *contents, dim_t size);

/**
 * @brief read a textfile
 *
 * @param filepath path to file
 * @param out a pointer to the buffer where the contents of the text file will be written to
 * @return bytes read
 */
dim_t read_text_file(char *filepath, char **out);

/**
 * @brief write text to text file
 *
 * @param filepath path to file
 * @param string string to write
 * @param size size of string
 * @return bytes written
 */
dim_t write_text_file(char *filepath, char **string, dim_t size);

#endif
