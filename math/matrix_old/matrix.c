#include "matrix.h"

// In-file helper function definitions
static Data determinantDivideAndConquer(Matrix *mtrx, hashtable *ht);
char *getErrorString(int err);
void printString(char *str);
bool isError(int err);

char *getErrorString(int err)
{
	switch (err)
	{
	case ERR_MAKE_MATRIX:
		return "creating matrix failed.";
		break;
	case ERR_NON_SQUARE_MATRIX:
		return "non square matrix.";
		break;
	case ERR_MATRIX_IS_NULL:
		return "matrix was null pointer.";
		break;
	case ERR_NUMBER_OF_ARGUMENTS_DONT_MATCH:
		return "number of arguments don't match.";
		break;
	case ERR_FREE_MATRIX_IS_NULL:
		return "matrix pointer was null on free.";
		break;
	case ERR_DET_MATRIX_IS_NULL:
		return "matrix pointer was null on determinant calculation.";
		break;
	case ERR_INIT_MATRIX_IS_NULL:
		return "matrix pointer was null on initialization.";
		break;
	case ERR_CLONE:
		return "clone failed.";
		break;
	case ERR_MUST_BE_DOUBLE:
		return "value must be double.";
		break;
	case ERR_TEST_FUNC_INVALID_ARGUMENT:
		return "test funciton invalid argument.";
		break;
	case ERR_INV_MTRX_DETERMINANT_DIV0:
		return "inverse matrix failed. Determinant was zero resulting in division by zero.";
		break;
	default:
		return "Uknown error";
		break;
	}
}

void printString(char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
		printf("%c", str[i]);

	printf("\n");
}

bool isError(int err)
{
	return err != SUCCESS1;
}

void errorHandler1(int err)
{
	if (isError(err))
	{
		printf("\nError %d ", err);
		printString(getErrorString(err));
		// exit(errc);
	}
	else
		return;
}

void printMatrix(Matrix m)
{
	for (int i = 0; i < m->columns; i++)
	{
		for (int j = 0; j < m->rows; j++)
			printf("%*.2f ", 5, m->matrix[INDEX(i, j, m->columns)]);
		printf("\n");
	}
	printf("\n\n");
}

void freemtrx2(void* p)
{
	Matrix* m = (Matrix*)p;
	freeMatrix(m);
}
void freeMatrix(Matrix *mtrx)
{
	/* if ((*mtrx) == NULL)
		return ERR_FREE_MATRIX_IS_NULL;
	else if ((*mtrx)->matrix == NULL)
		return ERR_FREE_MATRIX_IS_NULL; */

	free((*mtrx)->matrix);
	(*mtrx)->matrix = NULL;
	free((*mtrx));
	*mtrx = NULL;
	// return SUCCESS1;
}

ErrorCode1 matrixTestInitialization(Matrix mtrx)
{
	if (!mtrx || !mtrx->matrix)
		return ERR_INIT_MATRIX_IS_NULL;

	int column_value = 0;
	int row_value = 0;
	int row_col_value = 0;
	for (int row = 0; row < mtrx->rows; row++)
	{
		for (int col = 0; col < mtrx->columns; col++)
		{
			row_col_value = row_value + column_value;					  // sets values of matrix to indecies (does not work above 10x10 matrices)
			mtrx->matrix[INDEX(col, row, mtrx->columns)] = row_col_value; // put value later
			column_value++;
		}
		column_value = 0;
		row_value += 10;
	}
	return SUCCESS1;
}

ErrorCode1 initializeMatrixFromList(Matrix mtrx, List list)
{
	if (mtrx->columns * mtrx->rows != numberOfNodesInList(list))
		return ERR_NUMBER_OF_ARGUMENTS_DONT_MATCH;
	if (!mtrx || !mtrx->matrix)
		return ERR_INIT_MATRIX_IS_NULL;

	List listIterator = list;
	for (int row = 0; row < mtrx->rows; row++)
	{
		for (int col = 0; col < mtrx->columns; col++)
		{
			mtrx->matrix[INDEX(col, row, mtrx->columns)] = listIterator->data;
			listIterator = listIterator->next;
		}
	}
	return SUCCESS1;
}

