// this file purpose is to test the speed upgrade from memoization
#include "test2.h"

#define START_INDEX 2
#define NEW_ALGO_MAX 5
#define OLD_ALGO_MAX 4

#define DET_SPACES 30

static void print(const char *operation, int dimensions, seconds time, int calls)
{
    printf("computed %s with matrix with dimensions %dx%d during %.5f seconds with %d calls\n", operation, dimensions, dimensions, time, calls);
}

static void print_test_header1()
{
    printf("#################################################################### new algorithm test run ####################################################################\n");
}

static void print_test_header2()
{
    printf("############################################################### new vs old algorithm comparison ################################################################\n");
}

static void print_separator()
{
    printf("\n");
}

void comparison(int dim) // dim == matrix dimensions
{
    char str[1024];
    ticks start, end;
    seconds diff1, diff2;
    double determinant;
    double percent;
    Matrix matrixrand;

    errorHandler1(makeMatrix(dim, dim, &matrixrand));
    errorHandler1(initializeMatrixRandomNumbers(matrixrand, rand() % 100 + 2, 1));
    start = now();
    errorHandler1(getDeterminant(&matrixrand, &determinant));
    end = now();

    sprintf(str, "%s: %*.2f", "determinant new algorithm", DET_SPACES, determinant);
    diff1 = diff(start, end);
    print(str, dim, diff1, CNT);

    start = now();
    errorHandler1(getDeterminantSlow(&matrixrand, &determinant));
    end = now();

    sprintf(str, "%s: %*.2f", "determinant old algorithm", DET_SPACES, determinant);
    diff2 = diff(start, end);
    print(str, dim, diff2, CNT);
    if (diff1 < diff2) // if new algo was faster
    {
        if (diff1 != 0.0)
            percent = ((diff2/diff1) - 1) * 100; // how many times does diff1 (fast time) fit in diff2 (slow time)? --> times% faster (eg. 1.54 --> 154%)
        else
            percent = 0.0;
        printf("new algorithm was %.1f%% faster\n", percent);
    }
    else if (diff2 < diff1) // if old algo was faster
    {
        if (diff2 != 0.0)
            percent = ((diff1/diff2) - 1) * 100; // how many times does diff1 (fast time) fit in diff2 (slow time)? --> times% faster (eg. 1.54 --> 154%)
        else
            percent = 0.0;
        printf("old algorithm was %.1f%% faster\n", percent);
    }
    else
    {
        printf("Neither algorithm was faster.\n");
    }
    print_separator();
    freeMatrix(&matrixrand);
}

void testloop(int dim) // dim == matrix dimensions
{
    char str[1024];
    ticks start, end;
    double determinant;
    Matrix matrixrand;
    errorHandler1(makeMatrix(dim, dim, &matrixrand));
    errorHandler1(initializeMatrixRandomNumbers(matrixrand, rand() % 100 + 2, 1));
    start = now();
    errorHandler1(getDeterminant(&matrixrand, &determinant));
    end = now();

    sprintf(str, "%s: %*.2f", "determinant new algorithm", DET_SPACES, determinant);
    print(str, dim, diff(start, end), CNT);

    freeMatrix(&matrixrand);
}

void test2()
{
    srand(time(NULL));
    // i == matrix dimension to test with
    
    //testloop(10);
    int i;

    print_test_header1();
    for (i = START_INDEX; i < NEW_ALGO_MAX; i++)
        testloop(i);

    print_test_header2();
    for (i = START_INDEX; i <= OLD_ALGO_MAX; i++);
        //comparison(i);
}