#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

void wordDistance();

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

const size_t perfectRand()
{
	return rand();
}

typedef unsigned char Card;
#define INVALID_CARD 0

void perfectShuffle(Card cards[52])
{
	Card shuffled[52] = {};
	
	for(Card c = 1; c <= 52; c++)
	{
		size_t index = perfectRand() % 52;
	
		while(shuffled[index] != INVALID_CARD)
		{
			index = perfectRand() % 52;
		}
		
		shuffled[index] = c;
	}
	
	memcpy(cards, shuffled, sizeof(shuffled));
}

std::vector<int> choose(const size_t m, const std::vector<int> &numbers)
{
	auto it = numbers.begin();
	std::advance(it, m);
	std::vector<int> subset(numbers.begin(), it);
	
	for(size_t i = m; i < numbers.size(); i++)
	{
		size_t k = perfectRand() % i;
		if(k < m)
		{
			subset[k] = numbers[i];
		}
	}
	
	return subset;
}

const size_t twoDigits(size_t num)
{
	size_t twos = 0;
	
	while(num > 1)
	{
		if((num % 10) == 2)
		{
			twos++;
		}
		
		num /= 10;
	}
	
	return twos;
}

size_t countTwosBruteForce(const size_t n)
{
	size_t totalTwos = 0;
	
	for(size_t i = 0; i <= n; i++)
	{
		totalTwos += twoDigits(i);
	}
	
	return totalTwos;
}

size_t countDigits(size_t n)
{
	size_t digits = 0;
	
	while(n > 0)
	{
		digits++;
		n /= 10;
	}
	
	return digits;
}

const size_t power(const size_t base, const size_t exponent)
{
	size_t pow = 1;
	if(exponent == 0)
	{
		return 1;
	}
	else
	{
		for(size_t i = 0; i < exponent; i++)
		{
			pow *= base;
		}
		
		return pow;
	}
}

const size_t countTwosForDigit(const size_t n, const size_t digit)
{
	size_t place = power(10, digit);
	
	const size_t nDigit = (n / place) % 10;
	
	if(nDigit < 2)
	{
		return (n - (n % (place * 10))) / 10;
	}
	else if(nDigit > 2)
	{
		return ((n - (n % (place * 10))) + (place * 10)) / 10; 
	}
	else
	{
		return ((n - (n % (place * 10))) / 10) + (n % place) + 1;
	}
}

const size_t countTwos(const size_t n)
{
	size_t twos = 0;
	const size_t digitCount = countDigits(n);
	for(size_t digit = 0; digit < digitCount; digit++)
	{
		twos += countTwosForDigit(n, digit);
	}
	
	return twos;
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
	
	Card cards[52];
	for(Card c = 1; c <= 52; c++)
	{
		cards[c - 1] = c;
	}
	perfectShuffle(cards);
	printf("\nPerfect shuffle = ");
	for(size_t i = 0; i < 52; i++)
	{
		printf("%u, ", cards[i]);
	}
	printf("\n");
	
	std::vector<int> set(100);
	for(size_t i = 0; i < 100; i++)
	{
		set[i] = i;
	}
	
	std::vector<int> subset = choose(10, set);
	
	printf("\nChose {");
	for(size_t i = 0; i < subset.size(); i++)
	{
		printf("%d, ", subset[i]);
	}
	printf("}\n");
	
	printf("\n");
	for(size_t i = 10; i < 100000000; i *= 10)
	{
		printf("Count twos 0...%zu = %zu\n", i, countTwos(i));
	}
	
	printf("\n");
	for(size_t i = 10; i < 100000000; i *= 10)
	{
		printf("Count twos (brute force) 0...%zu = %zu\n", i, countTwosBruteForce(i));
	}
	
	wordDistance();
	
	return 0;
}
