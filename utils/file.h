/*
skriva läsa till fil
*/
#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

#define BYTE sizeof(unsigned char)
#define ELEMENT_SIZE 1

/**
 * read contents of a file (binary)
 * 
 * @param filepath path to the file
 * @param contents a pointer to the buffer where the contents of the file will be written to
 * @return bytes read
 */
size_t readFile(const char* filepath, void** contents);

/**
 * write data to file (binary)
 * 
 * @param filepath path to the file to write to
 * @param contents a pointer to the data that should be written to the file
 * @param size the size of the contents in bytes
 * @return bytes written
 */
size_t writeFile(const char* filepath, void* contents, size_t size);

/**
 * @brief read a textfile
 * 
 * @param filepath path to file
 * @param out a pointer to the buffer where the contents of the text file will be written to
 * @return bytes read
 */
size_t readTextFile(char* filepath, char** out);

#endif