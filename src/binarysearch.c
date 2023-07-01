#include "binarysearch.h"

int binary_search(void* array[], void* element, uint size, int (*compare)(const void* element_to_search_for, const void* candidate_element))
{
    uint end, start, index;
    end = size;
    start = 0;
    int result;
    while (start <= end)
    {
        index = start + (end - start)/2;
        result = (*compare)(element, array[index]);

        switch (result)
        {
            case 0: // elements were equal
                return index;
                break;
            case 1: // element to search for was bigger
                start = index + 1;
                break;
            case -1: // element to seach for was smaller
                end = index - 1;
                break;
            default:
                perror("The compare function should only return 0, -1, or 1.");
                exit(-1);
                return -1;
                break;
        }
    }

    return -1; // not found
}