ErrorCode1 initializeMatrixRandomNumbers(Matrix mtrx, int max, int min)
{
	if (!mtrx || !mtrx->matrix)
		return ERR_INIT_MATRIX_IS_NULL;

	for (int row = 0; row < mtrx->rows; row++)
	{
		for (int col = 0; col < mtrx->columns; col++)
		{
			mtrx->matrix[INDEX(col, row, mtrx->columns)] = rand() % (max - min) + min;
		}
	}
	return SUCCESS1;
}

ErrorCode1 copyto(Matrix input, Matrix output)
{
	if (input->rows != output->rows || input->columns != output->columns)
		return ERR_CLONE;

	for (int row = 0; row < output->rows; row++)
	{
		for (int col = 0; col < output->columns; col++)
		{
			output->matrix[INDEX(col, row, output->columns)] = input->matrix[INDEX(col, row, input->columns)];
		}
	}
	return SUCCESS1;
}

ErrorCode1 makeMatrix(int rows, int columns, Matrix *mtrx)
{
	Matrix temp = (matrix_data_type *)malloc(sizeof(matrix_data_type));
	Data *matrix = (Data *)malloc(rows * columns * sizeof(Data));
	if (!matrix || !temp)
		return ERR_MAKE_MATRIX;
	else
	{
		(*mtrx) = temp;
		(*mtrx)->matrix = matrix;
		(*mtrx)->columns = columns;
		(*mtrx)->rows = rows;
		return SUCCESS1;
	}
}

typedef struct
{
	Matrix m;
	double determinant;
} entry;

static int is_equal(cvoidp_t k, cvoidp_t l)
{
	entry *x = (entry *)k;
	entry *y = (entry *)l;
	Matrix a = (Matrix)x->m;
	Matrix b = (Matrix)y->m; // Im lazy
	if (a->columns != b->columns || a->rows != b->rows) // must be of same dimensions to be equal
		return -1;										// not equal
	for (int col = 0; col < a->columns; col++)
	{
		for (int row = 0; row < a->rows; row++)
		{
			if (a->matrix[INDEX(col, row, a->columns)] != b->matrix[INDEX(col, row, b->columns)])
				return -1; // not equal
		}
	}
	return 0; // equal
}

size_t myhash(cvoidp_t ent, const hashtable *ht)
{
	entry *e = (entry *)ent;
	// use generic hash fn
	int sizeBytes = e->m->columns * e->m->rows * sizeof(double);
	HASH sh = hash(e->m->matrix, sizeBytes);
	HASH f = sh % ht_size(ht);
	return f;
}

// Equation to calculate initial size of the hashtable:
// https://www.wolframalpha.com/input?i=exponential+fit+%7B3%2C4%7D%2C+%7B4%2C17%7D%2C+%7B5%2C56%7D%2C+%7B6%2C157%7D%2C+%7B7%2C284%7D%2C+%7B8%2C961%7D%2C+%7B9%2C2224%7D%2C+%7B10%2C5021%7D%2C+%7B11%2C11144%7D%2C+%7B12%2C24433%7D%2C+%7B13%2C53080%7D%2C+%7B14%2C114493%7D

// Eulers number; e constant
#define E 2.718281828459045091

//#define EXP_CONSTANT 0.773143
#define EXP_CONSTANT 0.7777777 // I use this constant instead so that the size grows a little bit faster than the amount of calls
#define COEFFICIENT_CONSTANT 2.28164

static size_t size(size_t n)
{
	long double exponent = (long double)n * EXP_CONSTANT;
	return (size_t)(COEFFICIENT_CONSTANT * powl(E, exponent));
}

