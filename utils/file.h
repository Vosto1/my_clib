/*
skriva läsa till fil
*/
#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

/**
 * read contents of a file
 * 
 * @param filepath path to the file
 * @param contents a pointer to the buffer where the contents of the file will be written to
 * @return bytes read
 */
size_t readFile(const char* filepath, void** contents);

/**
 * write data to file
 * 
 * @param filepath path to the file to write to
 * @param contents a pointer to the data that should be written to the file
 * @param size the size of the contents in bytes
 * @return bytes written
 */
size_t writeFile(const char* filepath, void* contents, size_t size);

#endif