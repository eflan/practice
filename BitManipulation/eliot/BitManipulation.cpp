#include <stdio.h>
#include <string.h>

void byte_to_binary(char x, char *binary)
{
    binary[0] = '\0';

    unsigned int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(binary, ((x & z) == z) ? "1" : "0");
    }
}

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

/**
 * Given a positive integer, print the next smallest and next largest
 * number that have the same number of one bits in their binary
 * represenation.
 */
const unsigned int NextLargestWithSameCountOneBits(const unsigned int n)
{	
	// (1) start at lowest bit,	
	// (2) find a one bit that is before a zero bit,
	for(unsigned int lowestBit = 0; lowestBit < 31; lowestBit++)
	{
		if((n & (1U << lowestBit)) != 0 && (n & (1U << (lowestBit + 1))) == 0)
		{
			// (3) shift that bit into the zero bit
			unsigned int nextLargest = n & ~(1U << lowestBit);
			nextLargest |= (1U << (lowestBit + 1));
			return nextLargest;
		}
	}
	
	// Default to n itself if we fail to construct the next largest
	// number with the same number of one bits.
	return n;
}

const unsigned int NextSmallestWithSameCountOneBits(unsigned int n)
{
	// (1) start at higest bit,	
	// (2) find a one bit that is before a zero bit,
	for(unsigned int highestBit = 31; highestBit > 0; highestBit--)
	{		
		if((n & (1U << highestBit)) != 0 && (n & (1U << (highestBit - 1))) == 0)
		{
			// (3) shift that bit into the zero bit
			unsigned int nextSmallest = n & ~(1U << highestBit);
			nextSmallest |= (1U << (highestBit - 1));
			return nextSmallest;
		}
	}
	
	// Default to n itself if we fail to construct the next largest
	// number with the same number of one bits.
	return n;
}

void TestNextLargest(const unsigned int n)
{
	const unsigned int nextLargest = NextLargestWithSameCountOneBits(n);
	
	char binary[33];
	uint_to_binary(n, binary);
	printf("               %.3u = %s\n", n, binary);
	
	uint_to_binary(nextLargest, binary);
	printf("next largest = %.3u = %s\n", nextLargest, binary);
}

void TestNextSmallest(const unsigned int n)
{
	const unsigned int nextSmallest = NextSmallestWithSameCountOneBits(n);
	
	char binary[33];
	uint_to_binary(n, binary);
	printf("                %.3u = %s\n", n, binary);
	
	uint_to_binary(nextSmallest, binary);
	printf("next smallest = %.3u = %s\n", nextSmallest, binary);
}

bool mystery(int n)
{
	// tests if n is a power of 2
	return (n & (n - 1)) == 0;
}

unsigned int CountBitsToConvertAtoB(const unsigned int A, const unsigned int B)
{
	// gather all bits that do not match in A and B
	unsigned int doNotMatch = A ^ B;
	unsigned int count = 0;
	
	// count all one bits in doNotMatch
	for(unsigned int i = 0; i < 32; i++)
	{
		count += (doNotMatch & 1U);
		doNotMatch >>= 1;
	}
	
	return count;
}

unsigned int SwapEvenAndOddBits(const unsigned int n)
{
	unsigned int output = 0;
	
	// walk the bits from lowest to highest
	// if the next one matches then keep moving
	// otherwise, swap them
	for(unsigned int i = 0; i < 32; i += 2)
	{
		unsigned int lowBit = n & (1U << i);
		unsigned int highBit = n & (1U << (i + 1));
		
		if(lowBit != highBit)
		{
			lowBit <<= 1;
			highBit >>= 1;
		}
		
		output |= lowBit | highBit;
	}
	
	return output;
}

class BinaryUInt
{
public:
	BinaryUInt(unsigned int n = 0U) : _num(n) {}
	
	void SetNum(unsigned int n)
	{
		_num = n;
	}
	
	void Print()
	{
		char binary[33];
		uint_to_binary(_num, binary);
		printf("BinaryUInt: %s\n", binary);
	}
	
	bool GetBit(unsigned int i)
	{
		return (_num & (1U << i)) != 0;
	}
	
private:
	unsigned int _num;
};

unsigned int MissingNumber(BinaryUInt *array, size_t count)
{
	unsigned int expectedOneBits[32] = {};
	unsigned int foundOneBits[32] = {};
	unsigned int place = 1;
	unsigned int missing = 0;
	
	for(unsigned int i = 0; i < count; i++)
	{
		array[i].Print();
	}
	
	for(unsigned int i = 0; i < 32; i++)
	{
		for(unsigned int k = 0; k < count + 1; k++)
		{
			if(((k / place) % 2) != 0)
			{
				expectedOneBits[i]++;
			}
			if(k < count && array[k].GetBit(i))
			{
				foundOneBits[i]++;
			}
		}
		place *= 2;
		
		if(foundOneBits[i] < expectedOneBits[i])
		{
			printf("Expected %u 1 bits for the %uth bit but found %u 1 bits instead.\n", expectedOneBits[i], i, foundOneBits[i]);
			missing |= (1U << i);
		}
	}
	
	return missing;
}