static void print(hashtable* ht)
{
    for (int i = 0; i < ht_size(ht); i++)
    {
        if (ht->entries[i] != UNUSED)
        {
            entry* e = (entry*)ht->entries[i];
            printf("index %d: USED\n", i);
        }
        else
            printf("index %d: UNUSED\n", i);
    }
    printf("\n");
}

// if the matrix is smaller or equal to 4x4 then the naive version should be called (no memoization)
// otherwise this (memoized) version should be called.
ErrorCode1 getDeterminant(Matrix *mtrx, Data *pdet)
{
	if (mtrx == NULL)
		return ERR_DET_MATRIX_IS_NULL;
	if ((*mtrx)->columns != (*mtrx)->rows)
		return ERR_NON_SQUARE_MATRIX;

	RST; // reset counter
	hashtable ht;
	// calculate initial hashtable size to optimize memory allocation
	size_t n = (size_t)(*mtrx)->rows; // (cols == rows)
	size_t htablesize = size(n);
	if (htablesize % 2 == 0)
		htablesize += 1;
	assert(ht_init(&ht, htablesize, &myhash, &is_equal, &freemtrx2) == htablesize);
	*pdet = determinantDivideAndConquer(mtrx, &ht);
	//printf("initial hashtable size: %lld\n", htablesize);
	entry *ent, *ent2;
	Matrix m;
	//print(&ht);
	for (int i = 0; i < ht_size(&ht); i++)
	{
		if (ht.entries[i] != UNUSED)
		{
			ent = (entry *)ht.entries[i];
			m = ent->m;
			//printMatrix(m);
		}
	}
	for (int i = 0; i < ht_size(&ht); i++)
	{
		if (ht.entries[i] != UNUSED)
		{
			ent = (entry *)ht.entries[i];
			m = ent->m;
			//printMatrix(m);
			ent2 = ht_delete(&ht, ent);
			assert(ent == ent2);
			freeMatrix(&m);
			free(ent);
			//m = NULL; its done in freeMatrix
			//print(&ht);
		}
	}
	ht_free(&ht);
	return SUCCESS1;
}

