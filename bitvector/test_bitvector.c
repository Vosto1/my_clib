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

    errcinit();

    bitvector bv = bv_create_empty();
    bv_init(&bv);
    error_handler();
    bitvector out = bv_create_empty(); // this is initialized in bits2bools
    binary bin = bin_new();
    binary fromFile = bin_new();
    int j;

    // stats
    ticks start, end, testStart, testEnd;
    seconds testTime, teststotalTime, avg = 0.0f;
    size_t boolsWrittenAndRead = 0;
    int avglen = 0;

    testStart = now();
    for (int k = 0; k < tests; k++)
    {
        int length = (rand() % lengthRange) + 1;
        start = now();
        // create bitvector with 1-lengthRange bits
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
        assert(bit_count(&bv) == length);

        // convert to binary
        assert(bools2bits(&bv, &bin) == bin_amount_bytes(bin));
        error_handler();
        // write to file
        assert(write_binary_to_file(&bin, "autotestfile.bin") != 0);
        error_handler();
        // read file that was written
        assert(read_binary_from_file("autotestfile.bin", &fromFile) != 0);
        error_handler();

        // assure that both binaries are equal
        assert(bin_equal(bin, fromFile));
        
        bv_init(&out);   // initialize
        error_handler();
        assert(bits2bools(&fromFile, &out) != false);
        error_handler();
        
        // check both bvs are equal (bv read from file and original)
        bool *a;
        bool *b;
        for (int x = 0; x < bv.used; x++)
        {
            a = bv_at(&bv, x);
            error_handler();
            b = bv_at(&out, x);
            error_handler();
            assert((*a) == (*b));
        }

        // reset bitvectors
        if (bit_count(&bv) != 0)
        {
           assert(bv_clear(&bv) != 0);
            error_handler();
        }
        
        if (bit_count(&out) != 0)
        {
            assert(bv_clear(&out) != 0);
            error_handler();
        }
        
        bv_delete(&out); // test delete
        error_handler();
        end = now();
        testTime = diff(start, end);
        printf("bitvector test %d successful (bitvector length %d, test time: %f)\n", k, length, testTime);
        boolsWrittenAndRead += length;
        avg += testTime;
        avglen += length;
        bin_free(bin);      // free binaries
        bin_free(fromFile); // free binaries
    }
    testEnd = now();
    teststotalTime = diff(testStart, testEnd);
    avg /= tests; // calc average exec time
    avglen /= tests;
    printf("%d tests passed, bools written to and read from file: %lld times. Total time: ", tests, boolsWrittenAndRead);
    char unit = 's';
    if (teststotalTime > 60.0f && teststotalTime < 3600.0f)
    {
        teststotalTime /= 60.0f;
        unit = 'm';
    }
    else if (teststotalTime >= 3600.0f)
    {
        teststotalTime /= 3600.0f;
        unit = 'h';
    }
    printf("%f%c, with an average time of %fs (average bv length: %d)\n", teststotalTime, unit, avg, avglen);
    bv_delete(&bv);
    error_handler();
    /*bv_delete(&out);
    error_handler();*/
}

void test_sequence()
{
    srand(time(NULL));
    errcinit();

    bitvector bv = bv_create_empty();
    error_handler();
    bv_init(&bv);
    error_handler();
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

    bitvector out = bv_create_empty();
    bv_init(&out);
    assert(bits2bools(&b, &out) != false);

    printf("converted from:\n");
    printbitvector(&bv);
    bv_delete(&bv); // free
    error_handler();
    printf("convertion back, result:\n");
    printbitvector(&out);
    bv_delete(&out); // free
    error_handler();

    printf("written bits:\n");
    printbinary(&b);
    assert(write_binary_to_file(&b, "./test2.txt") != 0);
    error_handler();
    free(b.bytes); // free
    binary read;
    assert(read_binary_from_file("./test2.txt", &read) != 0);
    printf("read bits:\n");
    printbinary(&read);

    bitvector readbv = bv_create_empty();
    bv_init(&readbv);
    assert(bits2bools(&read, &readbv) != false);
    free(read.bytes); // free

    printbitvector(&readbv);
    bv_delete(&readbv); // free
    error_handler();
}
