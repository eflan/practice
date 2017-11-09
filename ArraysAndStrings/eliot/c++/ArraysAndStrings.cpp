#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>

/**
 * Implement atoi(char *str)
 */
bool isDigit(char ch)
{
  if(ch < '0' || ch > '9')
  {
    return false;
  }
  else
  {
    return true;
  }
}

int atoi(char *str)
{
  const int ErrorValue = -1;

  if(str == NULL || *str == '\0')
  {
    return ErrorValue;
  }

  int sign = 1;
  if(*str == '-')
  {
    sign = -1;
    str++;
  }

  int num = 0;

  for(int i = 0; *str != '\0'; i++, str++)
  {
    num *= 10;

    if(!isDigit(*str))
    {
      return ErrorValue;
    }

    num += *str - '0';
  }

  return num;
}

/**
 * 1. Implement an algorithm to determine if a string has all unique characters. What if you cannot use any additional data structures?
 */
 bool Simple_AllCharsUnique(char *str)
 {
	// The easy way, put every character into a hash table
	bool seen[255] = {};
	size_t i = 0;
	
	while(str[i] != '\0')
	{
		//
		// Don't bother having a slot for (char)0 a.k.a. '\0'
		// because we bail out of the loop above if we encounter
		// the terminating NUL.
		//
		// Instead, keep (char)1 in slot 0 and so on.
		//
		
		if(seen[str[i] - static_cast<char>(1)])
		{
			return false;
		}
		else
		{
			seen[str[i] - static_cast<char>(1)] = true;
		}
		
		i++;
	}
	
	return true;
 }
 
 int string_comparer(const void *pch1, const void *pch2)
 {
	 const char ch1 = *(reinterpret_cast<const char *>(pch1));
	 const char ch2 = *(reinterpret_cast<const char *>(pch2));

     if(ch1 > ch2)
     {
		 return 1;
	 }
	 else if(ch1 < ch2)
	 {
		 return -1;
	 }
	 else
	 {
		 return 0;
	 }
 }
 
 //
 // Uses no additional data structures
 //
 bool Convoluted_AllCharsUnique(char *str)
 {
	 // Just sort the string and then check if any two adjacent characters are the same
	 size_t len = strlen(str);
	 qsort(str, len, sizeof(char), string_comparer);
	 
	 // N.B. if the string is empty or of length one it contains all unique characters.
	 for(size_t i = 1; i < len; i++)
	 {
		 if(str[i - 1] == str[i])
		 {
			 return false;
		 }
	 }
	 
	 return true;
 }
 
 /**
  * 2. Implement a function void reverse(char *str) in C/C++ which reverses a null terminated string.
  */
void reverse(char *str)
{
	if(str != nullptr)
	{
		// Keep a pointer to the beginning of the string.
		char *begin = str;
		
		// Walk to the end of the string.
		while(*str != '\0')
		{
			str++;
		}
		
		// An empty string identical to its reverse.
		if(str != begin)
		{
			// We don't want to swap with the terminating NUL.
			str--;
			
			// Now walk both pointers toward the middle, swapping characters as we go.
			while(begin < str)
			{
				char temp = *begin;
				*begin = *str;
				*str = temp;
				begin++;
				str--;
			}
		}
	}
}

bool IsPermutation(char *str1, char *str2)
{
	size_t totalChars = 0;
	
	// '\0' is not a valid string character so don't bother counting it
	// in the zero index of this array
	unsigned int charCount[255] = {};

	if(str1 == str2)
	{
		// they’re the same string so by definition they are permutations of one another
		return true;
	}
	else if(str1 == nullptr || str2 == nullptr)
	{
		// treat null strings as invalid input
		return false;
	}
	
	while(*str1 != '\0')
	{
		charCount[static_cast<unsigned int>(*str1) + 1]++;
		str1++;
		totalChars++;
	}

	while(*str2 != '\0')
	{
		if(charCount[static_cast<unsigned int>(*str2) + 1] == 0)
		{
			return false;
		}
		else if(totalChars == 0)
		{
			// one string has more characters than the other
			return false;
		}
		else
		{
			charCount[static_cast<unsigned int>(*str2) + 1]--;
			totalChars--;
		}
		
		str2++;
	}

	return totalChars == 0;
}

