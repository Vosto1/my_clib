#include <assert.h>
#include "bitvector.h"

static uint bytes_to_bits(uint bytes);
static void freebit(void* bit);

uint bit_count(bitvector *bv)
{
    return sda_count(bv);
}

bitvector bv_create_empty()
{
    bitvector bv = sda_create_empty();
    return bv;
}

int bv_init(bitvector *bv)
{
    return sda_init(bv, 1, &freebit);
}

int bv_add(bitvector *bv, bool value)
{
    bool *newbool = (bool *)malloc(sizeof(bool));
    if (newbool == NULL)
    {
        return ERRMEM;
    }
    else
    {
        *newbool = value;
        return sda_insert(bv, (void *)newbool);
    }
}

int bv_merge(bitvector *bv, bitvector *mergeWith)
{
    if (bv == NULL || mergeWith == NULL)
    {
        return NULLARG;
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
bool *bv_at(bitvector *bv, uint index)
{
    return (bool *)sda_at(bv, index);
}

uint bv_clear(bitvector *bv)
{
    uint size = sda_count(bv);
    for (uint i = 0; i < size; i++)
        assert(bv_remove_last(bv));
    return size;
}

bool bv_delete(bitvector *bv)
{
    return sda_free(bv);
}

// converts a bitvector into an array of bytes
int bools2bits(bitvector *bv, binary *out)
{
    if (bv == NULL || out == NULL)
    {
        return NULLARG;
    }
    uint residualBitsInLastByte = bytes_to_bits(sizeof(byte)) - (bit_count(bv) % bytes_to_bits(sizeof(byte)));
    // size of the bin file
    uint bits = bit_count(bv) + residualBitsInLastByte; // file size + the number of residual bits
    uint amountOfBytes = bits / bytes_to_bits(sizeof(byte));
    // init array of bytes
    byte *bin = (byte *)malloc(amountOfBytes * sizeof(byte));
    if (bin == NULL)
    {
        return ERRMEM;
    }
    for (uint i = 0; i < amountOfBytes; i++)
    { // for every byte
        byte b;
        for (uint k = 0; k < bytes_to_bits(sizeof(byte)); k++)
        {                     // initialize byte
            b = b & (0 << k); // set all bits to 0
        }
        // in the next for loop we go though the next bytes' bits:
        uint currentBit = bytes_to_bits(i * sizeof(byte)); // (next byte start)
        uint nextByteEndInBits = bytes_to_bits(i * sizeof(byte)) + bytes_to_bits(sizeof(byte));
        // j is where we are in the bit sequence and x is where we are in the current byte (0-8)
        for (uint j = currentBit, x = 0; j < nextByteEndInBits && j < bit_count(bv); j++, x++)
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
        return false;
    }
    
    if (b->bytes == NULL)
    {
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

int write_binary_to_file(binary *b, char *file)
{
    byte *res = (byte *)&b->residualBits;
    // create memory buffer with the right amount of bytes
    uint byteSizeResidual = sizeof(uint);
    uint byteSizeData = sizeof(byte) * b->amountOfBytes;
    byte *buffer = (byte *)malloc((byteSizeResidual + byteSizeData) * sizeof(byte));
    if (!buffer)
    {
        return ERRMEM;
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
    uint fileSize = byteSizeResidual + byteSizeData;

    if (write_file(file, (void *)buffer, fileSize) == fileSize)
    {
        free(buffer);
        return fileSize;
    }
    else
    {
        free(buffer);
        return ERRWRITE;
    }
}

int read_binary_from_file(char *file, binary *b)
{
    void *fileContents;
    uint fileSize = read_file(file, &fileContents);
    byte *byteBuffer = (byte *)fileContents; // set byte pointer to point to buffer to enable pointer arithmetic

    uint byteSizeResidual = sizeof(uint);    // residual bits size in bytes
    uint byteSizeData = fileSize - byteSizeResidual; // get data size in bytes

    uint residualBits = 0;
    byte *res = (byte *)&residualBits;
    for (int i = 0; i < byteSizeResidual; i++) // for each byte in residual bits value
        res[i] = byteBuffer[i];                // set byte i in residualBits to bit i in bytes read from file

    byte *tmp = (byte *)malloc(byteSizeData * sizeof(byte));
    if (!tmp)
    {
        return ERRMEM;
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

static uint bytes_to_bits(uint bytes)
{
    return bytes * BITS_IN_BYTE;
}

static void freebit(void* bit)
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
    for (uint i = 0; i < bin->amountOfBytes; i++)
    {                      // for every byte
        b = bin->bytes[i]; // extract byte at position i
        for (uint j = 0; j < bytes_to_bits(sizeof(byte)); j++)
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

uint bin_amount_bytes(binary x)
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
