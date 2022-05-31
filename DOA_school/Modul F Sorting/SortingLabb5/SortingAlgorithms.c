#define _CRT_SECURE_NO_WARNINGS
#include "SortingAlgorithms.h"
#include "Statistics.h"
#include "Array.h"
#include <assert.h>
#include <string.h>

int isImplemented(SortingAlgorithm algorithm)
{
	switch (algorithm)
	{
//      case BUBBLE_SORT:
      case INSERTION_SORT:
//      case SELECTION_SORT:
//      case QUICK_SORT:
//      case MERGE_SORT:
            return 1;
        default:
            return 0;
	}
}

/*Antalet byten i en och samma algoritm kan vara olika beroende på implementationen. Ibland ligger datat redan på rätt plats och då kan man välja att testa det och inte göra ett byte (vilket ger extra jämförelse) eller så kan man ändå göra ett byte (med sig själv). Följer man de algoritmer som vi gått igenom på föreläsningarna exakt så gör man en swap även på ett element som ligger på rätt plats
 
    När du analyserar det data som genereras (result.txt) så behöver du ha detta i åtanke */

/******************************************************************************************/
/* Era algoritmer har: */

static void bubbleSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
	int* pek1, * pek2, * stop;
	pek1 = arrayToSort;
	pek2 = arrayToSort + 1;
	stop = arrayToSort + (size - 1);
	int swapped, i, nrOfelementsInRightPlaceCounter = 0;
	do {
		swapped = 0;
		while (notEqualTo(pek1, stop, statistics))
		{
			if (greaterThan(*pek1, *pek2, statistics))
			{
				swapElements(pek1, pek2, statistics);
				swapped = 1;
			}
			pek1++;
			pek2++;
		}
		pek1 = arrayToSort;
		pek2 = arrayToSort + 1;
		stop -= 1;
	} while (equalTo(swapped, 1, statistics));
}

static void insertionSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
	int tmp, i, j;
	for (i = 0; i < size; i++)
	{
		tmp = arrayToSort[i];
		for (j = i; j > 0; j--)
		{
			if (tmp < arrayToSort[j - 1])
			{
				int swap = arrayToSort[j];
				arrayToSort[j] = arrayToSort[j - 1];
				arrayToSort[j - 1] = swap;
			}
			else
			{
				break;
			}
		}
	}
}

static void selectionSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
}

static void merge(int arr[], int left, int right, int mid, Statistics* statistics)
{
	int lenLeft = mid - left + 1;
	int lenRight = right - mid;

	int* arrLeft = NULL;
	int* arrRight = NULL;

	arrLeft = (int)malloc(sizeof(int) * lenLeft);
	assert(arrLeft != NULL);
	
	arrRight = (int)malloc(sizeof(int) * lenRight);
	assert(arrRight != NULL);
	
	int i;
	
	for (i = 0; i < lenLeft && arrLeft != NULL; i++)
	{
		arrLeft[i] = arr[i + left];
	}
	for (i = 0; i < lenRight && arrRight != NULL; i++)
	{
		arrRight[i] = arr[i + mid + 1];
	}

	i = left;
	int i1 = 0, i2 = 0;

	while (i1 < lenLeft && i2 < lenRight)
	{
		if (arrLeft[i1] < arrRight[i2])
		{
			arr[i] = arrLeft[i1];
			i++;
			i1++;
		}
		else
		{
			arr[i] = arrRight[i2];
			i++;
			i2++;
		}
	}

	while (i1 < lenLeft)
	{
		arr[i] = arrLeft[i1];
		i++;
		i1++;
	}
	while (i2 < lenRight)
	{
		arr[i] = arrRight[i2];
		i++;
		i2++;
	}
}

static void divide(int arr[], int left, int right, Statistics* statistics)
{
	
	if (left >= right)
		return;
	int mid = left + (right - left) / 2;
	divide(arr, left, mid, statistics);
	divide(arr, mid + 1, right, statistics);
	merge(arr, left, right, mid, statistics);
}