static Data determinantDivideAndConquer(Matrix *mtrx, hashtable *ht)
{
	INC; // increment counter
	// temporary entry to check if we already calculated this matrix
	entry *e = (entry *)malloc(sizeof(entry));
	if (e == NULL)
		exit(-200);
	e->m = *mtrx;
	entry *res;
	if ((res = (entry *)ht_lookup(ht, e)) != NULL)
	{
		freeMatrix(mtrx);
		free(e);
		return res->determinant;
	}
	free(e);
	int factorRowNumber = 0;
	Data result = 0;
	Data factor = 0;
	// Loop through the top row of the matrix. See Chapter "Determinants" in "Grundl�ggande linj�r algebra" by Hillevi Gavel.
	for (int factorColumnNumber = 0; factorColumnNumber < (*mtrx)->columns; factorColumnNumber++)
	{ // iterate through the top column/row. See method 3.1 Chapter "Determinants" in "Grundl�ggande linj�r algebra" by Hillevi Gavel.
		factor = (*mtrx)->matrix[INDEX(factorColumnNumber, factorRowNumber, (*mtrx)->columns)];
		if (factor == 0)
		{
			return 0; // zero factor law
		}
		int evenOrOddColumn = factorColumnNumber; // odd columns are negative. See Chapter "Determinants" in "Grundl�ggande linj�r algebra" by Hillevi Gavel. (Why even or odd: page 101).
		if ((*mtrx)->rows == 2 && (*mtrx)->columns == 2 && factorColumnNumber == 1 && factorRowNumber == 0)
		{
			// we dont want to iterate through the 2x2 matrix. Its the base case.
			// is this needed?
			return 0;
		}
		if ((*mtrx)->rows == 2 && (*mtrx)->columns == 2)
		{ 	// 2x2 matrix is the base case. See Chapter "Determinants" in "Grundl�ggande linj�r algebra" by Hillevi Gavel (page 95)
			// cross multiplication, kind of. Base case. See Chapter "Determinants" in "Grundl�ggande linj�r algebra" by Hillevi Gavel.
			Data positiveValue = (*mtrx)->matrix[INDEX(0, 0, (*mtrx)->columns)] * (*mtrx)->matrix[INDEX(1, 1, (*mtrx)->columns)];
			Data negativeValue = (*mtrx)->matrix[INDEX(1, 0, (*mtrx)->columns)] * (*mtrx)->matrix[INDEX(0, 1, (*mtrx)->columns)];
			// printf("\n(%d * %d - %d * %d)", (*mtrx)->matrix[INDEX(0, 0, (*mtrx)->columns)], (*mtrx)->matrix[INDEX(1, 1, (*mtrx)->columns)], (*mtrx)->matrix[INDEX(1, 0, (*mtrx)->columns)], (*mtrx)->matrix[INDEX(0, 1, (*mtrx)->columns)]);
			return positiveValue - negativeValue;
		}
		// Divide and Conquer. Get the values that will be used to make a new smaller matrix.
		List list = createEmptyList();
		// sdarray a = sda_create_empty();
		for (int row = 0; row < (*mtrx)->rows; row++)
		{
			for (int col = 0; col < (*mtrx)->columns; col++)
			{
				if (col != factorColumnNumber && row != factorRowNumber)
				{
					addLast(&list, (*mtrx)->matrix[INDEX(col, row, (*mtrx)->columns)]);
				}
			}
		}
		// calc size of the new matrix
		int nrOfNodes = numberOfNodesInList(list);
		int rows = sqrt(nrOfNodes); // square matrix so: rows == columns ---> rows * rows || columns * columns || rows * columns = size: sqrt(size) = columns = rows.
		int columns = rows;
		Matrix smallerMatrix = NULL;
		errorHandler1(makeMatrix(rows, columns, &smallerMatrix));
		errorHandler1(initializeMatrixFromList(smallerMatrix, list)); // list made with the double for loop
		clearList(&list);
		// printMatrix(smallerMatrix); // debug print
		// Recursive call to do the same with the new smaller matrix. Divide and Conquer.
		if (evenOrOddColumn % 2 == 0)
		{																// is the column number even or odd?
			Data ret = determinantDivideAndConquer(&smallerMatrix, ht); // determinant of smallerMatrix

			if (smallerMatrix != NULL)
			{
				e = (entry *)malloc(sizeof(entry));
				if (e == NULL)
					exit(-300);
				e->determinant = ret;
				e->m = smallerMatrix;
				ht_insert(ht, e);
			}
			// instead of freeing it here, I use memoization to speed up calculations and free them from the hashtable later
			// freeMatrix(&smallerMatrix); // The function is not destructive to the original matrix. Only smallerMatrix is freed i.e. the matrices made in this function.
			result += factor * ret;
		}
		else
		{
			Data ret = determinantDivideAndConquer(&smallerMatrix, ht); // determinant of smallerMatrix

			if (smallerMatrix != NULL)
			{
				e = (entry *)malloc(sizeof(entry));
				if (e == NULL)
					exit(-300);
				e->determinant = ret;
				e->m = smallerMatrix;
				ht_insert(ht, e);
			}
			// instead of freeing it here, I use memoization to speed up calculations and free them from the hashtable later
			// freeMatrix(&smallerMatrix); // The function is not destructive to the original matrix. Only smallerMatrix is freed i.e. the matrices made in this function.
			result += -1 * factor * ret; // odd columns are negative. See Chapter "Determinants" in "Grundl�ggande linj�r algebra" by Hillevi Gavel. (Why even or odd: page 101).
		}
	}
	return result;
}

