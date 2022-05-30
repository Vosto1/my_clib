#include "test_bitVector.h"

void test_sequence() {
    ErrorCode e;
    bool array[] = {true, false, true, false, true, true, false};
    int arrSize = sizeof(array)/sizeof(array[0]);
    //writeFile("test.txt", array, sizeof(bool) * 7);

    bitVector bv = createEmptyBitVector();
    e = convert(array, &bv, arrSize);
    errorHandler(e);

    printData(bitCount(&bv), bv.array);

    binary b;
    b.residualBits = 0;
    b.amountOfBytes = 0;
    b.bytes = NULL;

    bools2bits(&bv, &b);
    dbug(b.bytes, b.amountOfBytes);

    bitVector out;
    bits2bools(&b, &out);


    printf("sent bits:\n");
    printData(bitCount(&bv), bv.array);
    printf("read bits:\n");
    printData(bitCount(&out), out.array);

    printf("written bits:\n");
    dbug(b.bytes, b.amountOfBytes);
    e = writeBinaryFromFile(&b, "./test2.txt");
    binary read;
    e = readBinaryFromFile("./test2.txt", &read);
    printf("read bits:\n");
    dbug(read.bytes, read.amountOfBytes);
    printf("read res: %d, bytes: %d\n", read.residualBits, read.amountOfBytes);

    bitVector readbv;
    bits2bools(&read, &readbv);

    printData(readbv.used, readbv.array);
}