#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int add(int a, int b)
{
	int sum = 0;
	size_t bit = 1;
	bool carry = false;
	
	while(bit < 4294967296)
	{
		int curBit = static_cast<int>((a & bit) ^ (b & bit));
		if(carry)
		{
			if(curBit == 0)
			{
				sum |= bit;
				carry = ((a & bit) > 0) && ((b & bit) > 0);
			}
			else
			{
				// carry again, leaving this bit of sum as zero
			}
		}
		else
		{
			sum |= curBit;
			carry = ((a & bit) > 0) && ((b & bit) > 0);
		}
		
		bit <<= 1;		
	}
	
	return sum;
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	
	printf("Addition without any arithmetic operators --\n");
	for(unsigned int i = 0; i < 10; i++)
	{
		int a = rand();
		int b = rand();
		
		if((rand() % 2) == 0)
		{
			a = -a;
		}
		
		if((rand() % 2) == 0)
		{
			b = -b;
		}
		printf("%d + %d = %d (should be %d)\n", a, b, add(a, b), (a + b));
	}
	
	return 0;
}
