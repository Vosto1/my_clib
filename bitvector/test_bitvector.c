#include "test_bitvector.h"

void auto_tests(int tests, int lengthRange)
{
    srand(time(NULL));
    // testing algo:
    // 1 create random (random sequence of bools) bitvector of random length
    // 2 convert to binary and write to file
    // 3 read from file to another bitvector
    // 4 assert that it is equal to the original
    // 5 loop on this algorithm thousands of times.

    bitvector bv = bvcreateEmpty();
    errorHandler();
    bvinit(&bv);
    errorHandler();
    bitvector out = bvcreateEmpty();
    errorHandler();
    bvinit(&out);
    errorHandler();
    binary bin;
    bin.amountOfBytes = 0;
    bin.residualBits = 0;
    bin.bytes = NULL;
    binary fromFile;
    fromFile.amountOfBytes = 0;
    fromFile.residualBits = 0;
    fromFile.bytes = NULL;
    int j;

    // stats
    ticks start, end, testStart, testEnd;
    seconds testTime, teststotalTime;
    size_t boolsWrittenAndRead = 0;

    testStart = now();
    for (int k = 0; k < tests; k++)
    {
        int length = rand() % lengthRange;
        start = now();
        // create bitvector with 0-lengthRange bits
        for (int i = 0; i < length; i++)
        {
            j = rand() % 2;
            switch (j)
            {
            case 0:
                bvadd(&bv, true);
                break;
            case 1:
                bvadd(&bv, false);
                break;
            }
        }
        assert(bv.used == length);

        // convert to binary
        assert(bools2bits(&bv, &bin) == bin.amountOfBytes);
        // write to file
        writeBinaryToFile(&bin, "autotestfile.bin");
        // read file that was written
        readBinaryFromFile("autotestfile.bin", &fromFile);

        // debug
        // printbinary(&bin);
        // printbinary(&fromFile);

        byte b1, b2;
        for (int x = 0; x < bin.amountOfBytes; x++)
        {
            b1 = bin.bytes[x];
            b2 = fromFile.bytes[x];
            assert(b1 == b2);
        }
        bits2bools(&fromFile, &out);
        // debug
        // printbitvector(&bv);
        // printbitvector(&out);
        bool *a;
        bool *b;
        for (int x = 0; x < bv.used; x++)
        {
            a = bvat(&bv, x);
            b = bvat(&out, x);
            assert((*a) == (*b));
        }

        // reset bitvectors
        bvclear(&bv);
        bvclear(&out);
        end = now();
        testTime = diff(start, end);
        printf("bitvector test %d successful (bitvector length %d, test time: %f)\n", k, length, testTime);
        boolsWrittenAndRead += length;
    }
    testEnd = now();
    teststotalTime = diff(testStart, testEnd);
    printf("%d tests passed, bools written to and read from file: %lld time: ", tests, boolsWrittenAndRead);
    char unit = 's';
    if (teststotalTime > 60.0f && teststotalTime < 3600.0f)
    {
        teststotalTime /= 60;
        unit = 'm';
    }
    printf("%f%c\n", teststotalTime, unit);
}

void test_sequence()
{
    srand(time(NULL));

    bitvector bv = bvcreateEmpty();
    errorHandler();
    bvinit(&bv);
    errorHandler();
    bool *newbool;
    int j;
    for (int i = 0; i < 12; i++)
    {
        j = rand() % 2;
        switch (j)
        {
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
}