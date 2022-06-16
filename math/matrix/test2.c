// this file purpose is to test the speed upgrade from memoization
#include "test2.h"

static void print(const char *operation, int dimensions, seconds time, int calls)
{
    printf("computed %s with matrix with dimensions %dx%d during %fs with %d calls\n", operation, dimensions, dimensions, time, calls);
}

void testloop(int dim) // dim == matrix dimensions
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

    sprintf(str, "%s: %.2f", "determinant new algorithm", determinant);
    print(str, dim, diff(start, end), CNT);

    start = now();
    errorHandler1(getDeterminantSlow(&matrixrand, &determinant));
    end = now();

    sprintf(str, "%s: %.2f", "determinant old algorithm", determinant);
    print(str, dim, diff(start, end), CNT);

    freeMatrix(&matrixrand);
}

void test2()
{
    srand(time(NULL));
    // i == matrix dimension to test with
    for (int i = 2; i <= 11; i++)
        testloop(i);
}