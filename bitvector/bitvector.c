#include <assert.h>
#include "bitvector.h"

static unsigned int bytes_to_bits(unsigned int bytes);
static void freebit(voidp_t bit);

size_t bit_count(bitvector *bv)
{
    return sda_count(bv);
}

bitvector bv_create_empty()
{
    bitvector bv = sda_create_empty();
    return bv;
}

size_t bv_init(bitvector *bv)
{
    return sda_init(bv, 1, &freebit);
}

size_t bv_add(bitvector *bv, bool value)
{
    bool *newbool = (bool *)malloc(sizeof(bool));
    if (newbool == NULL)
    {
        //errcset(EMEM_ALLOC);
        return 0;
    }
    else
    {
        *newbool = value;
        return sda_insert(bv, (void *)newbool);
    }
}

size_t bv_merge(bitvector *bv, bitvector *mergeWith)
{
    if (bv == NULL || mergeWith == NULL)
    {
        //errcset(ENULL_ARG);
        return 0;
    }
    for (int i = 0; i < bit_count(mergeWith); i++)
        bv_add(bv, mergeWith->array[i]);
    return bit_count(bv);
}

bool bv_remove_last(bitvector *bv)
{
    bool *rm = sda_remove_last(bv);
    if (rm == NULL)
        return false;
    else
    {
        free(rm);
        return true;
    }
}

// at, get value at index
bool *bv_at(bitvector *bv, int index)
{
    if (index >= bv->used)
    {
        //errcset(EINDEX_OUT_OF_BOUNDS);
        return NULL;
    }
    else
        return (bool *)bv->array[index];
}

size_t bv_clear(bitvector *bv)
{
    size_t size = bv->used;
    for (size_t i = 0; i < size; i++)
        assert(bv_remove_last(bv));
    return size;
}

bool bv_delete(bitvector *bv)
{
    return sda_free(bv);
}

// converts a bitvector into an array of bytes
unsigned int bools2bits(bitvector *bv, binary *out)
{
    if (bv == NULL || out == NULL)
    {
        //errcset(ENULL_ARG);
        return 0;
    }
    unsigned int residualBitsInLastByte = bytes_to_bits(sizeof(byte)) - (bit_count(bv) % bytes_to_bits(sizeof(byte)));
    // size of the bin file
    unsigned int bits = bit_count(bv) + residualBitsInLastByte; // file size + the number of residual bits
    unsigned int amountOfBytes = bits / bytes_to_bits(sizeof(byte));
    // init array of bytes
    byte *bin = (byte *)malloc(amountOfBytes * sizeof(byte));
    if (bin == NULL)
    {
        //errcset(EMEM_ALLOC);
        return 0;
    }
    for (unsigned int i = 0; i < amountOfBytes; i++)
    { // for every byte
        byte b;
        for (unsigned int k = 0; k < bytes_to_bits(sizeof(byte)); k++)
        {                     // initialize byte
            b = b & (0 << k); // set all bits to 0
        }
        // in the next for loop we go though the next bytes' bits:
        unsigned int currentBit = bytes_to_bits(i * sizeof(byte)); // (next byte start)
        unsigned int nextByteEndInBits = bytes_to_bits(i * sizeof(byte)) + bytes_to_bits(sizeof(byte));
        // j is where we are in the bit sequence and x is where we are in the current byte (0-8)
        for (unsigned int j = currentBit, x = 0; j < nextByteEndInBits && j < bit_count(bv); j++, x++)
        {                                     // set bits according to bitvector
            bool *bit = (bool *)bv_at(bv, j); // convert from void* to bool*
            if (*bit)
            {
                b = b | (1 << x); // set bit on position x to 1
            }
            // if bit is false then we skip it, because all bits are initialized to 0 in the start of the outer for-loop
        }
        bin[i] = b; // save byte to byte array
    }
    out->residualBits = residualBitsInLastByte;
    out->amountOfBytes = amountOfBytes;
    out->bytes = bin;
    return amountOfBytes;
}

// converts an array of bytes to a bitvector
bool bits2bools(binary *b, bitvector *out)
{
    if (out == NULL)
    {
        //errcset(ENULL_ARG);
        return false;
    }
    
    if (b->bytes == NULL)
    {
        //errcset(EARR_EMPTY);
        return false;
    }

    for (int i = 0; i < b->amountOfBytes; i++)
    {                           // for each byte
        byte tmp = b->bytes[i]; // extract byte at position i
        for (int j = 0; j < bytes_to_bits(sizeof(byte)); j++)
        {                         // for each bit
            if (tmp & (1 << j))   // if there is a 1 at position j
                bv_add(out, true); // add true
            else
                bv_add(out, false); // otherwise there was a 0, add false
        }
    }

    for (int k = 0; k < b->residualBits; k++)
        bv_remove_last(out);

    return true;
}

