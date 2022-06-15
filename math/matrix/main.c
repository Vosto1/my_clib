#include "matrix.h"

int main(void) {
	srand(time(0)); //seed random numbers, rand()
	/* for (int i = 0; i < 12; i++) {
		errorHandler(i);
	} */
	
	//some features of the code implemented and usable here
	_menu();
	


	//matrix implementation test (not done)
	matrixTestFunction();


	return 0;

	//"manual" input and calculation
	List list = createEmptyList();
	addLast(&list, 2);
	addLast(&list, 4);
	addLast(&list, 5);
	addLast(&list, 1);
	addLast(&list, 7);
	addLast(&list, 2);
	addLast(&list, 8);
	addLast(&list, 5);
	addLast(&list, 3);

	Data determinant;

	Matrix matrix;
	errorHandler(makeMatrix(3, 3, &matrix));
	errorHandler(initializeMatrixFromList(matrix, list));


	//compare determinant of matrix and its transposed determinant
	errorHandler(getDeterminant(&matrix, &determinant));

	printf(FORMAT_STRING, determinant);

	errorHandler(transposeMatrix(&matrix));
	errorHandler(getDeterminant(&matrix, &determinant));

	printf(FORMAT_STRING, determinant);
	
	//compare determinant and its inverse determinant
	errorHandler(getDeterminant(&matrix, &determinant));

	printf(FORMAT_STRING, determinant);

	getInverseMatrix(&matrix);

	//printMatrix(matrix);

	errorHandler(getDeterminant(&matrix, &determinant));

	printf(FORMAT_STRING, determinant);

	freeMatrix(&matrix);

	
	return 0;
}
