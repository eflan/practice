#include <stdio.h>
#include <inttypes.h>
#include <vector>
#include <unordered_map>
#include <list>
#include <string>

#define _countof(x) (sizeof(x) / sizeof(x[0]))

void PlayBeatTheDealer(const unsigned int deckSize);

const uint64_t countStepCombos(const uint64_t steps, std::vector<uint64_t> &path, std::vector<std::vector<uint64_t> > &accum)
{
	uint64_t total = 0UL;
	
	if(steps == 0UL)
	{
		accum.push_back(path);
		return 1UL;
	}	
	if(steps >= 1UL)
	{
		path.push_back(1UL);
		total += countStepCombos(steps - 1UL, path, accum);
		path.pop_back();
	}
	if(steps >= 2UL)
	{
		path.push_back(2UL);
		total += countStepCombos(steps - 2UL, path, accum);
		path.pop_back();
	}
	if(steps >= 3UL)
	{
		path.push_back(3UL);
		total += countStepCombos(steps - 3UL, path, accum);
		path.pop_back();
	}
	
	return total;
}

const uint64_t countStepCombosMemoized(const uint64_t steps, uint64_t &hits, std::unordered_map<uint64_t, uint64_t> &solutions)
{	
	if(steps == 0UL)
	{
		return 1UL;
	}
	
	auto it = solutions.find(steps);
	if(it != solutions.end())
	{
		hits++;
		return it->second;
	}

	uint64_t total = 0UL;
	uint64_t count = 0;

	if(steps >= 1UL)
	{
		count = countStepCombosMemoized(steps - 1UL, hits, solutions);
		solutions[steps - 1UL] = count;
		total += count;
	}
	if(steps >= 2UL)
	{
		count = countStepCombosMemoized(steps - 2UL, hits, solutions);
		solutions[steps - 2UL] = count;
		total += count;
	}
	if(steps >= 3UL)
	{
		count = countStepCombosMemoized(steps - 3UL, hits, solutions);
		solutions[steps - 3UL] = count;
		total += count;
	}
	
	return total;
}


class Coordinate
{
public:
	Coordinate(size_t x = 0, size_t y = 0) : _x(x), _y(y) {}
	Coordinate(const Coordinate &copy) : _x(copy.X()), _y(copy.Y()) {}
	
	const size_t X() const { return _x; }
	const size_t Y() const { return _y; }
	
private:
	size_t _x;
	size_t _y;
};

typedef std::list<Coordinate> Path;

const bool pathContains(const size_t x, const size_t y, const Path &path)
{
	for(const Coordinate &pos : path)
	{
		if(pos.X() == x && pos.Y() == y)
		{
			return true;
		}
	}
	
	return false;
}

class Offlimits
{
public:
	virtual const bool operator()(const Coordinate &position) const = 0;
};

void printPath(const size_t width, const size_t height, const Path &path, const Offlimits &off)
{
	for(size_t i = 0; i < height; i++)
	{
		for(size_t k = 0; k < width; k++)
		{
			if(pathContains(k, i, path))
			{
				printf("r");
			}
			else if(off(Coordinate(k, i)))
			{
				printf("x");
			}
			else
			{
				printf(".");
			}
		}
		printf("\n");
	}
}

const bool robotLastMoveWasRight(const Path &progress)
{
	const size_t length = progress.size();
	if(length > 1)
	{
		auto it = progress.begin();
		for(size_t i = 0; i < length - 2; i++)
		{
			it++;
		}

		return progress.back().Y() == it->Y() && progress.back().X() == (it->X() + 1);
	}
	else
	{
		return false;
	}
}

Path robotPath(const Offlimits &off, size_t width, size_t height, const Path &progress)
{
	Coordinate position = progress.back();
	if(position.X() == (width - 1) && position.Y() == (height - 1))
	{
		return progress;
	}
	else
	{		
		Coordinate right(position.X() + 1, position.Y());
		Coordinate down(position.X(), position.Y() + 1);
		Coordinate moves[2];

		if(robotLastMoveWasRight(progress))
		{
			moves[0] = down;
			moves[1] = right;
		}
		else
		{
			moves[0] = right;
			moves[1] = down;
		}
		
		for(size_t i = 0; i < 2; i++)
		{
			if(moves[i].X() < width && moves[i].Y() < height && !off(moves[i]))
			{	
				Path go(progress);
				go.push_back(moves[i]);
				Path p = robotPath(off, width, height, go);
				if(!p.empty())
				{
					return p;
				}
			}
		}
				
		return Path();
	}
}

