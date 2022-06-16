#include "matrix.h"
#include "test.h"

//testing macros
////////////////////////////////////////////////////////////////////////////////////////
#define SIZE 3
#define COL SIZE
#define ROW SIZE //square matrix

#define nsqm_COL 5
#define nsqm_ROW 7 //non-square matrix
////////////////////////////////////////////////////////////////////////////////////////


static void testMtrx(Matrix mtrx, Matrix mtrxCmp,  List list) {
	if (list && !mtrxCmp) {
		List templist = list;
		for (int row = 0; row < mtrx->rows; row++) {
			for (int col = 0; col < mtrx->columns; col++) {
				assert(mtrx->matrix[INDEX(col, row, mtrx->columns)] == templist->data);
				templist = templist->next;
			}
		}
	}
	else if (mtrxCmp && !list) {
		for (int row = 0; row < mtrx->rows; row++) {
			for (int col = 0; col < mtrx->columns; col++) {
				assert(mtrx->matrix[INDEX(col, row, mtrx->columns)] == mtrxCmp->matrix[INDEX(col, row, mtrxCmp->columns)]);
			}
		}
	}
	else {
		errorHandler1(ERR_TEST_FUNC_INVALID_ARGUMENT);
	}
}


static void print(const char* string) {
	printf("%*s", -40, string);
}

