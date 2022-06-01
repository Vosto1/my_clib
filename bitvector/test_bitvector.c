#include "test_bitvector.h"

void test_sequence() {
    srand(time(NULL));

    bitvector bv = bvcreateEmpty();
    errorHandler();
    bvinit(&bv);
    errorHandler();
    bool* newbool;
    int j;
    for (int i = 0; i < 12; i++) {
        j = rand() % 2;
        switch (j) {
            case 0:
                bvadd(&bv, true);
                break; 
            case 1:
                bvadd(&bv, false);
                break;
        }
    }
    printbitvector(&bv);

    binary b;
    b.residualBits = 0;
    b.amountOfBytes = 0;
    b.bytes = NULL;

    bools2bits(&bv, &b);
    printbinary(&b);

    bitvector out;
    bits2bools(&b, &out);


    printf("converted from:\n");
    printbitvector(&bv);
    printf("convertion back, result:\n");
    printbitvector(&out);

    printf("written bits:\n");
    printbinary(&b);
    assert(writeBinaryToFile(&b, "./test2.txt") != -1);
    errorHandler();
    binary read;
    assert(readBinaryFromFile("./test2.txt", &read) != -1);
    printf("read bits:\n");
    printbinary(&read);

    bitvector readbv;
    bits2bools(&read, &readbv);

    printbitvector(&readbv);

    // testing algo: 
    // 1 create random bitvector of random length
    // 2 convert to binary and write to file
    // 3 read from file to another bitvector
    // 4 assert that it is equal to the original
    // 5 loop on this algorithm thousands of times.
}