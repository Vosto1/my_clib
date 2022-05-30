#include "text_analysis.h"

size_t readTextFile(char* filename, char** out) {
    void* buffer;
    size_t readBytes = readFile(filename, buffer);
    *out = (char*)buffer;
    return readBytes;
}