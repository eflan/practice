#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unordered_map>
#include <vector>

void printWords(const std::vector<std::string> &words)
{
	printf("{");
	for(const std::string &word : words)
	{
		printf("%s, ", word.c_str());
	}
	printf("}");
}

void merge(std::vector<std::string> &words, std::vector<std::string> &helper, const size_t low, const size_t mid, const size_t high)
{
	for(size_t i = low; i <= high; i++)
	{
		helper[i].assign(words[i]);
	}
	
	size_t left = low;
	size_t right = mid + 1;
	
	for(size_t cur = low; cur <= high; cur++)
	{
		if(left <= mid && (right > high || (helper[left].length() > helper[right].length())))
		{
			words[cur].assign(helper[left]);
			left++;
		}
		else if(right <= high)
		{
			words[cur].assign(helper[right]);
			right++;
		}
	}
}

void sortDecreasing(std::vector<std::string> &words, std::vector<std::string> &helper, const size_t low, const size_t high)
{
	if(low < high)
	{
		const size_t mid = (low + high) / 2;
		sortDecreasing(words, helper, low, mid);
		sortDecreasing(words, helper, mid + 1, high);
		merge(words, helper, low, mid, high);
	}
}

// merge sort, decreasing
void sortDecreasing(std::vector<std::string> &words)
{
	std::vector<std::string> helper(words.size());
	sortDecreasing(words, helper, 0, words.size() - 1);
}

typedef std::unordered_map<char, std::vector<std::string *> > WordBuckets;

WordBuckets bucketByInitial(std::vector<std::string> &words)
{
	WordBuckets buckets;
	
	for(size_t i = 0; i < words.size(); i++)
	{
		std::string *curWord = &(words[i]);
		char initial = curWord->at(0);
	
		auto it = buckets.find(initial);
		if(it == buckets.end())
		{
			std::vector<std::string *> oneElement;
			oneElement.push_back(curWord);
			buckets[initial] = oneElement;
		}
		else
		{
			buckets[initial].push_back(curWord);
		}
	}
	
	return buckets;
}

const size_t endOf(const std::string &word, const size_t index, const std::string &substring)
{
	const size_t end = index + substring.length();
	if(end > word.length())
	{
		return 0;
	}
	
	for(size_t i = index; i < word.length() && (i - index) < substring.length(); i++)
	{
		if(word[i] != substring[i - index])
		{
			return 0;
		}
	}
	
	return end;
}

bool decompose(const std::string &word, const size_t index, const WordBuckets &wordBuckets, std::vector<std::string> &components)
{
	if(index == word.size())
	{
		return true;
	}
	
	auto it = wordBuckets.find(word[index]);
	if(it == wordBuckets.end())
	{
		components.clear();
		return false;
	}
	else
	{
		// Try every word that fits
		for(const std::string *substring : it->second)
		{
			size_t end = endOf(word, index, *substring);
			if(end != 0)
			{
				//
				// Don't just go with the word we are currently trying unless
				// we are already past the beginning of the word.
				//
				
				if(components.size() != 0 || end < word.size())
				{
					components.push_back(*substring);
					if(decompose(word, end, wordBuckets, components))
					{
						return true;
					}
					else
					{
						components.pop_back();
					}
				}
			}
		}
		
		components.clear();
		return false;
	}
}

std::string makeLongest(std::vector<std::string> &words, std::vector<std::string> &components)
{
	sortDecreasing(words);
	WordBuckets byInitial = bucketByInitial(words);
	
	for(std::string &word : words)
	{
		if(decompose(word, 0, byInitial, components))
		{
			return word;
		}
	}
	
	return std::string();
}

void longestWord()
{
	std::vector<std::string> words;
	words.push_back(std::string("cat"));
	words.push_back(std::string("banana"));
	words.push_back(std::string("dog"));
	words.push_back(std::string("nana"));
	words.push_back(std::string("hammock"));
	words.push_back(std::string("walk"));
	words.push_back(std::string("walker"));
	words.push_back(std::string("bananahammock"));
	words.push_back(std::string("dogwalker"));
	
	printf("\nLongest word in \n");
	printWords(words);
	
	std::vector<std::string> components;
	std::string longest = makeLongest(words, components);
	
	printf("\nis %s which is composed of ", longest.c_str());
	printWords(components);
	printf("\n");
}
