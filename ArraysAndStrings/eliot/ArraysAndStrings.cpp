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

int main(int argc, char *argv[])
{
	char str1[] = "abcabc";
	char str2[] = "abcdefghijklmnopqrstuvwxyz";
	
	printf("Simple_AllCharsUnique(\"%s\") is %u.\n", str1, Simple_AllCharsUnique(str1));
	printf("Simple_AllCharsUnique(\"%s\") is %u\n", str2, Simple_AllCharsUnique(str2));
	printf("Convoluted_AllCharsUnique(\"%s\") is %u.\n", str1, Convoluted_AllCharsUnique(str1));
	printf("Convoluted_AllCharsUnique(\"%s\") is %u\n", str2, Convoluted_AllCharsUnique(str2));
	
	char reverse1[] = "abc1234";
	char reverse2[] = "abcdefghijklmnopqrstuvwxyz";
	
	printf("reverse(%s) is ", reverse1);
	reverse(reverse1);
	printf("%s.\n", reverse1);
	printf("reverse(%s) is ", reverse2);
	reverse(reverse2);
	printf("%s.\n", reverse2);

	char permutation1[] = "abc123";
	char permutation2[] = "1a2b3c";
	char notPermutation1[] = "abc12";
	char notPermutation2[] = "zyx890";
	
	printf("IsPermutation(%s, %s) is %u.\n", permutation1, permutation2, IsPermutation(permutation1, permutation2));
	printf("IsPermutation(%s, %s) is %u.\n", permutation1, notPermutation1, IsPermutation(permutation1, notPermutation1));
	printf("IsPermutation(%s, %s) is %u.\n", permutation1, notPermutation2, IsPermutation(permutation1, notPermutation2));
	printf("IsPermutation(%s, %s) is %u.\n", permutation2, notPermutation1, IsPermutation(permutation2, notPermutation1));
	printf("IsPermutation(%s, %s) is %u.\n", permutation2, notPermutation2, IsPermutation(permutation2, notPermutation2));
	
	return 0;
}