// sarrus beginning test (base case)
// if ((*mtrx)->rows == 3 && (*mtrx)->columns == 3) { //2x2 matrix is the base case. See Chapter "Determinants" in "Grundl�ggande linj�r algebra" by Hillevi Gavel (page 95)
//			//cross multiplication, kind of. Base case. See Chapter "Determinants" in "Grundl�ggande linj�r algebra" by Hillevi Gavel.
//	int positiveValue = 0;
//	for (int i = 0; i < 3; i++) {
//		positiveValue += (*mtrx)->matrix[INDEX(0, (i % 3), (*mtrx)->columns)] * (*mtrx)->matrix[INDEX(1, ((i + 1) % 3), (*mtrx)->columns)] * (*mtrx)->matrix[INDEX(2, ((i + 2) % 3), (*mtrx)->columns)];
//	}
//
//	int positiveValue = (*mtrx)->matrix[INDEX(0, 0, (*mtrx)->columns)] * (*mtrx)->matrix[INDEX(1, 1, (*mtrx)->columns)];
//	int negativeValue = (*mtrx)->matrix[INDEX(1, 0, (*mtrx)->columns)] * (*mtrx)->matrix[INDEX(0, 1, (*mtrx)->columns)];
//	printf("\n(%d * %d - %d * %d", (*mtrx)->matrix[INDEX(0, 0, (*mtrx)->columns)], (*mtrx)->matrix[INDEX(1, 1, (*mtrx)->columns)], (*mtrx)->matrix[INDEX(1, 0, (*mtrx)->columns)], (*mtrx)->matrix[INDEX(0, 1, (*mtrx)->columns)]);
//	return positiveValue - negativeValue;
// }

ErrorCode1 getCofactorMatrix(Matrix *mtrx, Matrix *resultMatrix)
{
	if (mtrx == NULL || resultMatrix == NULL)
		return ERR_DET_MATRIX_IS_NULL;
	if ((*mtrx)->columns != (*mtrx)->rows || (*resultMatrix)->columns != (*mtrx)->rows)
		return ERR_NON_SQUARE_MATRIX;

	Data result = 0;
	for (int factorColumnNumber = 0; factorColumnNumber < (*mtrx)->columns; factorColumnNumber++)
	{
		for (int factorRowNumber = 0; factorRowNumber < (*mtrx)->rows; factorRowNumber++)
		{
			// result += determinantDivideAndConquer(mtrx);
			// Divide and Conquer. Get the values that will be used to make a new smaller matrix.
			List list = createEmptyList();
			for (int row = 0; row < (*mtrx)->rows; row++)
			{
				for (int col = 0; col < (*mtrx)->columns; col++)
				{
					if (col != factorColumnNumber && row != factorRowNumber)
						addLast(&list, (*mtrx)->matrix[INDEX(col, row, (*mtrx)->columns)]);
				}
			}
			// calc size of the new matrix
			int nrOfNodes = numberOfNodesInList(list);
			int rows = sqrt(nrOfNodes); // square matrix so: rows == columns ---> rows * rows || columns * columns || rows * columns = size: sqrt(size) = columns = rows.
			int columns = rows;
			Matrix smallerMatrix = NULL;
			errorHandler1(makeMatrix(rows, columns, &smallerMatrix));
			errorHandler1(initializeMatrixFromList(smallerMatrix, list)); // list made with the double for loop
			clearList(&list);
			// printMatrix(smallerMatrix); // debug print
			//  Why odd = negative and even = positive: see chapter 3 (3.4.3) Cramers rule in the book.
			if (((factorColumnNumber + 1) + (factorRowNumber + 1)) % 2 == 0)
			{ // is the column number even or odd?
				// result = determinantDivideAndConquer(&smallerMatrix);
				getDeterminant(&smallerMatrix, &result);
				(*resultMatrix)->matrix[INDEX(factorColumnNumber, factorRowNumber, (*resultMatrix)->columns)] = result;
				// printf("\n\n%d\n\n", result);
				result = 0;
				freeMatrix(&smallerMatrix); // It works!!! The error was the INDEX() macro. It was still using the macro used for tests: COL.
			}
			else
			{
				// odd columns are negative. See Chapter "Determinants" in "Grundl�ggande linj�r algebra" by Hillevi Gavel. (Why even or odd: page 101).
				// result = -1 * determinantDivideAndConquer(&smallerMatrix);
				getDeterminant(&smallerMatrix, &result);
				result *= -1;
				(*resultMatrix)->matrix[INDEX(factorColumnNumber, factorRowNumber, (*resultMatrix)->columns)] = result;
				// printf("\n\n%d\n\n", result);
				result = 0;
				freeMatrix(&smallerMatrix); // It works!!! The function is not destructive to the original matrix. Only smallerMatrix is freed i.e. the matrices made in this function.
			}
		}
	}
	return SUCCESS1;
}

