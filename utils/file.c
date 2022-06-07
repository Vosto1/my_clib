#include "file.h"

// return read bytes
size_t readFile(const char* filepath, void** out) {
	// make filepointer and open file
	FILE* rfp = fopen(filepath, "rb"); // rb == read binary
	if (!rfp) {
		errcset(EFILE_OPENWRITE);
		return 0;
	}

	// get file size (bytes)
	fseek(rfp, 0, SEEK_END);
	size_t length = ftell(rfp);
	fseek(rfp, 0, SEEK_SET);

	// create buffer
	size_t sizeBytes = length * BYTE;
	void* file = (void*)malloc(sizeBytes);
	if (!file) {
		errcset(EMEM_ALLOC);
		return 0;
	}

	// read file contents
	size_t readBytes = fread(file, ELEMENT_SIZE, sizeBytes, rfp);
	// close file
	fclose(rfp);

	*out = file;
	return readBytes;
}

// return written bytes
size_t writeFile(const char* filepath, void* contents, size_t size) {
	// make filepointer and open file
	FILE* wfp = fopen(filepath, "wb"); // wb == write binary
	if (!wfp) {
		errcset(EFILE_OPENREAD);
		return 0;
	}

    if (contents == NULL) {
        errcset(ENULL_ARG);
		return 0;
    }

	size_t sizeBytes = size * BYTE;
	size_t writtenBytes =  fwrite(contents, ELEMENT_SIZE, sizeBytes, wfp);
	// close file
	fclose(wfp);
	return writtenBytes;
}

// read file and return contents as a string (by ref)
size_t readTextFile(char* filepath, char** out) {
    void* buffer;
    size_t readBytes = readFile(filepath, &buffer);
    *out = (char*)buffer;
    return readBytes;
}

// future functions
//size_t appendFile(); // append to the end of the file
//size_t readBytesFile(int amountBytes, int offset); // read a specified amount of bytes from file (from offset to amountBytes)