size_t RowOffset(size_t pxWidth, size_t y)
{
	return y * (pxWidth / 8);
}

size_t SetBitsFrom(char *rasterByte, size_t bit)
{
	unsigned char full = 0xff;
	full >>= bit;
	*rasterByte = full;
	char binary[9] = {};
	byte_to_binary(full, binary);
	printf("SetBitsFrom (bit %zu) = %s\n", bit, binary);
	return 8 - bit;
}

size_t SetBitsTo(char *rasterByte, size_t bit)
{
	unsigned char full = 0xff;
	full >>= bit;
	*rasterByte = ~full;
	char binary[9] = {};
	byte_to_binary(~full, binary);
	printf("SetBitsTo (bit %zu) = %s\n", bit, binary);
	return bit;
}

void DrawLine(size_t screenSize, char *screen, size_t pxWidth, size_t x1, size_t x2, size_t y)
{
	// x1, x2, and y are all bit indices that correspond to pixels
	size_t lineLength = x2 - x1;
	
	// start of the line
	const size_t lineStartByte = RowOffset(pxWidth, y) + (x1 / 8);
	// end of the line
	const size_t lineEndByte = RowOffset(pxWidth, y) + (x2 / 8);

	printf("\nLineStartByte = %zu, LineEndByte = %zu, LineLength = %zu\n", lineStartByte, lineEndByte, lineLength);
	
	size_t countBitsSet = SetBitsFrom(&(screen[lineStartByte]), (x1 % 8));
	lineLength -= countBitsSet;
		
	countBitsSet = SetBitsTo(&(screen[lineEndByte]), (x2 % 8));
	lineLength -= countBitsSet;
	
	// fill bytes in between
	for(size_t i = 0; i < lineLength; i += 8)
	{
		screen[lineStartByte + 1 + (i / 8)] = 0xff;
	}
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
	
	printf("\n");
	for(unsigned int n = 0; n < 10; n++)
	{
		TestNextLargest(n);
	}
	
	printf("\n");
	for(unsigned int n = 0; n < 10; n++)
	{
		TestNextSmallest(n);
	}
	
	printf("\n");
	printf("You would need to flip %u bits to convert %u to %u.\n", CountBitsToConvertAtoB(29, 15), 29, 15);
	printf("You would need to flip %u bits to convert %u to %u.\n", CountBitsToConvertAtoB(31, 15), 31, 15);
	printf("You would need to flip %u bits to convert %u to %u.\n", CountBitsToConvertAtoB(31, 0), 31, 0);
	printf("You would need to flip %u bits to convert %u to %u.\n", CountBitsToConvertAtoB(31, 31), 31, 31);
	printf("You would need to flip %u bits to convert %u to %u.\n", CountBitsToConvertAtoB(31, 30), 31, 30);

	printf("\n");
	unsigned int swapped = SwapEvenAndOddBits(17);
	char binaryInput[33];
	uint_to_binary(17, binaryInput);
	char binaryOutput[33];
	uint_to_binary(swapped, binaryOutput);
	printf("Swap even and odd bits of %u = %s -> %s = %u.\n", 17, binaryInput, binaryOutput, swapped);

	printf("\n");
	BinaryUInt array[10];
	for(unsigned int i = 0; i < 9; i++)
	{
		array[i].SetNum(i);
	}
	array[9].SetNum(10);
	
	// 9 is the missing number
	
	unsigned int missing = MissingNumber(array, 10);
	printf("Missing number is %u\n", missing);

	BinaryUInt array100[100];
	for(unsigned int i = 0; i < 100; i++)
	{
		array100[i].SetNum(i);
	}
	array100[88].SetNum(100);
	
	// 88 is the missing number
	
	missing = MissingNumber(array100, 100);
	printf("Missing number is %u\n", missing);
	
	char screen[80 * 10] = {};
	DrawLine(sizeof(screen), screen, 80, 50, 78, 50);
	
	printf("\nScreen with a line\n");
	char binary[9] = {};
	for(size_t i = 0; i < 80; i++)
	{
		for(size_t k = 0; k < 10; k++)
		{			
			byte_to_binary(screen[(i * 10) + k], binary);
			printf("%s ", binary);
		}
		printf(" [bytes %zu to %zu]\n", (i * 10), (i * 10) + 9);
	}
	return 0;
}
