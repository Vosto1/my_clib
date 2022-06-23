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

    bitvector bv = bv_create_empty();
    error_handler();
    bv_init(&bv);
    error_handler();
    bitvector out = bv_create_empty();
    error_handler();
    bv_init(&out);
    error_handler();
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
    seconds testTime, teststotalTime, avg = 0.0f;
    dim_t boolsWrittenAndRead = 0;
    int avglen = 0;

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
                assert(bv_add(&bv, true) != 0);
                break;
            case 1:
                assert(bv_add(&bv, false) != 0);
                break;
            }
        }
        assert(bv.used == length);

        // convert to binary
        assert(bools2bits(&bv, &bin) == bin.amountOfBytes);
        // write to file
        assert(write_binary_to_file(&bin, "autotestfile.bin") != 0);
        // read file that was written
        assert(read_binary_from_file("autotestfile.bin", &fromFile) != 0);

        // check both binaries are equal
        byte b1, b2;
        for (int x = 0; x < bin.amountOfBytes; x++)
        {
            b1 = bin.bytes[x];
            b2 = fromFile.bytes[x];
            assert(b1 == b2);
        }
        assert(bits2bools(&fromFile, &out) != false);
        
        // check both bvs are equal (bv read from file and original)
        bool *a;
        bool *b;
        for (int x = 0; x < bv.used; x++)
        {
            a = bv_at(&bv, x);
            b = bv_at(&out, x);
            assert((*a) == (*b));
        }

        // reset bitvectors
        assert(bv_clear(&bv) != 0);
        assert(bv_clear(&out) != 0);
        end = now();
        testTime = diff(start, end);
        printf("bitvector test %d successful (bitvector length %d, test time: %f)\n", k, length, testTime);
        boolsWrittenAndRead += length;
        avg += testTime;
        avglen += length;
    }
    testEnd = now();
    teststotalTime = diff(testStart, testEnd);
    avg /= tests; // calc average exec time
    avglen /= tests;
    printf("%d tests passed, bools written to and read from file: %lld time: ", tests, boolsWrittenAndRead);
    char unit = 's';
    if (teststotalTime > 60.0f && teststotalTime < 3600.0f)
    {
        teststotalTime /= 60;
        unit = 'm';
    }
    printf("%f%c, with an average time of %fs (average bv length: %d)\n", teststotalTime, unit, avg, avglen);
}

void test_sequence()
{
    srand(time(NULL));

    bitvector bv = bv_create_empty();
    error_handler();
    bv_init(&bv);
    error_handler();
    bool *newbool;
    int j;
    for (int i = 0; i < 12; i++)
    {
        j = rand() % 2;
        switch (j)
        {
        case 0:
            assert(bv_add(&bv, true) != 0);
            break;
        case 1:
            assert(bv_add(&bv, false) != 0);
            break;
        }
    }
    printbitvector(&bv);

    binary b;
    b.residualBits = 0;
    b.amountOfBytes = 0;
    b.bytes = NULL;

    assert(bools2bits(&bv, &b) != 0);
    printbinary(&b);

    bitvector out;
    assert(bits2bools(&b, &out) != false);

    printf("converted from:\n");
    printbitvector(&bv);
    printf("convertion back, result:\n");
    printbitvector(&out);

    printf("written bits:\n");
    printbinary(&b);
    assert(write_binary_to_file(&b, "./test2.txt") != 0);
    error_handler();
    binary read;
    assert(read_binary_from_file("./test2.txt", &read) != 0);
    printf("read bits:\n");
    printbinary(&read);

    bitvector readbv;
    assert(bits2bools(&read, &readbv) != false);

    printbitvector(&readbv);
}