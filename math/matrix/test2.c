// this file purpose is to test the speed upgrade from memoization
#include "test2.h"

#define START_INDEX 2
#define NEW_ALGO_MAX 15
#define OLD_ALGO_MAX 11

static void print(const char *operation, int dimensions, seconds time, int calls)
{
    printf("computed %s with matrix with dimensions %dx%d during %.2f seconds with %d calls\n", operation, dimensions, dimensions, time, calls);
}

static void print_test_header1()
{
    printf("#################################################################### new algorithm test run ####################################################################\n");
}

static void print_test_header2()
{
    printf("############################################################### new vs old algorithm comparison ################################################################\n");
}

void comparison(int dim) // dim == matrix dimensions
{
    char str[64];
    ticks start, end;
    double determinant;
    Matrix matrixrand;
    errorHandler1(makeMatrix(dim, dim, &matrixrand));
    errorHandler1(initializeMatrixRandomNumbers(matrixrand, rand() % 100 + 2, 1));
    start = now();
    errorHandler1(getDeterminant(&matrixrand, &determinant));
    end = now();

    sprintf(str, "%s: %*.2f", "determinant new algorithm", 40, determinant);
    print(str, dim, diff(start, end), CNT);

    start = now();
    errorHandler1(getDeterminantSlow(&matrixrand, &determinant));
    end = now();

    sprintf(str, "%s: %*.2f", "determinant old algorithm", 40, determinant);
    print(str, dim, diff(start, end), CNT);

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

    sprintf(str, "%s: %*.2f", "determinant new algorithm", 40, determinant);
    print(str, dim, diff(start, end), CNT);

    freeMatrix(&matrixrand);
}

void test2()
{
    srand(time(NULL));
    // i == matrix dimension to test with
    int i;

    print_test_header1();
    for (i = START_INDEX; i < NEW_ALGO_MAX; i++)
        testloop(i);

    print_test_header2();
    for (i = START_INDEX; i <= OLD_ALGO_MAX; i++)
        comparison(i);
}