static void mergeSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
	for (int i = 0; i < size; i++)
	{
		printf("%d ", arrayToSort[i]);
	}
	printf("\n");
	divide(arrayToSort, 0, size - 1, statistics);
	for (int i = 0; i < size; i++)
	{
		printf("%d ", arrayToSort[i]);
	}
	printf("\n");
	printf("\n");
	printf("\n");
}

static void quickSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
}

/******************************************************************************************/


char* getAlgorithmName(SortingAlgorithm algorithm)
{
	/* Ar inte strangen vi allokerar lokal for funktionen?
	   Nej, inte i detta fall. Strangkonstanter ar ett speciallfall i C */
	switch (algorithm)
	{
        case BUBBLE_SORT:	 return "  Bubble sort ";
        case SELECTION_SORT: return "Selection sort";
        case INSERTION_SORT: return "Insertion sort";
        case MERGE_SORT:	 return "  Merge sort  ";
        case QUICK_SORT:	 return "  Quick sort  ";
        default: assert(0 && "Ogiltig algoritm!"); return "";
	}
}

// Sorterar 'arrayToSort' med 'algorithmToUse'. Statistik for antal byten och jamforelser hamnar i *statistics
static void sortArray(ElementType* arrayToSort, unsigned int size, SortingAlgorithm algorithmToUse, Statistics* statistics)
{
	if(statistics != NULL)
		resetStatistics(statistics);

	switch (algorithmToUse)
	{
	case BUBBLE_SORT:	 bubbleSort(arrayToSort, size, statistics); break;
	case SELECTION_SORT: selectionSort(arrayToSort, size, statistics); break;
	case INSERTION_SORT: insertionSort(arrayToSort, size, statistics); break;
	case MERGE_SORT:	 mergeSort(arrayToSort, size, statistics); break;
	case QUICK_SORT:	 quickSort(arrayToSort, size, statistics); break;
	default:
		assert(0 && "Ogiltig algoritm!");
	}
}

// Forbereder arrayer for sortering genom att allokera minne for dem, samt intialisera dem
static void prepareArrays(SortingArray sortingArray[], SortingAlgorithm algorithm, const ElementType* arrays[], const unsigned int sizes[])
{
	assert(isImplemented(algorithm));

	int i;
	int totalArraySize;

	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		totalArraySize = sizeof(ElementType)*sizes[i];
		sortingArray[i].arrayToSort = malloc(totalArraySize);
		memcpy(sortingArray[i].arrayToSort, arrays[i], totalArraySize);

		sortingArray[i].algorithm = algorithm;
		sortingArray[i].arraySize = sizes[i];
		resetStatistics(&sortingArray[i].statistics);
	}
}

// Sorterar arrayerna
static void sortArrays(SortingArray toBeSorted[])
{
	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		SortingArray* current = &toBeSorted[i];
		sortArray(current->arrayToSort, current->arraySize, current->algorithm, &current->statistics);
	
		if (!isSorted(current->arrayToSort, current->arraySize))
		{
			printf("Fel! Algoritmen %s har inte sorterat korrekt!\n", getAlgorithmName(current->algorithm));
			printf("Resultatet ‰r: \n");
			printArray(current->arrayToSort, current->arraySize, stdout);
			assert(0); // Aktiveras alltid
		}
	}
}

void printResult(SortingArray sortedArrays[], FILE* file)
{
	assert(file != NULL);

	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		fprintf(file, "%4d element: ", sortedArrays[i].arraySize);
		printStatistics(&sortedArrays[i].statistics, file);
		fprintf(file, "\n");
	}
	fprintf(file, "\n");
}

void sortAndPrint(SortingArray sortingArray[], SortingAlgorithm algorithm, const ElementType* arrays[], unsigned int sizes[], FILE* file)
{
	assert(file != NULL);

	prepareArrays(sortingArray, algorithm, arrays, sizes);
	sortArrays(sortingArray);
	printResult(sortingArray, file);
}

void freeArray(SortingArray sortingArray[])
{
	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		if (sortingArray[i].arrayToSort != NULL)
			free(sortingArray[i].arrayToSort);
		sortingArray[i].arrayToSort = NULL;
		sortingArray[i].arraySize = 0;
		resetStatistics(&sortingArray[i].statistics);
	}
}
