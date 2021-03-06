#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#define _countof(x) (sizeof(x) / sizeof(x[0]))

void printArray(size_t count, int *array)
{
	printf("{");
	for(size_t i = 0; i < count; i++)
	{
		if(i != count - 1)
		{
			printf("%d, ", array[i]);
		}
		else
		{
			printf("%d", array[i]);
		}
	}
	printf("}\n");
}

void merge(size_t countA, int *A, size_t countB, int *B)
{
	std::vector<int> temp;
	temp.reserve(countA + countB);
	size_t aIndex = 0;
	size_t bIndex = 0;
	
	for(size_t i = 0; i < temp.capacity(); i++)
	{
		if(aIndex < countA && (bIndex >= countB || A[aIndex] < B[bIndex]))
		{
			temp.push_back(A[aIndex]);
			aIndex++;
		}
		else
		{
			temp.push_back(B[bIndex]);
			bIndex++;
		}
	}
	
	for(size_t i = 0; i < temp.size(); i++)
	{
		A[i] = temp[i];
	}
}

const bool areAnagrams(const char *const strA, const char *const strB)
{
	size_t countA[255] = {};
	size_t countB[255] = {};
	
	size_t i = 0;
	while(strA[i] != '\0' && strB[i] != '\0')
	{
		countA[strA[i] + 1]++;
		countB[strB[i] + 1]++;
		i++;
	}
	
	if(strA[i] == '\0' && strB[i] == '\0')
	{
		for(i = 0; i < 255; i++)
		{
			if(countA[i] != countB[i])
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	
	return true;
}

int anagramEqual(const void *a, const void *b)
{
	const char *const strA = *(reinterpret_cast<const char * const *>(a));
	const char *const strB = *(reinterpret_cast<const char * const *>(b));
	
	if(areAnagrams(strA, strB))
	{
		return 0;
	}
	else
	{
		return strcmp(strA, strB);
	}
}

void sortAnagramsEqual(size_t count,  const char * const *strArray)
{
	qsort(const_cast<const char **>(strArray), count, sizeof(const char *const), anagramEqual);
}

bool rotatedBinarySearch(const int target, size_t count, int *array, size_t *targetIndex)
{
	if(count == 0)
	{
		return false;
	}
	else
	{
		printf("Searching %d ... %d\n", array[0], array[count - 1]);
		size_t mid = (count - 1) / 2;
			
		if(array[mid] < target)
		{
			// try right, if that fails, try left
			if(rotatedBinarySearch(target, count - mid, array + mid, targetIndex))
			{
				*targetIndex = *targetIndex + mid;
				return true;
			}
			else if(rotatedBinarySearch(target, mid, array, targetIndex))
			{
				return true;
			}
		}
		else if(array[mid] > target)
		{
			// try left, if that fails, try right
			if(rotatedBinarySearch(target, mid, array, targetIndex))
			{
				return true;
			}
			else if(rotatedBinarySearch(target, count - mid, array + mid, targetIndex))
			{
				*targetIndex = *targetIndex + mid;
				return true;
			}
		}
		else
		{
			*targetIndex = mid; 
			return true;
		}
		
		return false;
	}
}

int main(int argc, char *argv[])
{
	int A[] = {1, 3, 5, 7, 7, 9, 9, 11, 0, 0, 0, 0, 0, 0};
	int B[] = {2, 4, 6, 8, 10, 12};

	printf("A = ");
	printArray(8, A);
	printf("B = ");
	printArray(6, B);
	printf("A merged with B = ");
	merge(8, A, 6, B);
	printArray(14, A);
	
	const char *const strings[] = {"abc123", "foo", "bar", "a1b2c3", "arb", "baz", "def"};
	sortAnagramsEqual(_countof(strings), strings);
	
	printf("\nSorted with anagrams equal = {");
	for(size_t i = 0; i < _countof(strings); i++)
	{
		if(i != _countof(strings) - 1)
		{
			printf("%s, ", strings[i]);
		}
		else
		{
			printf("%s", strings[i]);
		}
	}
	printf("}\n");
	
	int binSearchInput[] = {15, 16, 19, 20, 25, 1, 3, 4, 5, 7, 10, 14};
	size_t index = 0;
	if(rotatedBinarySearch(5, _countof(binSearchInput), binSearchInput, &index))
	{
		printf("\nRotated binary search: %d is at index %zu (array[%zu] = %d).\n", 5, index, index, binSearchInput[index]); 
	}
	else
	{
		printf("\nRotated binary search failed!\n");
	}

	int binSearchInput2[] = {5, 7, 10, 14, 15, 16, 19, 20, 25, 1, 3, 4};
	if(rotatedBinarySearch(5, _countof(binSearchInput), binSearchInput2, &index))
	{
		printf("\nRotated binary search 2: %d is at index %zu (array[%zu] = %d).\n", 5, index, index, binSearchInput2[index]); 
	}
	else
	{
		printf("\nRotated binary search 2 failed!\n");
	}
	
	return 0;
}