Path robotPath(const Offlimits &off, size_t width, size_t height)
{
	Path start;
	start.push_back(Coordinate(0, 0));
	return robotPath(off, width, height, start);
}

typedef std::vector<std::vector<bool> > RobotMap;

RobotMap makeRobotMap(const size_t width, const size_t height)
{
	RobotMap map;
	for(size_t i = 0; i < height; i++)
	{
		std::vector<bool> row;
		for(size_t k = 0; k < width; k++)
		{
			// 10% impassable terrain except at the top left corner
			row.push_back((!(i == 0 && k == 0)) && (rand() % 10) == 1);
		}
		
		map.push_back(row);
	}
	
	return map;
}

void printMap(const RobotMap &map)
{
	const size_t height = map.size();
	for(size_t i = 0; i < height; i++)
	{
		for(size_t k = 0; k < map[i].size(); k++)
		{
			if(map[i][k])
			{
				printf("1");
			}
			else
			{
				printf("0");
			}
		}
		printf("\n");
	}
}

class OfflimitsMap : public Offlimits
{
public:
	OfflimitsMap(const RobotMap &map)
		: _impassable(map)
	{
	}
	
	const bool operator()(const Coordinate &position) const
	{
		return _impassable[position.Y()][position.X()];
	}
	
	void print() const
	{
		printMap(_impassable);
	}
	
private:
	RobotMap _impassable;
};

std::list<std::string> parentheses(const size_t n, const size_t openCount, const size_t closedCount, const std::string &progress)
{
	if(closedCount == n)
	{
		std::list<std::string> empty;
		empty.push_back(progress);
		return empty;
	}
	
	std::list<std::string> accum;
	if(openCount < n)
	{
		std::string step(progress);
		step += "(";
		std::list<std::string> combos = parentheses(n, openCount + 1, closedCount, step);
		accum.insert(accum.end(), combos.begin(), combos.end());
	}
	
	if(closedCount < openCount)
	{
		std::string step(progress);
		step += ")";
		std::list<std::string> combos = parentheses(n, openCount, closedCount + 1, step);
		accum.insert(accum.end(), combos.begin(), combos.end());
	}
	
	return accum;
}

std::list<std::string> parentheses(const size_t n)
{
	return parentheses(n, 0, 0, std::string());
}

size_t magicIndex(size_t i, size_t n, size_t *sorted)
{
	if(i >= n)
	{
		return i;
	}
	else if(sorted[i] == i)
	{
		return i;
	}
	else
	{
		if(sorted[i] > i)
		{
			return magicIndex(sorted[i], n, sorted);
		}
		else
		{
			return magicIndex(i + 1, n, sorted);
		}
	}
}

size_t magicIndex(size_t n, size_t *sorted)
{
	return magicIndex(0, n, sorted);
}

template<typename T>
class Set
{
public:
	Set()
		: _members()
	{
	}
	
	Set(const T &unary)
		: _members()
	{
		_members.push_back(unary);
	}
	
	Set(const Set<T> &copy)
		: _members(copy.members())
	{
	}
	
	Set(Set<T> &&move)
		: _members()
	{
		move._members.swap(_members);
	}
	
	Set(const std::vector<T> &members)
		: _members(members)
	{
	}
	
	const std::vector<T> &members() const
	{
		return _members;
	}
	
	const Set<T> operator+(const Set<T> &rhs) const
	{
		std::vector<T> all(members());
		all.insert(all.begin(), rhs.members().begin(), rhs.members().end());
		return Set<T>(all);
	}
	
	const Set<T> operator+(const T &rhs) const
	{
		std::vector<T> all(members());
		all.push_back(rhs);
		return Set<T>(all);
	}
	
	const Set<T> operator-(const T&rhs) const
	{
		std::vector<T> all(members());
	
		auto it = all.begin();
		while(it != all.end())
		{
			if(*it == rhs)
			{
				break;
			}
			else
			{
				it++;
			}
		}
		
		all.erase(it);
		
		return Set<T>(all);
	}
	
	const size_t size() const
	{
		return _members.size();
	}
	
	const Set<T> operator=(const Set<T> &rhs)
	{
		_members = rhs.members();
		return *this;
	}
	
private:
	std::vector<T> _members;
};

