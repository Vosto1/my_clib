#include "bitvector.h"


size_t bitCount(bitvector* bv) {
    return sda_count(bv);
}

bitvector bvcreateEmpty() {
    bitvector bv = sda_createEmpty();
    return bv;
}

size_t bvinit(bitvector* bv) {
    return sda_init(bv, 1);
}

size_t bvadd(bitvector* bv, bool value) {
    bool* newbool = (bool*)malloc(sizeof(bool));
    if (newbool == NULL) {
        errcset(EMEM_ALLOC);
        return -1;
    } else {
        *newbool = value;
        return sda_insert(bv, (void*)newbool);
    }
}

size_t bvunion(bitvector* bv, bitvector* unionWith) {
    return sda_union(bv, unionWith);
}

bool bvremoveLast(bitvector* bv) {
    bool* rm = sda_removeLast(bv);
    if (rm == NULL) return false;
    else {
        free(rm);
        return true;
    }
}

// at == get value at index
bool* bvat(bitvector *bv, int index) {
    if (index >= bv->used) {
        errcset(EARR_INDEX_OCCUPIED);
        return NULL;
    } else return (bool*)bv->array[index];
}

size_t bvclear(bitvector* bv) {
    return sda_clear(bv);
}

// converts a bitvector into an array of bytes
unsigned int bools2bits(bitvector* bv, binary* out) {
    if (bv == NULL || out == NULL) {
        errcset(ENULL_ARG);
        return -1;
    } 
    unsigned int residualBitsInLastByte = sizeBits(sizeof(byte)) - (bitCount(bv) % sizeBits(sizeof(byte)));
    // size of the bin file
    unsigned int bits = bitCount(bv)  + residualBitsInLastByte; // file size + the number of residual bits
    unsigned int amountOfBytes = bits / sizeBits(sizeof(byte));
    // init array of bytes
    byte* bin = (byte*)malloc(amountOfBytes * sizeof(byte));
    if (bin == NULL) { 
        errcset(EMEM_ALLOC);
        return -1;
    }
    for (unsigned int i = 0; i < amountOfBytes; i++) { // for every byte
        byte b;
        for (unsigned int k = 0; k < sizeBits(sizeof(byte)); k++) { // initialize byte
            b = b & (0 << k); // set all bits to 0
        }
        // in the next for loop we go though the next bytes' bits:
        unsigned int currentBit = sizeBits(i*sizeof(byte)); // (next byte start)
        unsigned int nextByteEndInBits = sizeBits(i*sizeof(byte)) + sizeBits(sizeof(byte));
        // j is where we are in the bit sequence and x is where we are in the current byte (0-8)
        for(unsigned int j = currentBit, x = 0; j < nextByteEndInBits && j < bitCount(bv); j++, x++) { // set bits according to bitvector
            bool* bit = (bool*)bv->array[j]; // convert from void* to bool*
            if (*bit) {
                b = b | (1 << x); // set bit on position x to 1
            }
        }
        bin[i] = b;
    }
    out->residualBits = residualBitsInLastByte;
    out->amountOfBytes = amountOfBytes;
    out->bytes = bin;
    return amountOfBytes;
}

// converts an array of bytes to a bitvector
size_t bits2bools(binary* b, bitvector* out) {
    if(b->bytes == NULL) {
        errcset(EARR_EMPTY);
        return -1; 
    }
    // initialize -->
    bitvector bv = bvcreateEmpty();
    if(bvinit(&bv) == -1) {
        return -1;
    }
    // <--

    for (int i = 0; i < b->amountOfBytes; i++) { // for each byte
        byte tmp = b->bytes[i]; // extract byte at position i
        for (int j = 0; j < sizeBits(sizeof(byte)); j++) { // for each bit
            if (tmp & (1 << j)) // if there is a 1 at position j 
                bvadd(&bv, true); // add true
            else
                bvadd(&bv, false); // otherwise there was a 0, add false
        }
    }

    for (int k = 0; k < b->residualBits; k++)
        bvremoveLast(&bv);

    *out = bv;
    return bv.used;
}

size_t writeBinaryToFile(binary* b, char* file) {
    byte* res = (byte*)&b->residualBits;
    // create memory buffer with the right amount of bytes
    size_t byteSizeResidual = sizeof(unsigned int);
    size_t byteSizeData = sizeof(byte) * b->amountOfBytes;
    byte* buffer = (byte*)malloc((byteSizeResidual + byteSizeData) * sizeof(byte));
    if (!buffer) {
        errcset(EMEM_ALLOC);
        return -1;
    }
    byte tmp;
    // start at the back for the uint
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
    
    if(writeFile(file, (void*)buffer, fileSize) == fileSize)
        return fileSize;
    else {
        errcset(EWRITE_BINARY);
        return -1;
    }
}

size_t readBinaryFromFile(char* file, binary* b) {
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
    if (!tmp) {
        errcset(EMEM_ALLOC);
        return -1;
    }
    else b->bytes = tmp;

    byte temp;
    for (int i = byteSizeResidual, j = 0; i < byteSizeResidual + byteSizeData; i++, j++) { // for each byte from the end of residual bits data
        temp = byteBuffer[i]; // extract byte from read bytes
        b->bytes[j] = temp; // set extracted byte in data
    }
    b->amountOfBytes = byteSizeData;
    b->residualBits = residualBits;
    return byteSizeData;
}


const unsigned int bitsinBYTE = 8;

static unsigned int sizeBits(unsigned int bytes) {
    return bytes * bitsinBYTE;
}

void printbitvector(bitvector* bv) {
    for (int i = 0; i < bitCount(bv); i++) {
        bool* b = (bool*)bv->array[i];
        if (*b)
            printf("value at %d: [true]\n", i);
        else
            printf("value at %d: [false]\n", i);
    } printf("\n");
}

void printbinary(binary* bin) {
    byte b;
    printf("byte count: %d\n", bin->amountOfBytes);
    printf("residual: %d\n", bin->residualBits);
    printf("binary: ");
    for (unsigned int i = 0; i < bin->amountOfBytes; i++) { // for every byte
        b = bin->bytes[i]; // extract byte at position i
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