void matrixTestFunction() {
	srand(time(0)); //init random numbers rand()

	//initial tests
	printf("Matrix test function\n");
	printf("-------------------------------------------\n");
	print("Matrix memory alloc: ");
	Matrix gmTestInit;
	errorHandler1(makeMatrix(ROW, COL, &gmTestInit));
	printf("OK\n");

	print("Matrix elements initialization: ");
	errorHandler1(matrixTestInitialization(gmTestInit));
	printf("OK\n");
	print("Free matrix allocated mem: ");
	freeMatrix(&gmTestInit);
	printf("OK\n");



	//initialize from list test
	print("Initialize from list: ");
	//set up
	List list = createEmptyList();
	addLast(&list, 1);
	addLast(&list, 5);
	addLast(&list, 8);
	addLast(&list, 3);
	addLast(&list, 4);
	addLast(&list, 2);
	addLast(&list, 10);
	addLast(&list, 6);
	addLast(&list, 7);

	Matrix gmListInit;
	errorHandler1(makeMatrix(ROW, COL, &gmListInit));

	errorHandler1(initializeMatrixFromList(gmListInit, list));

	//test results
	testMtrx(gmListInit, NULL, list);

	//no clean up, gmListInit and list used elsewhere
	printf("OK\n");



	//clone test
	print("Clone matrix: ");
	//set up
	Matrix gmCloneFrom;
	Matrix gmCloneTo;
	errorHandler1(makeMatrix(ROW, COL, &gmCloneFrom));
	errorHandler1(makeMatrix(ROW, COL, &gmCloneTo));
	errorHandler1(initializeMatrixFromList(gmCloneFrom, list));
	clearList(&list);
	errorHandler1(copyto(gmCloneFrom, gmCloneTo));
	//test results
	testMtrx(gmCloneFrom, gmCloneTo, NULL);
	//clean up
	freeMatrix(&gmCloneFrom);
	freeMatrix(&gmCloneTo);
	printf("OK\n");



	//random numbers init test
	print("Initialize random numbers: ");
	//set up
	Matrix gmRandom;
	errorHandler1(makeMatrix(nsqm_ROW, nsqm_COL, &gmRandom));
	errorHandler1(initializeMatrixRandomNumbers(gmRandom, 10, 0));
	//clean up
	freeMatrix(&gmRandom);
	printf("OK\n");


	//determinant test
	print("Calc determinant: ");
	//set up
	Matrix gmDetTest;
	errorHandler1(makeMatrix(ROW, COL, &gmDetTest));
	errorHandler1(copyto(gmListInit, gmDetTest));
	Data determinant = 0;
	errorHandler1(getDeterminant(&gmDetTest, &determinant));
	//test results
	//determinant of list init is -165
	assert(determinant == -165);
	assert(gmDetTest != NULL);
	//clean up
	freeMatrix(&gmListInit);
	printf("OK\n");

	
	//inverse test
	print("Calc inverse matrix: ");
	//set up
	List list6 = createEmptyList();
	addLast(&list6, 1);
	addLast(&list6, 3);
	addLast(&list6, 3);
	addLast(&list6, 1);
	addLast(&list6, 4);
	addLast(&list6, 3);
	addLast(&list6, 1);
	addLast(&list6, 3);
	addLast(&list6, 4);
	Matrix inverseMatrixTest;
	errorHandler1(makeMatrix(3, 3, &inverseMatrixTest));
	initializeMatrixFromList(inverseMatrixTest, list6);
	getInverseMatrix(&inverseMatrixTest);
	//test results
	List inverseValuesList = createEmptyList();
	addLast(&inverseValuesList, 7);
	addLast(&inverseValuesList, -3);
	addLast(&inverseValuesList, -3);
	addLast(&inverseValuesList, -1);
	addLast(&inverseValuesList, 1);
	addLast(&inverseValuesList, 0);
	addLast(&inverseValuesList, -1);
	addLast(&inverseValuesList, 0);
	addLast(&inverseValuesList, 1);
	testMtrx(inverseMatrixTest, NULL, inverseValuesList);
	//cleanup
	clearList(&inverseValuesList);
	freeMatrix(&inverseMatrixTest);
	printf("OK\n");


	//scalar tests
	print("Multiply matrix with scalar: ");
	//set up
	List list5 = createEmptyList();
	addLast(&list5, 1);
	addLast(&list5, 3);
	addLast(&list5, 4);
	addLast(&list5, 2);
	addLast(&list5, 5);
	addLast(&list5, 1);
	addLast(&list5, 2);
	addLast(&list5, 2);
	addLast(&list5, 1);
	Matrix scalarMultiplicationTest;
	errorHandler1(makeMatrix(3, 3, &scalarMultiplicationTest));
	initializeMatrixFromList(scalarMultiplicationTest, list5);
	multiplyMatrixWithScalarValue(&scalarMultiplicationTest, -1);
	//test results
	List scalarTestList = createEmptyList();
	addLast(&scalarTestList, -1);
	addLast(&scalarTestList, -3);
	addLast(&scalarTestList, -4);
	addLast(&scalarTestList, -2);
	addLast(&scalarTestList, -5);
	addLast(&scalarTestList, -1);
	addLast(&scalarTestList, -2);
	addLast(&scalarTestList, -2);
	addLast(&scalarTestList, -1);
	//values should have changed to those in the list
	testMtrx(scalarMultiplicationTest, NULL, scalarTestList);
	//clean up
	clearList(&scalarTestList);
	freeMatrix(&scalarMultiplicationTest);
	printf("OK\n");


	//adjugate tests
	print("Calc adjugate matrix: ");
	//set up
	List list4 = createEmptyList();
	addLast(&list4, 1);
	addLast(&list4, 0);
	addLast(&list4, 0);
	addLast(&list4, -2);
	addLast(&list4, 0);
	addLast(&list4, 0);
	addLast(&list4, 0);
	addLast(&list4, -2);
	addLast(&list4, 0);
	Matrix adjugateMatrixTest;
	Matrix adjugateMatrixClone;
	errorHandler1(makeMatrix(3, 3, &adjugateMatrixTest));
	errorHandler1(makeMatrix(3, 3, &adjugateMatrixClone));
	initializeMatrixFromList(adjugateMatrixTest, list4);
	copyto(adjugateMatrixTest, adjugateMatrixClone);
	getAdjugateMatrix(&adjugateMatrixTest);
	//test results
	List adjugateTestList = createEmptyList();
	addLast(&adjugateTestList, 0);
	addLast(&adjugateTestList, 0);
	addLast(&adjugateTestList, 0);
	addLast(&adjugateTestList, 0);
	addLast(&adjugateTestList, 0);
	addLast(&adjugateTestList, 0);
	addLast(&adjugateTestList, 4);
	addLast(&adjugateTestList, 2);
	addLast(&adjugateTestList, 0);
	//input to adjugate func should be adjugate
	testMtrx(adjugateMatrixTest, NULL, adjugateTestList);
	//clone matrix shouldnt have been changed
	testMtrx(adjugateMatrixClone, NULL, list4);
	//clean up
	clearList(&adjugateTestList);
	freeMatrix(&adjugateMatrixClone);
	freeMatrix(&adjugateMatrixTest);
	printf("OK\n");


	//transpose tests
	print("Calc transposed matrix: ");
	//set up
	List list3 = createEmptyList();
	addLast(&list3, 1);
	addLast(&list3, 0);
	addLast(&list3, 0);
	addLast(&list3, -2);
	addLast(&list3, 0);
	addLast(&list3, 0);
	addLast(&list3, 0);
	addLast(&list3, -2);
	addLast(&list3, 0);
	Matrix transposeMatrixTest;
	errorHandler1(makeMatrix(3, 3, &transposeMatrixTest));
	initializeMatrixFromList(transposeMatrixTest, list3);
	transposeMatrix(&transposeMatrixTest);
	//test results
	List transposeTestList = createEmptyList();
	addLast(&transposeTestList, 1);
	addLast(&transposeTestList, -2);
	addLast(&transposeTestList, 0);
	addLast(&transposeTestList, 0);
	addLast(&transposeTestList, 0);
	addLast(&transposeTestList, -2);
	addLast(&transposeTestList, 0);
	addLast(&transposeTestList, 0);
	addLast(&transposeTestList, 0);
	//matrix should have been transposed
	testMtrx(transposeMatrixTest, NULL, transposeTestList);
	//clean up
	freeMatrix(&transposeMatrixTest);
	printf("OK\n");



	//print("Big Matrix Calc Adjugate: ");
	//List list2 = createEmptyList();
	//addLast(&list2, 1);
	//addLast(&list2, 0);
	//addLast(&list2, 0);
	//addLast(&list2, -2);
	//addLast(&list2, 0);
	//addLast(&list2, 0);
	//addLast(&list2, 0);
	//addLast(&list2, -2);
	//addLast(&list2, 0);
	//addLast(&list2, -2);
	//addLast(&list2, 0);
	//addLast(&list2, 3);
	//addLast(&list2, 0);
	//addLast(&list2, 4);
	//addLast(&list2, 0);
	//addLast(&list2, -5);
	//addLast(&list2, 0);
	//addLast(&list2, -4);
	//addLast(&list2, 0);
	//addLast(&list2, 1);
	//addLast(&list2, 0);
	//addLast(&list2, -3);
	//addLast(&list2, 0);
	//addLast(&list2, 0);
	//addLast(&list2, 1);

	//Matrix gmAdjTest;
	//Matrix gmAdjResult;
	//errorHandler1(makeMatrix(5, 5, &gmAdjTest));
	//errorHandler1(makeMatrix(5, 5, &gmAdjResult));
	//errorHandler1(initializeMatrixFromList(gmAdjTest, list2));
	////printMatrix(gmAdjTest);
	//errorHandler1(getCofactorMatrix(&gmAdjTest, &gmAdjResult));
	////printMatrix(gmAdjResult);
	//freeMatrix(&gmAdjResult);
	//freeMatrix(&gmAdjTest);
	//printf("OK\n");

	//clearList(&list2);
	//final clean up
	clearList(&list3);
	clearList(&list4);
	clearList(&list5);
	clearList(&list6);
	printf("-------------------------------------------\n");
	printf("Test concluded.\n");
	printf("-------------------------------------------\n");

	
}