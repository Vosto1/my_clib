#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _shader_file {
	int lines;
	char** contents;
}shader_file;

// bättre att använda fseek
int countLines(FILE* fp) {
	int lines = 0;
	char ch;
	while (EOF != (ch = getc(fp)))
		if (ch == '\n')
			lines++;
	return lines;
}

void error() {
	int enumber = errno;
	char estr[3];
	sprintf(estr, "E%d", enumber);
	perror(estr);
}

// print array of strings and special characters
void debugPrint(char** str, int length) {
	int tmp = 0;
	for (int i = 0; i <= length; i++) {
		for (int j = 0; j < strlen(str[i]); j++, tmp = j) {
			if (str[i][j] == '\n')
				printf("\\n");
			else
				printf("%c", str[i][j]);
		}
		if (str[i][tmp] == '\0') // print null char
			printf("\\0\n");
	}
}

// print char array and special characters
void debugPrint2(const char* str, int length) {
	int tmp = 0;
	for (int i = 0; i < length; i++) {
		if (str[i] == '\n') // print newline char
			printf("\\n\n");
		else if (str[i] == '\0') // print null char
			printf("\\0\n");
		else
			printf("%c", str[i]);
	}
}

// load file lines into array of strings
int loadFile(const char* filepath, char*** contents) {
	// make filepointer and open file
	FILE* vfp = fopen(filepath, "r");
	if (!vfp) {
		error();
	}

	// count lines and allocate memory to hold file contents
	int amountOfLines = countLines(vfp);
	fclose(vfp);
	char** stringArray = (char**)malloc(sizeof(char) * 1024 * amountOfLines);
	if (!stringArray) {
		error();
	}
	for (int i = 0; i <= amountOfLines; i++) {
		stringArray[i] = (char*)malloc(1024 * sizeof(char));
		if (!stringArray[i]) {
			error();
		}
	}

	// read file contents and save them to the allocated memory
	char temp[1024];
	vfp = fopen(filepath, "r");
	for (int i = 0; i <= amountOfLines; i++) {
		fgets(temp, 1024, vfp);
		strcpy(stringArray[i], temp);
		printf("lines read: %d\n", i);
	}

	// dbug print
	

	// how to free the memory
	//for (int i = 0; i < amountOfLines; i++) {
	//	free(stringArray[i]);
	//}
	//free(stringArray);

	*contents = stringArray;
	return amountOfLines;
}

// load the whole file as one string (currently broken, reads garbage somehow)
int loadFile2(const char* filepath, char** contents) {
	// make filepointer and open file
	FILE* vfp = fopen(filepath, "r");
	if (!vfp) {
		error();
	}


	// count bytes
	fseek(vfp, 0, SEEK_END);
	int length = ftell(vfp);
	fseek(vfp, 0, SEEK_SET);
	// create 
	char* stringArray = (char*)malloc(sizeof(char) * length);
	if (!stringArray) {
		error();
	}

	// read file contents
	fread(stringArray, 1, length, vfp);

	* contents = stringArray;
	return length;
}

// a bunch of bit manipulation
/*
    for (int i = 0; i < sizeBits(sizeBytes); i++) {
        *temp = *temp & (0 << i); // initialize to zero
    }
    dbug(temp, sizeBytes);

    unsigned int bit = 0;
    int i = 0;
    dbug(res, sizeof(unsigned int));
    for (int i = 0; i < sizeof(unsigned int); i++) { // for each byte
        byte restmp = res[i]; // extract byte
        for (int j = 0; j < sizeBits(sizeof(byte)); j++) { // for each bit in byte
            bit = restmp & (1 << j); // extract bit at position j from the residualBits value
            *temp = *temp | (bit << (j + i)); // set bit at position j + i to the extracted bit
        }
    }

    dbug(temp, sizeBytes);

    unsigned int sizeBitsToWriteTo = sizeBits(sizeBytes - sizeof(unsigned int));
    bit = 0;
    for (int j = i; j < sizeBitsToWriteTo; j++) {
        bit = *b->bytes & (1 << j); // extract bit at position j
        *temp | (bit << j); // set extracted bit at position j
    }
    */

   /* void* buffer;
    size_t size = readFile(file, &buffer);
    byte* bytes = (byte*)buffer;
    unsigned int residualBits = 0;
    unsigned int bit = 0;
    // read residual bits which is in the top of the file
    for (int i = 0; i < sizeBits(sizeof(unsigned int)); i++) {
        bit = *bytes & (1 << i); // extract bit at i
        residualBits = residualBits | (bit << i); // set bit at i
    }
    // read binary data
    for (int j = sizeof(unsigned int); j < size; j++) { // for each byte
        byte tmp = bytes[j]; // extract byte
        for (int k = 0; k < sizeBits(sizeof(byte)); k++) {
            bit = tmp & (1 << k); // extract bit at k
            b->bytes[j] = b->bytes[j] | (bit << k); // set extracted bit at k in byte j
        }
    }
    b->residualBits = residualBits;
    b->amountOfBytes = size - sizeof(unsigned int);
    return SUCCESS; */