//
// Example
// Input: “Mr John Smith    “, 13
// Output: “Mr%20John%20Smith”
//

void EncodeSpaces(char *str, size_t length)
{
	char *end = (str + length) - 1;
	char *bufferEnd = str + length;

	if(str == nullptr || length == 0)
	{
		return;
	}

	// Find the end of the buffer of trailing whitespace
	// Assume it exactly fits the expanded space characters
	while(*bufferEnd != '\0')
	{
		bufferEnd++;
	}
	
	bool munching  = false;
	size_t countToMove = 0;

	while(end >= str)
	{
		// Starting at the end of the string, walk toward the start until we encounter a space
		if(*end == ' ')
		{
			if(!munching)
			{
				munching = true;
				
				if(reinterpret_cast<size_t>(bufferEnd) < countToMove || 
				   bufferEnd - countToMove < end)
				{
					throw "Buffer overrun! Buffer is too small to expand all spaces!";
				}
				
				// memmove the characters so far to the end of the string
				char *move = end + 1;
				memmove(bufferEnd - countToMove, move, countToMove);
				bufferEnd -= countToMove;
				countToMove = 0;
			}

			// Ensure we aren’t overrunning the buffer
			if(reinterpret_cast<size_t>(bufferEnd) < 3 || bufferEnd - 3 < end)
			{
				throw "Buffer too small to expand all spaces!";
			}

			// munch spaces, encoding them as %20 at the end of the string
			char *begin = bufferEnd - 3;
			begin[0] = '%';
			begin[1] = '2';
			begin[2] = '0';
			
			// move one step toward the start of the string
			end--;
			// advance bufferEnd to account for the %20
			bufferEnd -= 3;
		}
		else
		{
			if(munching)
			{
				munching = false;
			}		
			
			// accumulate another character to move and move end one
			// step toward the start of the string
			countToMove++;
			end--;
		}
	}
	
	if(countToMove > 0 && bufferEnd != end + 1 + countToMove)
	{
		throw "String contained too much trailing whitespace!"; 
	}
}

// Iterates on str so long as there are repeated characters
// Then compresses the characters by writing x? where x is a char
// and ? is a decimal number with an aribitrary number of digits
char *RunlengthEncode(char *str, char *compressed, size_t compressedSize, size_t *cchCompressed)
{	
	char *end = str + 1;
	while(*end != '\0' && *end == *str)
	{
		end++;
	}
	
	size_t num = end - str;
	*cchCompressed = snprintf(compressed, compressedSize, "%c%zu", *str, num);
	
	if(*cchCompressed >= compressedSize)
	{
		return nullptr;
	}
	else
	{
		return end;
	}
}

bool CompressStringHelper(char *str, char *compressed, size_t compressedSize)
{
	if(*str == '\0')
	{
		// if we reached the end of the string then we successfully
		// compressed it because we would have bailed out before
		// reaching the end if we compressed it and it was larger
		// than the original string.
		return true;
	}
	
	size_t cch = 0;
	char *end = RunlengthEncode(str, compressed, compressedSize, &cch);
	if(end == nullptr)
	{
		return false;
	}
	else
	{
		return CompressStringHelper(end, compressed + cch, compressedSize - cch);
	}
}

char *CompressString(char *str)
{
	size_t length = strlen(str);
	if(length == 0)
	{
		// Can't compress an empty or null string
		return str;
	}
	
	// Allocate a buffer that holds a string one character smaller
	// than str. This ensures that the compressed string is always
	// smaller than str. Compression will fail if the compressed
	// string would be larger or as large as str.
	char *compressed = new char[length];
	std::unique_ptr<char> spCompressed(compressed);
	
	if(CompressStringHelper(str, compressed, length + 1))
	{
		spCompressed.release();
		return compressed;
	}
	else
	{
		return str;
	}
}

class Pixel
{
public:
	Pixel() : _r(0), _g(0), _b(0), _alpha(0) {}

	Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha)
		:_r(r), _g(g), _b(b), _alpha(alpha)
	{
	}
	
	void Print()
	{
		printf("(%2.2u, %2.2u, %2.2u, %2.2u)", _r, _g, _b, _alpha);
	}
	
	Pixel &operator=(Pixel &rhs)
	{
		_r = rhs._r;
		_g = rhs._g;
		_b = rhs._b;
		_alpha = rhs._alpha;
		return *this;
	}
	
