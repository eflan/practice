#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_map>
#include <list>

/**
for(url in URLs)
{
    k = html[url]
    list = duplicates[k]
    duplicates[h] = cons(url, list)
}

for(k in duplicates.keys)
{
    list = duplicates[k]
    print(k, list)
}
*/

#define COUNT_OF_URLs 1000
#define MAX_URL 100
#define COUNT_OF_HTMLs 1000
#define MAX_HTML 100

void createURLandHTML(char urls[COUNT_OF_URLs][MAX_URL],
                      char htmls[COUNT_OF_HTMLs][MAX_HTML])
{
	srand(time(NULL));
	
	for(size_t k = 0; k < COUNT_OF_URLs; k++)
	{
		// Each URL should be unique.
		snprintf(urls[k], MAX_URL, "www.%zd.horse", k);
	}
	
	for(size_t i = 0; i < COUNT_OF_HTMLs; i++)
	{
		// 50% chance of a duplicate
		if(i > 0 && (rand() % 2) == 0)
		{
			// Pick a random HTML that has already been generated and copy it.
			size_t duplicateIndex = rand() % i;
			strncpy(htmls[i], htmls[duplicateIndex], MAX_HTML);
		}
		else
		{
			// Generate a new HTML. No need to be random about the content.
			snprintf(htmls[i], MAX_HTML, "<html> %zd </html>", i);
		}
	}
}

class HTMLKey
{
public:
	HTMLKey()
	{
		memset(_html, sizeof(_html), 0);
	}
	
	HTMLKey(char html[MAX_HTML])
	{
		strncpy(_html, html, MAX_HTML);
	}
	
	HTMLKey(const HTMLKey &rhs)
	{
		strncpy(_html, rhs._html, MAX_HTML);
	}
	
	HTMLKey &operator=(const HTMLKey &rhs)
	{
		strncpy(_html, rhs._html, MAX_HTML);
		return *this;
	}
	
	bool operator==(const HTMLKey &rhs) const
	{
		return 0 == strncmp(_html, rhs._html, MAX_HTML);
	}
	
	bool operator<(const HTMLKey &rhs) const
	{
		return 0 > strncmp(_html, rhs._html, MAX_HTML);
	}
	
	const char *HTML() const
	{
		return _html;
	}
	
	const size_t SizeOfHTML() const
	{
		return MAX_HTML;
	}
	
	class Hasher
	{
	public:
		size_t operator()(const HTMLKey &key) const
		{
			const char *html = key.HTML();
			const size_t htmlSize = key.SizeOfHTML();
			
			size_t hash = html[0] + 747984492;
			
			for(size_t i = 1; i < htmlSize; i++)
			{
				if(html[i] == '\0')
				{
					break;
				}
				else
				{
					hash += (hash * 101) + html[i];
				}
			}
			
			return hash;
		}
	};
	
private:
	char _html[MAX_HTML];
};

int main(int argc, char *argv[])
{	
	char urls[COUNT_OF_URLs][MAX_URL];
	char htmls[COUNT_OF_HTMLs][MAX_HTML];
	
	createURLandHTML(urls, htmls);
	
	std::unordered_map<HTMLKey, std::list<const char *>, HTMLKey::Hasher> table;
	
	/**
	 * for(url in URLs)
     * {
     *     k = html[url]
     *     list = duplicates[k]
     *     duplicates[h] = cons(url, list)
     * }
     */
    
	for(size_t i = 0; i < COUNT_OF_URLs; i++)
	{
		HTMLKey key(htmls[i]);
		std::list<const char *> urlList;

		auto found = table.find(key);
		if(found != table.end())
		{
			urlList = found->second;
		}
		urlList.push_front(urls[i]);
		
		table[key] = urlList;
	}
	
	for(std::pair<const HTMLKey, std::list<const char *> > &val: table)
	{
		printf("(");
		for(const char *url: val.second)
		{
			printf("%s, ", url);
		}
		printf(" empty) maps to %s\n", val.first.HTML());
	}	

    return 0;
}
