#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unordered_map>
#include <vector>

template<typename K, typename V>
class Hashtable
{
public:
	Hashtable()
		: _buckets()
	{
	}
	
	const bool containsKey(const K &key) const
	{
		auto it = _buckets.find(key);
		return it != _buckets.end();
	}
	
	void put(const K &key, V &value)
	{
		_buckets[key] = value;
	}
	
	V &get(const K &key)
	{
		return _buckets.at(key);
	}
	
	const V &operator[](const K &key) const
	{
		return _buckets.at(key);
	}
	
private:
	std::unordered_map<K, V> _buckets;
};

class PositionList
{
public:
	static const size_t distance(const size_t posA, const size_t posB)
	{
		if(posA < posB)
		{
			return posB - posA;
		}
		else
		{
			return posA - posB;
		}
	}
	
	PositionList() : _positions() {}
	
	const size_t length() const { return _positions.size(); }
	
	void addPosition(const size_t index)
	{
		_positions.push_back(index);
	}
	
	const size_t operator[](const size_t index) const
	{
		return _positions[index];
	}
	 
	const std::pair<size_t, size_t> closestPair(const PositionList &other) const
	{
		print();
		printf("\n");
		other.print();
		printf("\n");
		
		if(length() < other.length())
		{
			return closestPair(*this, other);
		}
		else
		{
			return closestPair(other, *this);
		}
	}
	
	void print() const
	{
		printf("{");
		for(size_t i = 0; i < length(); i++)
		{
			if(i == length() - 1)
			{
				printf("%zu }", _positions[i]);
			}
			else
			{
				printf("%zu, ", _positions[i]);
			}
		}
	}
	
private:
	static std::pair<size_t, size_t> closestPair(const PositionList &smaller, const PositionList &larger)
	{
		// We iterate through smaller, finding minimum distance for each element
		// and then return the 
		std::pair<size_t, size_t> closest;
		size_t closestDistance = 0xffffffffffffffffUL;
		size_t startSearch = 0;
		
		for(size_t i = 0; i < smaller.length(); i++)
		{
			std::pair<size_t, size_t> found = findClosest(smaller[i], startSearch, larger);
			if(found.second < closestDistance)
			{
				closestDistance = found.second;
				closest = std::pair<size_t, size_t>(smaller[i], larger[found.first]);
			}
			
			startSearch = found.first;
		}
		
		return closest;
	}
	
	static std::pair<size_t, size_t> findClosest(const size_t pos, const size_t startSearch, const PositionList &larger)
	{
		size_t closest = startSearch;
		size_t minDistance = distance(pos, larger[startSearch]);
		
		for(size_t i = startSearch + 1; i < larger.length(); i++)
		{
			size_t dist = distance(pos, larger[i]);
			if(dist < minDistance)
			{
				minDistance = dist;
				closest = i;
			}
			else
			{
				break;
			}
		}
		
		return std::pair<size_t, size_t>(closest, minDistance);
	}
	
	std::vector<size_t> _positions;
};

class WordStream
{
public:
	WordStream(const size_t wordCount)
		: _count(wordCount)
	{
	}
	
	const size_t count() const { return _count; }
	
	std::string operator[](const size_t index) const
	{
		std::string word("Word-0x");
		
		char buffer[17];
		snprintf(buffer, 17, "%zx", rand() % (count() / 10));
		word += buffer;
		
		return word;
	}
	
private:
	const size_t _count;
};

const std::pair<size_t, size_t> closest(const std::string &wordA, const std::string &wordB, const Hashtable<std::string, PositionList> table)
{
	return table[wordA].closestPair(table[wordB]);
}

void wordDistance()
{
	srand(time(NULL));
	
	printf("\nWord Distance\n-------------------\n");
	WordStream words(1000);
	Hashtable<std::string, PositionList> table;
	std::string randomWordA;
	std::string randomWordB;
	const size_t randomIndexA = rand() % words.count();
	const size_t randomIndexB = rand() % words.count();
	
	for(size_t i = 0; i < words.count(); i++)
	{		
		std::string word = words[i];
		
		if(i == randomIndexA)
		{
			randomWordA = word;
		}
		else if(i == randomIndexB)
		{
			randomWordB = word;
		}
		
		if(!table.containsKey(word))
		{
			PositionList position;
			position.addPosition(i);
			
			table.put(word, position);
		}
		else
		{
			table.get(word).addPosition(i);
		}
	}
	
	const std::pair<size_t, size_t> best = closest(randomWordA, randomWordB, table);
	
	printf("The minimum distance %zu between %s and %s is found at indices %zu and %zu\n",
	       PositionList::distance(best.first, best.second),
	       randomWordA.c_str(),
	       randomWordB.c_str(),
	       best.first,
	       best.second);
}
