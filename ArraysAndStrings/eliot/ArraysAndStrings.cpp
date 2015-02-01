#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	
	return 0;
}