template<typename T>
Set<Set<T> > subsets(const size_t subsetSize, const Set<T> &set)
{
	Set<Set<T> > sets;
	if(subsetSize == 1)
	{
		for(size_t element : set.members())
		{			
			sets = sets + Set<T>(element);
		}
		
		return sets;
	}
	else if(subsetSize == set.size())
	{
		return set;
	}
	else
	{
		for(size_t element : set.members())
		{
			Set<Set<T> > nMinusOne = subsets(subsetSize - 1, set - element);
			for(const Set<T> &subset : nMinusOne.members())
			{
				Set<T> n = subset + element;
				sets = sets + n;
			}
		}
		
		return sets;
	}
}

template<typename T>
Set<Set<T> > subsets(const Set<T> &set)
{
	Set<Set<T> > sets;
	
	for(size_t size = 1; size <= set.size(); size++)
	{
		sets = sets + subsets(size, set);
	}
	
	return sets;
}

std::vector<std::string> permutations(const std::string &str)
{
	std::vector<std::string> perms;

	if(str.size() == 0)
	{
		// Permutations is an empty set
	}
	else if(str.size() == 1)
	{
		perms.push_back(str);
	}
	else
	{
		for(size_t i = 0; i < str.length(); i++)
		{
			std::string rest(str);
			rest.erase(i, 1);
			std::vector<std::string> nMinusOne = permutations(rest);
			
			for(std::string &substr : nMinusOne)
			{
				std::string perm;
				perm += str[i];
				perm += substr;
				perms.push_back(perm);
			}
		}
	}
	
	return perms;
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	
	PlayBeatTheDealer(100);
	
	std::vector<uint64_t> path;
	std::vector<std::vector<uint64_t> >solutions;
	printf("\n\n%llu cobminations for %lu steps\n", countStepCombos(5UL, path, solutions), 5UL);
	
	for(std::vector<uint64_t> &path : solutions)
	{
		printf("(");
		for(uint64_t step : path)
		{
			printf("%llu ", step);
		}
		printf(")\n");
	}
	
	uint64_t hits = 0;
	std::unordered_map<uint64_t, uint64_t> memoizer;
	const uint64_t solutionCount = countStepCombosMemoized(100UL, hits, memoizer);
	printf("\n\n%llu cobminations for %lu steps (%llu memoizer hits)\n", solutionCount, 100UL, hits);
	
	OfflimitsMap offLimits(makeRobotMap(10, 10));
	printf("\nRobot path = \n");
	Path robot = robotPath(offLimits, 10, 10);
	printPath(10, 10, robot, offLimits);
	
	printf("\nAll combinations of 3 pairs of parentheses -- ");
	std::list<std::string> combos = parentheses(3);
	for(std::string &str : combos)
	{
		printf("%s, ", str.c_str());
	}
	printf("\n");
	
	for(size_t size = 0; size < 11; size++)
	{
		combos = parentheses(size);
		printf("%zu parentheses have %zu balanced combinations.\n", size, combos.size());
	}
	
	size_t numbers[] = {1, 2, 4, 11, 12, 13, 14, 14, 14, 14, 14, 14, 14, 14, 14 };
	size_t magic = magicIndex(_countof(numbers), numbers);
	if(magic < _countof(numbers))
	{
		printf("\nnumbers[%zu] == %zu\n", magic, numbers[magic]);
	}
	else
	{
		printf("\nArray does not contain a magic index :-(\n");
	}
	
	size_t noMagic[] = {1, 2, 4, 11, 12, 13, 14, 14, 14, 14, 14, 14, 14, 14 };
	magic = magicIndex(_countof(noMagic), noMagic);
	if(magic < _countof(noMagic))
	{
		printf("\nArray should not have contained a magic index!\n");
	}
	
	std::vector<size_t> five;
	five.push_back(0);
	five.push_back(1);
	five.push_back(2);
	five.push_back(3);
	five.push_back(4);
	Set<size_t> setOfFive(five);
	Set<Set<size_t> > subs = subsets(setOfFive);
	printf("\nSubsets of ");
	printf("(");
	for(const size_t &element : five)
	{
		printf("%zu, ", element);
	}
	printf(") --\n\n");
	
	for(const Set<size_t> &set : subs.members())
	{
		printf("(");
		for(const size_t &element : set.members())
		{
			printf("%zu, ", element);
		}
		printf(")\n");
	}
	
	std::string str("abcd");
	printf("\nPermutations of %s --\n", str.c_str());
	std::vector<std::string> perms = permutations(str);
	for(const std::string &perm : perms)
	{
		printf("%s\n", perm.c_str());
	}
	printf("\n");
	
	return 0;
}
