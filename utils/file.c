#include "file.h"

// return size of file (bytes)
size_t readFile(const char* filepath, void** out) {
	// make filepointer and open file
	FILE* rfp = fopen(filepath, "r");
	if (!rfp) {
		errcset(EFILE_OPENWRITE);
		return 0;
	}

	// get file size (bytes)
	fseek(rfp, 0, SEEK_END);
	size_t length = ftell(rfp);
	fseek(rfp, 0, SEEK_SET);

	// create 
	void* file = (void*)malloc(length);
	if (!file) {
		errcset(EMEM_ALLOC);
		return 0;
	}

	// read file contents
	size_t readBytes = fread(file, 1, length, rfp);
	// close file
	fclose(rfp);

	*out = file;
	return readBytes;
}

// return written bytes
size_t writeFile(const char* filepath, void* contents, size_t size) {
	// make filepointer and open file
	FILE* wfp = fopen(filepath, "w");
	if (!wfp) {
		errcset(EFILE_OPENREAD);
		return 0;
	}

    if (contents == NULL) {
        errcset(ENULL_ARG);
		return 0;
    }

	size_t writtenBytes =  fwrite(contents, sizeof(byte), size, wfp);
	// close file
	fclose(wfp);
	return writtenBytes;
}

//size_t appendFile();