ErrorCode1 getMinors(Matrix *mtrx, Matrix *resultMatrix)
{
	if (mtrx == NULL || resultMatrix == NULL)
		return ERR_DET_MATRIX_IS_NULL;
	if ((*mtrx)->columns != (*mtrx)->rows || (*resultMatrix)->columns != (*mtrx)->rows)
		return ERR_NON_SQUARE_MATRIX;

	Data result = 0;
	int factorRowNumber = 0;
	for (int factorColumnNumber = 0; factorColumnNumber < (*mtrx)->columns; factorColumnNumber++)
	{
		// Divide and Conquer. Get the values that will be used to make a new smaller matrix.
		List list = createEmptyList();
		for (int row = 0; row < (*mtrx)->rows; row++)
		{
			for (int col = 0; col < (*mtrx)->columns; col++)
			{
				if (col != factorColumnNumber && row != factorRowNumber)
					addLast(&list, (*mtrx)->matrix[INDEX(col, row, (*mtrx)->columns)]);
			}
		}
		// calc size of the new matrix
		int nrOfNodes = numberOfNodesInList(list);
		int rows = sqrt(nrOfNodes); // square matrix so: rows == columns ---> rows * rows || columns * columns || rows * columns = size: sqrt(size) = columns = rows.
		int columns = rows;
		Matrix smallerMatrix = NULL;
		errorHandler1(makeMatrix(rows, columns, &smallerMatrix));
		errorHandler1(initializeMatrixFromList(smallerMatrix, list)); // list made with the double for loop
		clearList(&list);
		// printMatrix(smallerMatrix); // debug print
		//  Why odd = negative and even = positive: see chapter 3 (3.4.3) Cramers rule in the book.
		if (((factorColumnNumber + 1) + (factorRowNumber + 1)) % 2 == 0)
		{ // is the column number even or odd?
			// result = determinantDivideAndConquer(&smallerMatrix);
			getDeterminant(&smallerMatrix, &result);
			(*resultMatrix)->matrix[INDEX(factorColumnNumber, factorRowNumber, (*resultMatrix)->columns)] = result;
			// printf("\n\n%d\n\n", result);
			result = 0;
			freeMatrix(&smallerMatrix); // It works!!! The error was the INDEX() macro. It was still using the macro used for tests: COL.
		}
		else
		{
			// odd columns are negative. See Chapter "Determinants" in "Grundl�ggande linj�r algebra" by Hillevi Gavel. (Why even or odd: page 101).
			// result = -1 * determinantDivideAndConquer(&smallerMatrix);
			getDeterminant(&smallerMatrix, &result);
			result *= -1;
			(*resultMatrix)->matrix[INDEX(factorColumnNumber, factorRowNumber, (*resultMatrix)->columns)] = result;
			// printf("\n\n%d\n\n", result);
			result = 0;
			freeMatrix(&smallerMatrix); // It works!!! The function is not destructive to the original matrix. Only smallerMatrix is freed i.e. the matrices made in this function.
		}
	}
	return SUCCESS1;
}

