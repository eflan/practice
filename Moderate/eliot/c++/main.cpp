#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/**
 * a = 0, b = 1
 * a = 0 xor 1 = 1
 * b = 1 xor 1 = 0
 * 
 * a = 01, b = 10
 * a = 01 xor 10 = 11
 * b = 11 xor 10 = 01
 * a = 11 xor 01 = 10
 **/
void swapInPlace(size_t *a, size_t *b)
{
	*a = (*a)^(*b);
	*b = (*a)^(*b);
	*a = (*a)^(*b);
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	size_t a = rand();
	size_t b = rand();
	
	printf("Swap --\na = %zu, b = %zu\n", a, b);
	swapInPlace(&a, &b);
	printf("a = %zu, b = %zu\n", a, b);
	
	return 0;
}
