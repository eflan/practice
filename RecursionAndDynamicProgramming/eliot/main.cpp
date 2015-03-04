#include <stdio.h>
#include <inttypes.h>
#include <vector>
#include <unordered_map>

void PlayBeatTheDealer();

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

int main(int argc, char *argv[])
{
	PlayBeatTheDealer();
	
	std::vector<uint64_t> path;
	std::vector<std::vector<uint64_t> >solutions;
	printf("\n\n%lu cobminations for %lu steps\n", countStepCombos(5UL, path, solutions), 5UL);
	
	for(std::vector<uint64_t> &path : solutions)
	{
		printf("(");
		for(uint64_t step : path)
		{
			printf("%lu ", step);
		}
		printf(")\n");
	}
	
	uint64_t hits = 0;
	std::unordered_map<uint64_t, uint64_t> memoizer;
	const uint64_t solutionCount = countStepCombosMemoized(100UL, hits, memoizer);
	printf("\n\n%lu cobminations for %lu steps (%lu memoizer hits)\n", solutionCount, 100UL, hits);
	
	return 0;
}