ErrorCode1 transposeMatrix(Matrix *mtrx)
{
	if (mtrx == NULL)
		return ERR_DET_MATRIX_IS_NULL;

	Matrix clone;
	Matrix transposedMatrix;

	// clone of original matrix
	errorHandler1(makeMatrix((*mtrx)->rows, (*mtrx)->columns, &clone));
	copyto(*mtrx, clone);

	// transpose matrix so rows become columns and columns become rows:
	//  therefore send in source matrix columns as rows and source matrix rows as columns for the new matrix. (has effect only on non-square matrices)
	errorHandler1(makeMatrix((*mtrx)->columns, (*mtrx)->rows, &transposedMatrix));

	// transpose with the clones' values
	for (int row = 0; row < (*mtrx)->rows; row++)
	{
		for (int col = 0; col < (*mtrx)->columns; col++)
		{
			transposedMatrix->matrix[INDEX(row, col, (*mtrx)->columns)] = (*mtrx)->matrix[INDEX(col, row, (*mtrx)->columns)];
		}
	}
	// printMatrix(transposedMatrix); //test
	freeMatrix(mtrx);
	errorHandler1(makeMatrix(transposedMatrix->rows, transposedMatrix->columns, mtrx));
	errorHandler1(copyto(transposedMatrix, (*mtrx)));

	// free locals
	freeMatrix(&transposedMatrix);
	transposedMatrix = NULL;
	freeMatrix(&clone);
	clone = NULL;
	return SUCCESS1;
}

ErrorCode1 getAdjugateMatrix(Matrix *mtrx)
{
	if (mtrx == NULL)
		return ERR_DET_MATRIX_IS_NULL;

	Matrix adjugateMatrix;

	errorHandler1(makeMatrix((*mtrx)->rows, (*mtrx)->columns, &adjugateMatrix));

	// get Cofactor matrix
	errorHandler1(getCofactorMatrix(mtrx, &adjugateMatrix));
	// transposed cofactor matrix == adjugate matrix
	errorHandler1(transposeMatrix(&adjugateMatrix));

	// copy back to original
	errorHandler1(copyto(adjugateMatrix, (*mtrx)));

	// free locals
	freeMatrix(&adjugateMatrix);
	adjugateMatrix = NULL;
	return SUCCESS1;
}

ErrorCode1 multiplyMatrixWithScalarValue(Matrix *mtrx, Data scalar)
{
	if (mtrx == NULL)
		return ERR_DET_MATRIX_IS_NULL;

	for (int row = 0; row < (*mtrx)->rows; row++)
	{
		for (int col = 0; col < (*mtrx)->columns; col++)
		{
			(*mtrx)->matrix[INDEX(col, row, (*mtrx)->columns)] = (*mtrx)->matrix[INDEX(col, row, (*mtrx)->columns)] * scalar;
		}
	}
	return SUCCESS1;
}

ErrorCode1 getInverseMatrix(Matrix *mtrx)
{
	Data determinant = 0;
	errorHandler1(getDeterminant(mtrx, &determinant));
	errorHandler1(getAdjugateMatrix(mtrx));

	if (determinant == 0)
		return ERR_INV_MTRX_DETERMINANT_DIV0;

	errorHandler1(multiplyMatrixWithScalarValue(mtrx, (1 / determinant)));
	return SUCCESS1;
}

Data determinantDivideAndConquerSlow(Matrix *mtrx);

ErrorCode1 getDeterminantSlow(Matrix *mtrx, Data *pdet)
{
	RST; // reset counter
	if (mtrx == NULL)
	{
		return ERR_DET_MATRIX_IS_NULL;
	}
	if ((*mtrx)->columns != (*mtrx)->rows)
	{
		return ERR_NON_SQUARE_MATRIX;
	}
	*pdet = determinantDivideAndConquerSlow(mtrx);
	return SUCCESS1;
}

