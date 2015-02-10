#include <stdio.h>
#include <string.h>

void uint_to_binary(unsigned int x, char *binary)
{
    binary[0] = '\0';

    unsigned int z;
    for (z = 2147483648; z > 0; z >>= 1)
    {
        strcat(binary, ((x & z) == z) ? "1" : "0");
    }
}

// INPUT N = 10000000000, M = 10011, i = 2, j = 6. OUTPUT N = 10001001100
unsigned int InsertBits(unsigned int N, unsigned int M, const unsigned short i, const unsigned short j)
{
	char binary[33];
	
	uint_to_binary(M, binary);
	printf("%s << %u = ", binary, i);
	
	// shift M up by i bits (10011 << 2 = 1001100)
	M <<= i;
	
	uint_to_binary(M, binary);
	printf("%s\n", binary);
	
	// keep most significant bit through j
	unsigned int maskJ = 0xffffffffUL << (j + 1UL);
	
	uint_to_binary(maskJ, binary);
	printf("maskJ = %s\n", binary);
	
	// keep bits i through 0
	unsigned int maskI = (1UL << i) - 1UL;
	
	uint_to_binary(maskI, binary);
	printf("maskI = %s\n", binary);
	
	unsigned int mask = maskJ | maskI;
	
	uint_to_binary(mask, binary);
	printf("mask  = %s\n", binary);
	
	// or M and N together
	return M | (N & mask);
}

void TestInsertBits(const unsigned int N, const unsigned int M, const unsigned int i, const unsigned int j)
{
	unsigned int inserted = InsertBits(N, M, i, j);

	char binaryN[33];
	char binaryM[33];
	char binaryInserted[33];

	uint_to_binary(N, binaryN);
	uint_to_binary(M, binaryM);
	uint_to_binary(inserted, binaryInserted);
	
	printf("N     = %s\nM     = %s\ni = %u, j = %u\n      = %s\n", binaryN, binaryM, i, j, binaryInserted);

}

void PrintBinary(double d)
{
	// binary representation is like so --
	// 0th bit = 1/2, 1st bit = 1/4, 2nd bit = 1/8, etc.
	if(d >= 1.0d || d <= 0.0d)
	{
		printf("%lf in binary is ERROR\n", d);
	}
	else
	{
		char binary[33];
		size_t i = 0;
		double temp = d;
		
		for(; i < 33; i++)
		{
			if(temp <= 0.0d)
			{
				break;
			}

			double bit = temp * 2.0d;
			if(bit >= 1.0d)
			{
				binary[i] = '1';
				temp = bit - 1.0d;
			}
			else
			{
				binary[i] = '0';
				temp = bit;
			}			
		}
		
		if(temp > 0.0d)
		{
			printf("%lf in binary is ERROR\n", d);
		}
		else
		{
			binary[i] = '\0';
			printf("%lf in binary is %s\n", d, binary);
		}
	}
}

bool mystery(int n)
{
	// tests if n is a power of 2
	return (n & (n - 1)) == 0;
}

int main(int argc, char *argv[])
{
	TestInsertBits(1024, 19, 2, 6);
	TestInsertBits(0xffffffff, 19, 2, 6);
	TestInsertBits(0xffffffff, 19, 27, 31);
	
	printf("\n");
	char binaryN[33];
	char binaryNMinus1[33];
	
	for(int i = 1; i <= 1024; i *= 2)
	{
		uint_to_binary(i, binaryN);
		uint_to_binary(i - 1, binaryNMinus1);
		printf("(%u & %u) a.k.a. (%s & %s) == 0? %s.\n", i, i - 1, binaryN, binaryNMinus1, mystery(i) ? ("Yes") : ("No"));
	}
	
	printf("\n");
	
	for(int i = 2; i <= 1025; i *= 2)
	{
		uint_to_binary(i + 1, binaryN);
		uint_to_binary(i, binaryNMinus1);
		printf("(%u & %u) a.k.a. (%s & %s) == 0? %s.\n", i + 1, i, binaryN, binaryNMinus1, mystery(i + 1) ? ("Yes") : ("No"));
	}
	
	printf("\n");
	PrintBinary(0.72d);
	PrintBinary(0.25d);
	PrintBinary(0.125d);
	PrintBinary(0.25d + 0.125d);
	
	return 0;
}
