#include "file.h"

// return read bytes
int read_file(const char *filepath, void **out)
{
	// make filepointer and open file
	FILE *rfp = fopen(filepath, "rb"); // rb == read binary
	if (!rfp)
	{
		return -1;
	}

	// get file size (bytes)
	fseek(rfp, 0, SEEK_END);
	long length = ftell(rfp);
	fseek(rfp, 0, SEEK_SET);

	// create buffer
	void *file = (void *)malloc(length);
	if (!file)
	{
		return -2;
	}

	// read file contents
	int readBytes = fread(file, BYTE, length, rfp);
	// close file
	fclose(rfp);

	*out = file;
	return readBytes;
}

// return written bytes
int write_file(const char *filepath, void *contents, uint size)
{
	// make filepointer and open file
	FILE *wfp = fopen(filepath, "wb"); // wb == write binary
	if (!wfp)
	{
		return -1;
	}

	if (contents == NULL)
	{
		return -2;
	}

	int writtenBytes = fwrite(contents, BYTE, size, wfp);
	// close file
	fclose(wfp);
	return writtenBytes;
}

// read file and return contents as a string (by ref)
int read_text_file(char *filepath, char **out)
{
	void *buffer;
	int readBytes = read_file(filepath, &buffer);
    if (readBytes < 0)
    {
        return readBytes;
    }
	*out = (char *)buffer;
    char* tmp = (char*)realloc(*out, readBytes + 1); // add space for '\0'
    if (tmp == NULL)
    {
        return -3;
    }
    tmp[readBytes] = '\0';
    *out = tmp;
	return readBytes;
}

int write_text_file(char *filepath, char **string, uint size)
{
	return write_file(filepath, (void *)string, size);
}

int get_file_size(char *filepath)
{
	FILE *fp = fopen(filepath, "rb"); // rb == read binary
    // get file size (bytes)
	if (fp == NULL)
	{
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	fclose(fp);
	return length;
}

// future functions
// int append_file(); // append to the end of the file
// int readBytesFile(int amountBytes, int offset); // read a specified amount of bytes from file (from offset to amountBytes)