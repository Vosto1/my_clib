#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "BSTree.h"

/*Det �r helt till�tet att l�gga till egna hj�lpfunktioner men de befintliga funktionerna f�r inte �ndras*/

/* Statiska hjalpfunktioner anvands av andra funktioner i tradet och ska inte ligga i interfacet (anvandaren behover inte kanna till dessa) */


/* Skapar en tradnod med det givna datat genom att allokera minne for noden. Glom inte att initiera pekarna*/
static struct treeNode* createNode(int data)
{
	struct treeNode* newNode = (struct treeNode*)malloc(sizeof(struct treeNode));
	if (newNode != NULL)
	{
		assert(newNode != NULL);
		newNode->data = data;
		newNode->left = NULL;
		newNode->right = NULL;
		return newNode;
	}
    // Glom inte att testa sa att allokeringen lyckades
	return NULL; // Ersatt med ratt returvarde
}

static void writeToArr(BSTree tree, int* arr, int* index)
{
	if (tree != NULL)
	{
		if (tree->left != NULL)
			writeToArr(tree->left, arr, index);
		arr[(*index)] = tree->data;
		(*index) += 1;
		if (tree->right != NULL)
			writeToArr(tree->right, arr, index);
	}
}

/* Returnerar en dynamiskt allokerad array som innehaller tradets data sorterat */
static int* writeSortedToArray(const BSTree tree)
{
    /* Skapa en dynamisk array men ratt storlek
     
       Skriv datat fr�n tradet sorterat till arrayen (minsta till storsta)
       - till detta kanske du behover en hjalpfunktion */
	int* arr = (int*)malloc(sizeof(int) * numberOfNodes(tree));
	if (arr != NULL)
	{
		int indexToIncrement = 0;
		writeToArr(tree, arr, &indexToIncrement);
		return arr; //Ersatt med korrekt returvarde
	}
	return NULL;
}

 //this is already building tree from sorted array?
static BSTree buildTreeFromArr(int arr[], int start, int end)
{
	if (start > end)
		return NULL;

	int mid = (start + end) / 2;
	struct treeNode* tree = createNode(arr[mid]);

	tree->left = buildTreeFromArr(arr, start, mid - 1);

	tree->right = buildTreeFromArr(arr, mid + 1, end);

	return tree;
}

/* Bygger upp ett sorterat, balanserat trad fran en sorterad array */
static void buildTreeSortedFromArray(BSTree* tree, const int arr[], int size)
{
    /* Bygg rekursivt fran mitten.
       Mittenelementet i en delarray skapar rot i deltradet
       Vanster delarray bygger vanster deltrad
       Hoger delarray bygger hoger deltrad*/
}


/* Implementation av tradet, funktionerna i interfacet */

/* Skapar ett tomt trad - denna funktion ar fardig */
BSTree emptyTree(void)
{
	return NULL;
}

/* Returnerar 1 ifall tradet ar tomt, 0 annars */
int isEmpty(const BSTree tree)
{
	if (tree == NULL)
		return 1;
	else
		return 0;
}

static void addDataSorted(BSTree* tree, BSTree toAdd)
{
	if ((*tree) == NULL)
		(*tree) = toAdd;
	else if (toAdd->data < (*tree)->data)
	{
		if ((*tree)->left == NULL)
		{
			(*tree)->left = toAdd;
			return;
		}
		else
			addDataSorted(&(*tree)->left, toAdd);
	}
	else if (toAdd->data > (*tree)->data)
	{
		if ((*tree)->right == NULL)
		{
			(*tree)->right = toAdd;
		}
		else
			addDataSorted(&(*tree)->right, toAdd);
	}
}
/* Satter in 'data' sorterat i *tree
 Post-condition: data finns i tradet*/
void insertSorted(BSTree* tree, int data)
{
	/*Tank pa att tradet kan vara tomt vid insattning
      Du bestammer sjalv hur dubletter ska hanteras, ska de inte accepteras eller
      ska de laggas till vanster/hoger?.
      Post-condition kan verifieras med hjalp av find(...)*/
	struct treeNode* temp = createNode(data);
	if (temp != NULL)
		addDataSorted(tree, temp);
	//assert(find(*tree, data) == 1); //vet ej om denna är rätt, la till den utan att testa
}

/* Utskriftsfunktioner
   Vid anrop: anvand stdio som andra argument for att skriva ut p� skarmen
   Det racker att ni implementerar LR ordningarna*/
void printPreorder(const BSTree tree, FILE *textfile)
{
	if (tree != NULL)
	{
		fprintf(textfile, "%d  ", tree->data);
		if (tree->left != NULL)
			printPreorder(tree->left, textfile);
		if (tree->right != NULL)
			printPreorder(tree->right, textfile);
	}
}

void printInorder(const BSTree tree, FILE *textfile)
{
	if (tree != NULL)
	{
		if (tree->left != NULL)
			printInorder(tree->left, textfile);
		fprintf(textfile, "%d  ", tree->data);
		if (tree->right != NULL)
			printInorder(tree->right, textfile);
	}
}

void printPostorder(const BSTree tree, FILE *textfile)
{
	if (tree != NULL)
	{
		if (tree->left != NULL)
			printPostorder(tree->left, textfile);
		if (tree->right != NULL)
			printPostorder(tree->right, textfile);
		fprintf(textfile, "%d  ", tree->data);
	}
}