private:
	unsigned char _r;
	unsigned char _g;
	unsigned char _b;
	unsigned char _alpha;
};

void RotateImage90(Pixel *image, const size_t n)
{
	// Example:
	// AB = CA
	// CD   DB
	// 
	// ABCD = MIEA
	// EFGH   NJFB
	// IJKL   OKGC
	// MNOP   PLHD
	
	// Iterate over columns, the new column is row N - column index
	
	Pixel *rotated = new Pixel[n * n];
	
	for(size_t i = 0; i < n; i++)
	{
		for(size_t k = 0; k < n; k++)
		{
			rotated[(k * n) + i] = image[(((n - 1) - i) * n) + k];
		}
	}

	memcpy(image, rotated, n * n * sizeof(Pixel));
	
	delete[] rotated;
}

void ZeroMatrixRow(const size_t row, int *matrix, const size_t m, const size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		matrix[(row * n) + i] = 0;
	}
}

void ZeroMatrixColumn(const size_t col, int *matrix, const size_t m, const size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		matrix[(i * n) + col] = 0;
	}
}

void ZeroMatrix(int *matrix, const size_t m, const size_t n)
{
	bool *rowsToZero = new bool[m];
	bool *colsToZero = new bool[n];
	
	for(size_t i = 0; i < m; i++)
	{
		rowsToZero[i] = false;
	}

	for(size_t i = 0; i < n; i++)
	{
		colsToZero[i] = false;
	}
	
	for(size_t i = 0; i < m; i++)
	{
		for(size_t k = 0; k < n; k++)
		{
			if(matrix[(i * n) + k] == 0)
			{
				rowsToZero[i] = true;
				colsToZero[k] = true;
			}
		}
	}
	
	for(size_t i = 0; i < m; i++)
	{
		if(rowsToZero[i])
		{
			ZeroMatrixRow(i, matrix, m, n);
		}
	}

	for(size_t i = 0; i < n; i++)
	{
		if(colsToZero[i])
		{
			ZeroMatrixColumn(i, matrix, m, n);
		}
	}
	
	delete[] rowsToZero;
	delete[] colsToZero;
}

bool IsSubString(char const*substring, char const*strToSearch)
{
	if(substring == NULL || strToSearch == NULL)
	{
		return false;
	}
	else if(substring == strToSearch)
	{
		return true;
	}
	
	char const *sub = substring;
	
	while(*strToSearch != '\0' && *sub != '\0')
	{
		if(*sub == *strToSearch)
		{
			// move on to next character in substring
			sub++;
		}
		else
		{
			// start over
			sub = substring;
		}
		
		strToSearch++;
	}
	
	return *sub == '\0';
}

// Returns true if s1 is a rotation of s2
bool IsRotation(char const *s1, char const *s2)
{
	// double s2 and then see if s1 is a substring of the doubling
	size_t s2Size = strlen(s2);
	
	const size_t s2DoubledSize = (s2Size * 2) + 1;
	if(s2DoubledSize < s2Size)
	{
		// integer overflow
		throw "Integer overflow!";
	}
	
	char *s2Doubled = new char[s2DoubledSize];
	memcpy(s2Doubled, s2, s2Size * sizeof(char));
	memcpy(s2Doubled + s2Size, s2, s2Size);
	s2Doubled[s2DoubledSize - 1] = '\0';
	
	bool isRotation = IsSubString(s1, s2Doubled);
	delete[] s2Doubled;
	return isRotation;
}

void inPlaceReverse(char *str)
{
  if(str == NULL)
  {
    return;
  }

  size_t length = strlen(str);
  char temp = '\0';
  size_t halfLength = length / 2;

  for(int i = 0; i < halfLength; i++)
  {
    temp = str[i];
    str[i] = str[(length - 1) - i];
    str[(length - 1) - i] = temp;
  }
}