Data determinantDivideAndConquerSlow(Matrix *mtrx)
{
	INC; // increment counter
	int factorRowNumber = 0;
	Data result = 0;
	Data factor = 0;
	// Loop through the top row of the matrix. See Chapter "Determinants" in "Grundl�ggande linj�r algebra" by Hillevi Gavel.
	for (int factorColumnNumber = 0; factorColumnNumber < (*mtrx)->columns; factorColumnNumber++)
	{ // iterate through the top column/row. See method 3.1 Chapter "Determinants" in "Grundl�ggande linj�r algebra" by Hillevi Gavel.
		factor = (*mtrx)->matrix[INDEX(factorColumnNumber, factorRowNumber, (*mtrx)->columns)];
		int evenOrOddColumn = factorColumnNumber; // odd columns are negative. See Chapter "Determinants" in "Grundl�ggande linj�r algebra" by Hillevi Gavel. (Why even or odd: page 101).
		if ((*mtrx)->rows == 2 && (*mtrx)->columns == 2 && factorColumnNumber == 1 && factorRowNumber == 0)
		{
			// we dont want to iterate through the 2x2 matrix. Its the base case.
			return 0;
		}
		if ((*mtrx)->rows == 2 && (*mtrx)->columns == 2)
		{ // 2x2 matrix is the base case. See Chapter "Determinants" in "Grundl�ggande linj�r algebra" by Hillevi Gavel (page 95)
			// cross multiplication, kind of. Base case. See Chapter "Determinants" in "Grundl�ggande linj�r algebra" by Hillevi Gavel.
			Data positiveValue = (*mtrx)->matrix[INDEX(0, 0, (*mtrx)->columns)] * (*mtrx)->matrix[INDEX(1, 1, (*mtrx)->columns)];
			Data negativeValue = (*mtrx)->matrix[INDEX(1, 0, (*mtrx)->columns)] * (*mtrx)->matrix[INDEX(0, 1, (*mtrx)->columns)];
			// printf("\n(%d * %d - %d * %d)", (*mtrx)->matrix[INDEX(0, 0, (*mtrx)->columns)], (*mtrx)->matrix[INDEX(1, 1, (*mtrx)->columns)], (*mtrx)->matrix[INDEX(1, 0, (*mtrx)->columns)], (*mtrx)->matrix[INDEX(0, 1, (*mtrx)->columns)]);
			return positiveValue - negativeValue;
		}
		// Divide and Conquer. Get the values that will be used to make a new smaller matrix.
		List list = createEmptyList();
		for (int row = 0; row < (*mtrx)->rows; row++)
		{
			for (int col = 0; col < (*mtrx)->columns; col++)
			{
				if (col != factorColumnNumber && row != factorRowNumber)
				{
					addLast(&list, (*mtrx)->matrix[INDEX(col, row, (*mtrx)->columns)]);
				}
			}
		}
		// calc size of the new matrix
		int nrOfNodes = numberOfNodesInList(list);
		int rows = sqrt(nrOfNodes); // square matrix so: rows == columns ---> rows * rows || columns * columns || rows * columns = size: sqrt(size) = columns = rows.
		int columns = rows;
		Matrix smallerMatrix = NULL;
		errorHandler1(makeMatrix(rows, columns, &smallerMatrix));
		errorHandler1(initializeMatrixFromList(smallerMatrix, list)); // list made with the double for loop
		clearList(&list);
		// printMatrix(smallerMatrix); // debug print
		// Recursive call to do the same with the new smaller matrix. Divide and Conquer.
		if (evenOrOddColumn % 2 == 0)
		{ // is the column number even or odd?
			result += factor * determinantDivideAndConquerSlow(&smallerMatrix);
			// printf(" * %d += %d\n", factor, result);

			freeMatrix(&smallerMatrix); // It works!!! The error was the INDEX() macro. It was still using the macro used for tests: COL.
		}
		else
		{
			result += -1 * factor * determinantDivideAndConquerSlow(&smallerMatrix); // odd columns are negative. See Chapter "Determinants" in "Grundl�ggande linj�r algebra" by Hillevi Gavel. (Why even or odd: page 101).
			// printf(" * %d += %d\n", -1 * factor, result);

			freeMatrix(&smallerMatrix); // It works!!! The function is not destructive to the original matrix. Only smallerMatrix is freed i.e. the matrices made in this function.
		}
	}
	return result;
}