size_t write_binary_to_file(binary *b, char *file)
{
    byte *res = (byte *)&b->residualBits;
    // create memory buffer with the right amount of bytes
    size_t byteSizeResidual = sizeof(unsigned int);
    size_t byteSizeData = sizeof(byte) * b->amountOfBytes;
    byte *buffer = (byte *)malloc((byteSizeResidual + byteSizeData) * sizeof(byte));
    if (!buffer)
    {
        //errcset(EMEM_ALLOC);
        return 0;
    }
    byte tmp;
    // start at the back for the uint
    int i = 0;
    int j = 0;
    while (i < byteSizeResidual)
    {
        // write residual bits to buffer
        tmp = res[i];
        buffer[i] = tmp;
        i++;
    }
    
    while (j < byteSizeData)
    {
        // write data to buffer
        tmp = b->bytes[j];
        buffer[i] = tmp;
        i++;
        j++;
    }
    size_t fileSize = byteSizeResidual + byteSizeData;

    if (write_file(file, (void *)buffer, fileSize) == fileSize)
    {
        free(buffer);
        return fileSize;
    }
    else
    {
        free(buffer);
        //errcset(EWRITE_BINARY);
        return 0;
    }
}

size_t read_binary_from_file(char *file, binary *b)
{
    void *fileContents;
    size_t fileSize = read_file(file, &fileContents);
    byte *byteBuffer = (byte *)fileContents; // set byte pointer to point to buffer to enable pointer arithmetic

    size_t byteSizeResidual = sizeof(unsigned int);    // residual bits size in bytes
    size_t byteSizeData = fileSize - byteSizeResidual; // get data size in bytes

    unsigned int residualBits = 0;
    byte *res = (byte *)&residualBits;
    for (int i = 0; i < byteSizeResidual; i++) // for each byte in residual bits value
        res[i] = byteBuffer[i];                // set byte i in residualBits to bit i in bytes read from file

    byte *tmp = (byte *)malloc(byteSizeData * sizeof(byte));
    if (!tmp)
    {
        //errcset(EMEM_ALLOC);
        return 0;
    }
    else
        b->bytes = tmp;

    byte temp;
    for (int i = byteSizeResidual, j = 0; i < byteSizeResidual + byteSizeData; i++, j++)
    {                         // for each byte from the end of residual bits data
        temp = byteBuffer[i]; // extract byte from read bytes
        b->bytes[j] = temp;   // set extracted byte in data
    }
    free(fileContents); // free memory allocated by file reading utility
    b->amountOfBytes = byteSizeData;
    b->residualBits = residualBits;
    return byteSizeData;
}

#define BITS_IN_BYTE 8;

static unsigned int bytes_to_bits(unsigned int bytes)
{
    return bytes * BITS_IN_BYTE;
}

static void freebit(voidp_t bit)
{
    free(bit);
}

void printbitvector(bitvector *bv)
{
    for (int i = 0; i < bit_count(bv); i++)
    {
        bool *b = (bool *)bv_at(bv, i);
        if (*b)
            printf("value at %d: [true]\n", i);
        else
            printf("value at %d: [false]\n", i);
    }
    printf("\n");
}

void printbinary(binary *bin)
{
    byte b;
    printf("byte count: %d\n", bin->amountOfBytes);
    printf("residual: %d\n", bin->residualBits);
    printf("binary: ");
    for (unsigned int i = 0; i < bin->amountOfBytes; i++)
    {                      // for every byte
        b = bin->bytes[i]; // extract byte at position i
        for (unsigned int j = 0; j < bytes_to_bits(sizeof(byte)); j++)
        { // for each bit in a byte
            if (b & (1 << j))
            { // extract bit at position j
                printf("1");
            }
            else
            {
                printf("0");
            }
        }
    }
    printf("\n");
}

binary bin_new()
{
    binary bin;
    bin.amountOfBytes = 0;
    bin.residualBits = 0;
    bin.bytes = NULL;
    return bin;
}

bool bin_equal(binary x, binary y)
{
    if (x.amountOfBytes != y.amountOfBytes || x.residualBits != y.residualBits)
        return false;
    // check if both binaries are equal
    byte b1, b2;
    for (int i = 0; i < x.amountOfBytes; i++)
    {
        b1 = x.bytes[i];
        b2 = y.bytes[i];
        if (b1 != b2)
            return false;
    }
    return true;
}

unsigned int bin_amount_bytes(binary x)
{
    return x.amountOfBytes;
}

bool bin_free(binary x)
{
    if (x.bytes == NULL)
        return false;
    free(x.bytes);
    return true;
}
