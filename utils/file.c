#include "file.h"

// return read bytes
size_t read_file(const char *filepath, void **out)
{
	// make filepointer and open file
	FILE *rfp = fopen(filepath, "rb"); // rb == read binary
	if (!rfp)
	{
		return 0;
	}

	// get file size (bytes)
	fseek(rfp, 0, SEEK_END);
	long length = ftell(rfp);
	fseek(rfp, 0, SEEK_SET);

	// create buffer
	void *file = (void *)malloc(length);
	if (!file)
	{
		return 0;
	}

	// read file contents
	size_t readBytes = fread(file, BYTE, length, rfp);
	// close file
	fclose(rfp);

	*out = file;
	return readBytes;
}

// return written bytes
size_t write_file(const char *filepath, void *contents, size_t size)
{
	// make filepointer and open file
	FILE *wfp = fopen(filepath, "wb"); // wb == write binary
	if (!wfp)
	{
		return 0;
	}

	if (contents == NULL)
	{
		return 0;
	}

	size_t writtenBytes = fwrite(contents, BYTE, size, wfp);
	// close file
	fclose(wfp);
	return writtenBytes;
}

// read file and return contents as a string (by ref)
size_t read_text_file(char *filepath, char **out)
{
	void *buffer;
	size_t readBytes = read_file(filepath, &buffer);
	*out = (char *)buffer;
	return readBytes;
}

size_t write_text_file(char *filepath, char **string, size_t size)
{
	return write_file(filepath, (void *)string, size);
}

// future functions
// size_t append_file(); // append to the end of the file
// size_t readBytesFile(int amountBytes, int offset); // read a specified amount of bytes from file (from offset to amountBytes)