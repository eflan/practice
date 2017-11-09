#include <stdexcept>
#include <stdio.h>
#include <vector>
#include <list>
#include <string>

template<typename K>
class DefaultHash
{
public:
	static const size_t hash(const K &key)
	{
		const unsigned char *memory = reinterpret_cast<const unsigned char *>(&key);
		
		if(sizeof(K) >= 8)
		{
			return *(reinterpret_cast<const size_t *>(&key));
		}
		else
		{
			size_t h = memory[0];
			for(size_t i = 1; i < sizeof(key); i++)
			{
				h <<= 8;
				h |= memory[i];
			}
			return h;
		}
	}
};

template<typename K, typename V, class Hash = DefaultHash<K> >
class Hashtable
{
public:
	Hashtable(const size_t numBuckets = 100,
	          const size_t loadFactor = 2)
		: _loadFactor(loadFactor),
		  _count(0),
		  _buckets()
	{
		_buckets.reserve(numBuckets);
	}
	
	const V &get(const K &key) const
	{
		return _buckets[bucketIndex(key)].get(key);		
	}
	
	void put(const K &key, const V &value)
	{
		growIfNeeded();
		_buckets[bucketIndex(key)].put(key, value);
		_count++;
	}
	
	const size_t worstChain() const
	{
		size_t chain = 0;
		
		for(const Bucket &bucket : _buckets)
		{
			if(bucket.chainLength() > chain)
			{
				chain = bucket.chainLength();
			}			
		}
		
		return chain;
	}
	
private:
	class Bucket
	{
	public:
		Bucket() : _list()
		{
			_list.reserve(10);
		}
		
		const V&get(const K &key) const
		{
			for(size_t i = 0; i < _list.size(); i++)
			{
				if(_list[i].first == key)
				{
					return _list[i].second;
				}
			}
			
			throw std::out_of_range(std::string("Key is not mapped."));
		}
		
		void put(const K &key, const V &value)
		{
			_list.push_back(std::pair<K, V>(key, value));
		}
		
		const std::vector<std::pair<K, V> > &contents() const
		{
			return _list;
		}
		
		const size_t chainLength() const
		{
			return _list.size();
		}
		
	private:
		std::vector<std::pair<K, V> > _list;
	};
	
	static const size_t hash(const K &key)
	{
		return Hash::hash(key);
	}
	
	const size_t bucketIndex(const K &key) const
	{
		return hash(key) % _buckets.capacity();
	}
	
	void growIfNeeded()
	{
		if(_count >= (_buckets.size() / LoadFactor()))
		{
			grow();
		}
	}
	
	void grow()
	{
		std::vector<Bucket> oldBuckets(_buckets.capacity() * LoadFactor());
		_buckets.swap(oldBuckets);		
		
		const size_t bucketCount = oldBuckets.size();
		for(size_t i = 0; i < bucketCount; i++)
		{
			const std::vector<std::pair<K, V> > &nodes = oldBuckets[i].contents();
			const size_t nodeCount = nodes.size();
			for(size_t k = 0; k < nodeCount; k++)
			{
				const std::pair<K, V> &node = nodes[k];
				_buckets[bucketIndex(node.first)].put(node.first, node.second);
			}
		}
	}
	
	const size_t LoadFactor() const
	{
		return _loadFactor;
	}
	
	size_t _loadFactor;
	size_t _count;
	std::vector<Bucket > _buckets;
};

void HashtableTest()
{
	const size_t TestSize = 2000000;
	Hashtable<size_t, std::string> defaultTable;

	char buffer[7];
	std::string str;
	for(size_t i = 0; i < TestSize; i++)
	{
		sprintf(buffer, "%.6zu", i);
		str = buffer;
		defaultTable.put(i, str);
	}

	printf("Worst chain length %zu\n", defaultTable.worstChain());

	for(unsigned int i = 0; i < 10; i++)
	{
		const size_t index = rand() % TestSize;
		printf("%zu -> \"%s\"\n", index, defaultTable.get(index).c_str());
	}

	for(size_t i = 0; i < TestSize; i++)
	{
		try
		{
			size_t val = strtoul(defaultTable.get(i).c_str(), NULL, 10);
			if(val != i)
			{
				printf("Expected %zu -> %zu, but %zu -> %zu instead!\n", i, i, i, val);
			}
		}
		catch(std::out_of_range &oor)
		{
			printf("Key %zu is not mapped!\n", i);
		}
	}
}