int main(int argc, char *argv[])
{
	char str1[] = "abcabc";
	char str2[] = "abcdefghijklmnopqrstuvwxyz";
	
	printf("\nSimple_AllCharsUnique(\"%s\") is %u.\n", str1, Simple_AllCharsUnique(str1));
	printf("Simple_AllCharsUnique(\"%s\") is %u\n", str2, Simple_AllCharsUnique(str2));
	printf("Convoluted_AllCharsUnique(\"%s\") is %u.\n", str1, Convoluted_AllCharsUnique(str1));
	printf("Convoluted_AllCharsUnique(\"%s\") is %u\n", str2, Convoluted_AllCharsUnique(str2));
	
	char reverse1[] = "abc1234";
	char reverse2[] = "abcdefghijklmnopqrstuvwxyz";
	
	printf("\nreverse(%s) is ", reverse1);
	reverse(reverse1);
	printf("%s.\n", reverse1);
	printf("reverse(%s) is ", reverse2);
	reverse(reverse2);
	printf("%s.\n", reverse2);

	char permutation1[] = "abc123";
	char permutation2[] = "1a2b3c";
	char notPermutation1[] = "abc12";
	char notPermutation2[] = "zyx890";
	
	printf("\nIsPermutation(\"%s\", \"%s\") is %u.\n", permutation1, permutation2, IsPermutation(permutation1, permutation2));
	printf("IsPermutation(\"%s\", \"%s\") is %u.\n", permutation1, notPermutation1, IsPermutation(permutation1, notPermutation1));
	printf("IsPermutation(\"%s\", \"%s\") is %u.\n", permutation1, notPermutation2, IsPermutation(permutation1, notPermutation2));
	printf("IsPermutation(\"%s\", \"%s\") is %u.\n", permutation2, notPermutation1, IsPermutation(permutation2, notPermutation1));
	printf("IsPermutation(\"%s\", \"%s\") is %u.\n", permutation2, notPermutation2, IsPermutation(permutation2, notPermutation2));

	char encode1[] = "Mr John Smith    ";
	char encode2[] = "Mr John   Smith        ";
	char encodeErrorTooLong[] = "Mr John   Smith         ";
	char encodeErrorTooShort[] = "Mr John   Smith       ";
	char beginsWithSpace[] = " Mr John   Smith          ";
	char beginsWithSpaceTooShort[] = " Mr John   Smith         ";
	
	printf("\nEncodeSpaces(\"%s\", %u) = ", encode1, 13);
	EncodeSpaces(encode1, 13);
	printf("%s\n", encode1);
	printf("EncodeSpaces(\"%s\", %u) = ", encode2, 15);
	EncodeSpaces(encode2, 15);
	printf("%s\n", encode2);
	printf("EncodeSpaces(\"%s\", %u) = ", beginsWithSpace, 16);
	EncodeSpaces(beginsWithSpace, 16);
	printf("%s\n", beginsWithSpace);
	
	try
	{
		printf("EncodeSpaces(\"%s\", %u) throws ", encodeErrorTooLong, 15);
		EncodeSpaces(encodeErrorTooLong, 15);
	}
	catch(char const*error)
	{
		if(strcmp("String contained too much trailing whitespace!", error) != 0)
		{
			printf("Wrong exception (\"%s\") thrown by EncodeSpaces(%s, 15)!\n", error, encodeErrorTooLong);
		}
		else
		{
			printf("\"%s\"\n", error);
		}
	}

	try
	{
		printf("EncodeSpaces(\"%s\", %u) throws ", encodeErrorTooShort, 15);
		EncodeSpaces(encodeErrorTooShort, 15);
	}
	catch(char const*error)
	{
		if(strcmp("Buffer too small to expand all spaces!", error) != 0)
		{
			printf("Wrong exception (\"%s\") thrown by EncodeSpaces(%s, 15)!\n", error, encodeErrorTooShort);
		}
		else
		{
			printf("\"%s\"\n", error);
		}
	}

	try
	{
		printf("EncodeSpaces(\"%s\", %u) throws ", beginsWithSpaceTooShort, 16);
		EncodeSpaces(beginsWithSpaceTooShort, 16);
	}
	catch(char const*error)
	{
		if(strcmp("Buffer too small to expand all spaces!", error) != 0)
		{
			printf("Wrong exception (\"%s\") thrown by EncodeSpaces(%s, 16)!\n", error, beginsWithSpaceTooShort);
		}
		else
		{
			printf("\"%s\"\n", error);
		}
	}
	
	char compressMe[] = "aabcccccaaa";
	std::unique_ptr<char> spCompressed;
	char *compressed = CompressString(compressMe);
	if(compressed != compressMe)
	{
		spCompressed.reset(compressed);
	}
	
	printf("\nCompressString(%s) = %s\n", compressMe, compressed);

	char doNotCompress[] = "abc";
	compressed = CompressString(doNotCompress);
	if(compressed != doNotCompress)
	{
		spCompressed.reset(compressed);
		printf("Erronious! \"abc\" should not be compressed\n");
	}
	else
	{
		printf("CompressString(%s) = %s\n", doNotCompress, compressed);
	}

	printf("\nImage\n");
	unsigned char ch = 0;
	const size_t n = 4;
	Pixel image[n * n];
	for(size_t i = 0; i < 4; i++)
	{
		for(size_t k = 0; k < 4; k++)
		{
			Pixel p(ch, ch, ch, ch);
			image[(i * n) + k] = p;
			ch++;
			
			image[(i * n) + k].Print();
		}
		printf("\n");
	}

	printf("\nAfter rotation is \n");
	RotateImage90(image, 4);

	for(size_t i = 0; i < 4; i++)
	{
		for(size_t k = 0; k < 4; k++)
		{
			image[(i * n) + k].Print();
		}
		printf("\n");
	}
	
	int matrix[5 * 10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		                   1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		                   1, 2, 3, 4, 5, 6, 0, 8, 9, 10,
		                   1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		                   1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	printf("Original matrix\n");
	for(size_t i = 0; i < 5; i++)
	{
		for(size_t k = 0; k < 10; k++)
		{
			printf(" %2.2u ", matrix[(i * 10) + k]);
		}
		printf("\n");
	}

	ZeroMatrix(matrix, 5, 10);

	printf("Zeroed matrix\n");
	for(size_t i = 0; i < 5; i++)
	{
		for(size_t k = 0; k < 10; k++)
		{
			printf(" %2.2u ", matrix[(i * 10) + k]);
		}
		printf("\n");
	}
	
	printf("\nIsSubstring(\"%s\", \"%s\") = %u\n", "water", "waterbottle", IsSubString("water", "waterbottle"));
	printf("IsSubstring(\"%s\", \"%s\") = %u\n", "erbott", "waterbottle", IsSubString("erbott", "waterbottle"));
	printf("IsSubstring(\"%s\", \"%s\") = %u\n", "erbottle", "waterbottle", IsSubString("erbottle", "waterbottle"));
	printf("IsSubstring(\"%s\", \"%s\") = %u\n", "waterbottl", "waterbottle", IsSubString("waterbottl", "waterbottle"));
	printf("IsSubstring(\"%s\", \"%s\") = %u\n", "waterbottle", "waterbottle", IsSubString("waterbottle", "waterbottle"));
	printf("IsSubstring(\"%s\", \"%s\") = %u\n", "waterbottlee", "waterbottle", IsSubString("waterbottlee", "waterbottle"));
	printf("IsSubstring(\"%s\", \"%s\") = %u\n", "are", "waterbottle", IsSubString("are", "waterbottle"));
	
	printf("\nIsRotation(\"%s\", \"%s\") = %u\n", "waterbottle", "erbottlewat", IsRotation("waterbottle", "erbottlewat"));
	printf("IsRotation(\"%s\", \"%s\") = %u\n", "waterbottle", "erbottlewat", IsRotation("waterbottle", "erbottlewate"));
	printf("IsRotation(\"%s\", \"%s\") = %u\n", "waterbottle", "erbottlewat", IsRotation("waterbottle", "erbottelwat"));

        printf("atoi(\"foo\") == %d\n", atoi("foo"));
        printf("atoi(\"-1234\") == %d\n", atoi("-1234"));
        printf("atoi(\"567\") == %d\n", atoi("567"));
        printf("atoi(\"0\") == %d\n", atoi("0"));
        printf("atoi(\"-0\") == %d\n", atoi("-0"));	

        char abcString[] = "abc123";
        printf("abcString = %s\n", abcString);
        inPlaceReverse(abcString);
        printf("reverse(\"abc123\") = %s\n", abcString);

	return 0;
}
