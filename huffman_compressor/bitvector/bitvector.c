#include "bitvector.h"

size_t bitCount(bitVector* bv) {
    return bv->used;
}

bitVector createEmptyBitVector() {
    bitVector bv = createEmptyDynamicArray();
    return bv;
}

ErrorCode initBitVector(bitVector* bv) {
    return initDynamicArray(bv, 1);
}

ErrorCode addbv(bitVector* bv, bool value) {
    return arrayInsert(bv, value);
}

ErrorCode unionbv(bitVector* bv, bitVector* unionWith) {
    return arrayUnion(bv, unionWith);
}

ErrorCode removeLastbv(bitVector* bv) {
    return arrayRemoveLast(bv);
}

ErrorCode clearbv(bitVector* bv) {
    return arrayClear(bv);
}

unsigned int sizeBits(unsigned int size) {
    static const unsigned int BYTE = 8;
    return size * BYTE;
}

// converts a bitvector into an array of bytes
ErrorCode bools2bits(bitVector* bv, binary* out) {
    if (bv == NULL || out == NULL) return NULL_ARG;
    unsigned int residualBitsInLastByte = sizeBits(sizeof(byte)) - (bitCount(bv) % sizeBits(sizeof(byte)));
    // size of the bin file
    unsigned int bits = bitCount(bv)  + residualBitsInLastByte; // file size + the number of residual bits
    unsigned int amountOfBytes = bits / sizeBits(sizeof(byte));
    // init array of bytes
    byte* binary = (byte*)malloc(amountOfBytes * sizeof(byte));
    if (binary == NULL) return MEM_ALLOC;

    int currentIndex = 0;
    for (unsigned int i = 0; i < amountOfBytes; i++) { // for every byte
        byte b;
        for (unsigned int k = 0; k < sizeBits(sizeof(byte)); k++) { // initialize byte
            b = b & (0 << k); // set all bits to 0
        }
        for(unsigned int j = 0; j < sizeBits(sizeof(byte)) && currentIndex < bitCount(bv) ; j++) { // set bits according to bitvector
            if (bv->array[j]) {
                b = b | (1 << j); // set bit on position j to 1
            }
            currentIndex++;
        }
        binary[i] = b;
    }
    out->residualBits = residualBitsInLastByte;
    out->amountOfBytes = amountOfBytes;
    out->bytes = binary;

    return SUCCESS;
}

// converts an array of bytes to a bitvector
ErrorCode bits2bools(binary* b, bitVector* out) {
    if(b->bytes == NULL) return ARR_EMPTY;
    // initialize -->
    ErrorCode e = SUCCESS;
    bitVector bv = createEmptyBitVector();
    e = initBitVector(&bv);
    if(e != SUCCESS) return e;
    // <--

    for (int i = 0; i < b->amountOfBytes; i++) { // for each byte
        byte tmp = b->bytes[i]; // extract byte at position i
        for (int j = 0; j < sizeBits(sizeof(byte)); j++) { // for each bit
            if (tmp & (1 << j)) // if there is a 1 at position j 
                e = addbv(&bv, true); // add true
            else
                e = addbv(&bv, false); // otherwise there was a 0, add false
        }
    }
    if (e != SUCCESS) return e;

    for (int k = 0; k < b->residualBits; k++)
        e = removeLastbv(&bv);

    *out = bv;
    return e;
}

ErrorCode writeBinaryFromFile(binary* b, char* file) {
    byte* res = (byte*)&b->residualBits;
    // create memory buffer with the right amount of bytes
    size_t byteSizeResidual = sizeof(unsigned int);
    size_t byteSizeData = sizeof(byte) * b->amountOfBytes;
    byte* buffer = (byte*)malloc((byteSizeResidual + byteSizeData) * sizeof(byte));
    if (!buffer) return MEM_ALLOC;
    byte tmp;
    int i = 0;
    int j = 0;
    while (i < byteSizeResidual) { // write residual bits to buffer
        tmp = res[i];
        buffer[i] = tmp;
        i++;
    }
    while (j < byteSizeData) { // write data to buffer
        tmp = b->bytes[j];
        buffer[i] = tmp;
        i++;
        j++;
    }
    size_t fileSize = byteSizeResidual + byteSizeData;
    
    dbug(buffer, fileSize);
    if (writeFile(file, (void*)buffer, fileSize) == fileSize) return SUCCESS;
    else return WRITE_BINARY;
}

ErrorCode readBinaryFromFile(char* file, binary* b) {
    void* fileContents;
    size_t fileSize = readFile(file, &fileContents);
    byte* byteBuffer = (byte*)fileContents; // set byte pointer to buffer to enable pointer arithmetic

    size_t byteSizeResidual = sizeof(unsigned int); // residual bits size in bytes
    size_t byteSizeData = fileSize - byteSizeResidual; // get data size in bytes

    unsigned int residualBits = 0;
    byte* res = (byte*)&residualBits;
    for (int i = 0; i < byteSizeResidual; i++) // for each byte in residual bits value
        res[i] = byteBuffer[i]; // set byte i in residualBits to bit i in bytes read from file

    byte* tmp = (byte*)malloc(byteSizeData * sizeof(byte));
    if (!tmp) return MEM_ALLOC;
    else b->bytes = tmp;

    byte temp;
    for (int i = byteSizeResidual, j = 0; i < byteSizeResidual + byteSizeData; i++, j++) { // for each byte from the end of residual bits data
        temp = byteBuffer[i]; // extract byte from read bytes
        b->bytes[j] = temp; // set extracted byte in data
    }
    b->amountOfBytes = byteSizeData;
    b->residualBits = residualBits;
}

void dbug(byte* binary, size_t amountOfBytes) {
    byte b;
    for (unsigned int i = 0; i < amountOfBytes; i++) { // for every byte
        b = binary[i]; // extract byte at position i
        for (unsigned int j = 0; j < sizeBits(sizeof(byte)); j++) { // for each bit in a byte
            if(b & (1 << j)) { // extract bit at position j
                printf("1");
            } else {
                printf("0");
            }
        }
    }
    printf("\n");
}