/* Returnerar 1 om 'data' finns i tree, 0 annars */
int find(const BSTree tree, int data)
{
	if (tree != NULL)
	{
		if (tree->data == data)
			return 1;
		else if (data < tree->data)
		{
			return find(tree->left, data);
		}
		else
		{
			return find(tree->right, data);
		}
	}
    // Tank pa att tradet kan vara tomt
	return 0; //Ersatt med korrekt returvarde
}

static int findSmallestDataRight(BSTree* tree)// skicka (*tree)->right
{
	if ((*tree)->left == NULL)
	{
		return (*tree)->data;
	}
	return findSmallestDataRight(&(*tree)->left);
}

//findNodeAndCreatePointerToNodeToBeRemovedAndCopied gamla funktions namnet lol
static BSTree* findNodeAddPtrToRmAndCpy(BSTree* tree, int data)
{
	if (data == (*tree)->data)
		return &(*tree);
	if (data < (*tree)->data)
		return findNodeAddPtrToRmAndCpy(&(*tree)->left, data);
	else
		return findNodeAddPtrToRmAndCpy(&(*tree)->right, data);
}

static void rmWithRightChild(BSTree* tree)
{
	BSTree toRemove = *tree;
	(*tree) = (*tree)->right;
	free(toRemove);
	toRemove = NULL;
}

static void rmWithLeftChild(BSTree* tree)
{
	BSTree toRemove = *tree;
	(*tree) = (*tree)->left;
	free(toRemove);
	toRemove = NULL;
}

/* Tar bort 'data' fran tradet om det finns */
void removeElement(BSTree* tree, int data)
{
	if ((*tree) != NULL)
	{
		if ((*tree)->data == data)
		{
			if ((*tree)->left == NULL && (*tree)->right == NULL)
			{
				free((*tree));
				(*tree) = NULL;
				return;
			}
			else
				if ((*tree)->left == NULL && (*tree)->right != NULL)
				{
					rmWithRightChild(tree);
					return;
				}
				else if((*tree)->right == NULL && (*tree)->left != NULL)
				{
					rmWithLeftChild(tree);
					return;
				}
				else
				{
					int tmpData = findSmallestDataRight(&(*tree)->right);
					BSTree* toRemove = findNodeAddPtrToRmAndCpy(tree, tmpData);
					removeElement(toRemove, tmpData);
					(*tree)->data = tmpData;
					return;
				}
		}
		if ((*tree)->data > data)
			removeElement(&(*tree)->left, data);
		else
			removeElement(&(*tree)->right, data);
	}
	/* Inget data ska/kan tas bort fran ett tomt trad
     Tre fall: Ett lov (inga barn), ett barn (vanster eller hoger), tva barn
     
     Glom inte att frigora noden nar den lankats ur tradet*/
}

/* Returnerar hur manga noder som totalt finns i tradet */
int numberOfNodes(const BSTree tree)
{
	if (tree == NULL)
	{
		return 0;
	}
	return 1 + numberOfNodes(tree->left) + numberOfNodes(tree->right);
}

/* Returnerar hur djupt tradet ar */
int depth(const BSTree tree)
{
	int x = 1, y = 1;
	if (tree == NULL)
		return 0;
	x += depth(tree->left);
	y += depth(tree->right);
	if (x >= y)
		return x;
	else
		return y;
}

/* Returnerar minimidjupet for tradet
   Se math.h for anvandbara funktioner*/
int minDepth(const BSTree tree)
{
	if (tree != NULL)
		return ceil((log(numberOfNodes(tree) + 1) / log(2)));
	return 0;
}

/* Balansera tradet sa att depth(tree) == minDepth(tree) */
void balanceTree(BSTree* tree)
{
	/* Forslag pa algoritm:
	   - overfor tradet till en dynamiskt allokerad array med writeSortedToArray()
	   - tom tradet med freeTree()
	   - bygg upp tradet rekursivt fran arrayen med buildTreeSortedFromArray()
	   - frigor minne for den dynamiskt allokerade arrayen


	   Post-conditions:
	   - tree har lika manga noder som tidigare
	   - djupet for tradet ar samma som minimumdjupet for tradet */
	if ((*tree) != NULL)
	{
		int* sortedArray = writeSortedToArray(*tree);
		if (sortedArray != NULL)
		{
			int treeAndArrSize = numberOfNodes(*tree);
			BSTree tmpTree = buildTreeFromArr(sortedArray, 0, treeAndArrSize - 1);
			if (numberOfNodes(*tree) == numberOfNodes(tmpTree) && depth(tmpTree) == minDepth(*tree))
			{
				free(*tree);
				*tree = tmpTree;
				free(sortedArray);
			}
			else 
			{
				free(sortedArray);
			}
		}
	}
}

/* Tom tradet och frigor minnet for de olika noderna */
void freeTree(BSTree* tree)
{
	if ((*tree) != NULL)
	{
		freeTree(&(*tree)->left);
		freeTree(&(*tree)->right);
		free((*tree));
		(*tree) = NULL;
	}
	
	// Post-condition: tradet ar tomt
}