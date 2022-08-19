#include "file.h"

// return read bytes
dim_t read_file(const char *filepath, void **out)
{
	// make filepointer and open file
	FILE *rfp = fopen(filepath, "rb"); // rb == read binary
	if (!rfp)
	{
		errcset(EFILE_OPENWRITE);
		return 0;
	}

	// get file size (bytes)
	fseek(rfp, 0, SEEK_END);
	dim_t length = ftell(rfp);
	fseek(rfp, 0, SEEK_SET);

	// create buffer
	dim_t sizeBytes = length * BYTE;
	void *file = (void *)malloc(sizeBytes);
	if (!file)
	{
		errcset(EMEM_ALLOC);
		return 0;
	}

	// read file contents
	dim_t readBytes = fread(file, ELEMENT_SIZE, sizeBytes, rfp);
	// close file
	fclose(rfp);

	*out = file;
	return readBytes;
}

// return written bytes
dim_t write_file(const char *filepath, void *contents, dim_t size)
{
	// make filepointer and open file
	FILE *wfp = fopen(filepath, "wb"); // wb == write binary
	if (!wfp)
	{
		errcset(EFILE_OPENREAD);
		return 0;
	}

	if (contents == NULL)
	{
		errcset(ENULL_ARG);
		return 0;
	}

	dim_t sizeBytes = size * BYTE;
	dim_t writtenBytes = fwrite(contents, ELEMENT_SIZE, sizeBytes, wfp);
	// close file
	fclose(wfp);
	return writtenBytes;
}

// read file and return contents as a string (by ref)
dim_t read_text_file(char *filepath, char **out)
{
	void *buffer;
	dim_t readBytes = read_file(filepath, &buffer);
	*out = (char *)buffer;
	return readBytes;
}

dim_t write_text_file(char *filepath, char **string, dim_t size)
{
	return write_file(filepath, (void *)string, size);
}

// future functions
// size_t append_file(); // append to the end of the file
// size_t readBytesFile(int amountBytes, int offset); // read a specified amount of bytes from file (from offset to amountBytes)