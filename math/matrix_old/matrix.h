#ifndef MATRIX_H
#define MATRIX_H

#include "../../s_dynamic_array/sdarray.h"
#include "../../hash_dictionary/hashtable.h"
#include "../../hash_dictionary/hash.h"
#include "../../hash_dictionary/hash.h"
#include "../../utils/counter.h"
#include "list.h"
#include "data.h"
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <assert.h>

// index macro, works for all matrices
#define INDEX(column, row, nrOfColumns) ((column)*nrOfColumns + (row))

enum errorCodes
{
	SUCCESS1 = 0,
	ERR_MAKE_MATRIX = 1,
	ERR_NON_SQUARE_MATRIX = 2,
	ERR_MATRIX_IS_NULL = 3,
	ERR_NUMBER_OF_ARGUMENTS_DONT_MATCH = 4,
	ERR_FREE_MATRIX_IS_NULL = 5,
	ERR_DET_MATRIX_IS_NULL = 6,
	ERR_INIT_MATRIX_IS_NULL = 7,
	ERR_CLONE = 8,
	ERR_MUST_BE_DOUBLE = 9,
	ERR_TEST_FUNC_INVALID_ARGUMENT = 10,
	ERR_INV_MTRX_DETERMINANT_DIV0 = 11,
};

typedef int ErrorCode1;

typedef struct
{
	Data *matrix; // An array of Data type
	int rows;
	int columns;
} matrix_data_type;

typedef matrix_data_type *Matrix;

// Calls exit() with errorcode number if an error occurs
void errorHandler1(int err);
// Prints matrix
void printMatrix(Matrix mtrx);
// Frees the allocated memory for the matrix. The pointer will point at NULL afterwards.
void freeMatrix(Matrix *mtrx);
// Initializes the matrix with column number * 10 + row number.
ErrorCode1 matrixTestInitialization(Matrix mtrx);
// Initializes the matrix with data from the list. The list must have rows * columns amount of data.
ErrorCode1 initializeMatrixFromList(Matrix mtrx, List list);
// Initializes the matrix with random numbers between min and max.
ErrorCode1 initializeMatrixRandomNumbers(Matrix mtrx, int max, int min);
// send allocated matrices, input needs to be initialized. Output is the clone.
ErrorCode1 copyto(Matrix input, Matrix output);
// declare size and send in a gm pointer and. Pointer is assigned uninitialized matrix with inputed size.
ErrorCode1 makeMatrix(int rows, int columns, Matrix *mtrx);
// Gets the determinant of the inputed matrix. Matrix must be a square matrix. Determinant is assigned to what pdet points at.
ErrorCode1 getDeterminant(Matrix *mtrx, Data *pdet);
ErrorCode1 getDeterminantSlow(Matrix *mtrx, Data *pdet);
// Get cofactor matrix of input matrix.
ErrorCode1 getCofactorMatrix(Matrix *mtrx, Matrix *resultMatrix);
// transpose input matrix
ErrorCode1 transposeMatrix(Matrix *mtrx);
// get adjugate of input matrix
ErrorCode1 getAdjugateMatrix(Matrix *mtrx);
// multiply all elements in the input matrix with the scalar
ErrorCode1 multiplyMatrixWithScalarValue(Matrix *mtrx, Data scalar);
// get inverse matrix from the input matrix
ErrorCode1 getInverseMatrix(Matrix *mtrx);

// Try some features of the library (interactive)
void _menu();
// Test the functions of the library (light)
void matrixTestFunction();

#endif
