#include "matrix.h"
#include "menu.h"

#define MAX_STRING 1024

void _menu() {
	if (!strcmp(FORMAT_STRING, INT)) {
		exit(ERR_MUST_BE_DOUBLE);
	}
	Data element;
	Data det;
	char buffer[MAX_STRING];
	char switch_on = 'n';
	Matrix workingMatrix = NULL;
	int columns = 0;
	int rows = 0;
	char binaryChoice = 'n';
	bool run = true;
	while (run) {
		//system("cls");
		printf("------------------------------------------\n");
		printf("c - create matrix\n");
		printf("d - calc determinant\n");
		printf("i - get inverse matrix\n");
		printf("e - exit\n");
		printf("------------------------------------------\n");
		printf("Choice: ");
		fgets(buffer, MAX_STRING, stdin);
		switch_on = buffer[0];
		switch (switch_on)
		{
		case 'c':
			if (workingMatrix != NULL) {
				printf("delete old matrix? (y/n)\n");
				fflush(stdin);

				fgets(buffer, MAX_STRING, stdin);
				binaryChoice = buffer[0];
				
				if (binaryChoice != 'y') {
					break;
				}
				errorHandler1(freeMatrix(&workingMatrix));
			}
			List list = createEmptyList();
			printf("\ncolumns: ");
			scanf("%d", &columns);
			printf("rows: ");
			fflush(stdin);
			scanf("%d", &rows);
			errorHandler1(makeMatrix(rows, columns, &workingMatrix));
			fflush(stdin);
			for (int i = 0; i < columns * rows; i++) {
				printf("\nEnter element on index [%d, %d]: ", (i / columns), i % 3);
				fflush(stdin);
				scanf("%lf", &element);
				addLast(&list, element);
			}

			errorHandler1(initializeMatrixFromList(workingMatrix, list));
			clearList(&list);
			list = createEmptyList();
			printf("Entered matrix:\n");
			printMatrix(workingMatrix);
			fflush(stdin);
			break;
		case 'd':
			det = 0;
			errorHandler1(getDeterminant(&workingMatrix, &det));
			fflush(stdin);
			printMatrix(workingMatrix);
			printf("determinant: %lf\n", det);
			break;
		case 'i':
			printf("Input matrix: \n");
			printMatrix(workingMatrix);
			getInverseMatrix(&workingMatrix);
			fflush(stdin);
			printf("Inverse: \n");
			printMatrix(workingMatrix);
			break;
		case 'e':
			run = false;
			break;
		default:
			//do nothing
			break;
		}
		fflush(stdin);
	}
}