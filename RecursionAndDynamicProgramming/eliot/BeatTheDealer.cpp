#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unordered_map>
#include <list>

#define _countof(x) (sizeof(x) / (sizeof(x[0])))

/**
 * Player versus dealer
 *
 * Cards are 0 through 9
 * 
 * Two cards face up, one for each player
 *  -> player decides to hit or not (at most hit once)
 *  -> dealer hits if has lower score, tied or higher does not hit
 * 
 * (card1 + card2) % 10 is how you calculate hand score
 *
 * player has higher score than the dealer +1 point
 * player has lower score than the dealer -1 point
 * tie = 0
 *
 * You know the order of all the cards ahead of time, 100 cards so calculate optimal score
 */
struct PlayerRound
{	
	PlayerRound()
		: hit(false),
		  dealtCard(0),
		  hitCard(0)
	{
	}
	
	PlayerRound(unsigned int dc)
		: hit(false),
		  dealtCard(dc),
		  hitCard(0)
	{
	}

	void Hit(unsigned int h)
	{
		hit = true;
		hitCard = h;
	}
	
	void Print() const
	{
		if(hit)
		{
			printf("%u hits %u = %u %% 10 = %u", dealtCard, hitCard, Hand(), HandValue()); 
		}
		else
		{
			printf("%u stays %% 10 = %u", dealtCard, HandValue()); 
		}
	}
	
	int Hand() const
	{
		return dealtCard + hitCard;
	}
	
	int HandValue() const
	{
		return (dealtCard + hitCard) % 10;
	}
	
	PlayerRound &operator=(const PlayerRound &r)
	{
		dealtCard = r.dealtCard;
		hit = r.hit;
		hitCard = r.hitCard;
		return *this;
	}
	
	bool hit;
	unsigned int dealtCard;
	unsigned int hitCard;
};

class GameRound
{
public:
	GameRound()
		: _player(),
		  _dealer()
	{
	}

	void Play(const PlayerRound &player, const PlayerRound &dealer)
	{
		_player = player;
		_dealer = dealer;
	}
	
	void Print() const
	{
		printf("Player ");
		_player.Print();
		printf("\nDealer ");
		_dealer.Print();
		printf("\nTotal %d.\n", Total());
	}
	
	int Total() const
	{
		return Total(_player, _dealer);
	}
	
	GameRound &operator=(const GameRound &r)
	{
		_player = r._player;
		_dealer = r._dealer;
		return *this;
	}
	
private:
	static int Total(const PlayerRound &player, const PlayerRound &dealer)
	{
		if(player.HandValue() > dealer.HandValue())
		{
			return 1;
		}
		else if(player.HandValue() == dealer.HandValue())
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	
	PlayerRound _player;
	PlayerRound _dealer;
};


class Memoizer
{
public:
	virtual const int Total(const size_t deckSize) const = 0;
	virtual const std::list<GameRound> &Moves(const size_t deckSize) const = 0;
	virtual void Remember(const size_t deckSize, const int total, const std::list<GameRound> &moves) = 0;
	virtual const bool HasTotal(const size_t deckSize) const = 0;
};

class HashMemoizer : public Memoizer
{
public:
	virtual const int Total(const size_t deckSize) const
	{
		return _memoryScores.at(deckSize);
	}
	
	virtual const std::list<GameRound> &Moves(const size_t deckSize) const
	{
		return _memoryMoves.at(deckSize);
	}
	
	virtual void Remember(const size_t deckSize, const int total, const std::list<GameRound> &moves)
	{
		_memoryScores.insert(std::make_pair(deckSize, total));
		_memoryMoves.insert(std::make_pair(deckSize, moves));
	}
	
	virtual const bool HasTotal(const size_t deckSize) const
	{
		return _memoryScores.find(deckSize) != _memoryScores.end();
	}
	
private:
	std::unordered_map<size_t, const int> _memoryScores;
	std::unordered_map<size_t, const std::list<GameRound> > _memoryMoves;
};

int Simulate(bool hit, const size_t deckSize, unsigned int *deck, size_t *pDeckAdjustment, GameRound &round)
{
	PlayerRound player(deck[0]);
	PlayerRound dealer(deck[1]);
	size_t cardsUsed = 2;
	
	if(hit)
	{
		player.Hit(deck[cardsUsed]);
		cardsUsed++;
	}

	if(player.HandValue() > dealer.HandValue())
	{
		dealer.Hit(deck[cardsUsed]);
		cardsUsed++;
	}
	
	*pDeckAdjustment = cardsUsed; 
	
	round.Play(player, dealer);
	
	return round.Total();
}

int BestScore(Memoizer &memo, size_t deckSize, unsigned int *deck, std::list<GameRound> &game)
{
	if(memo.HasTotal(deckSize))
	{
		game = memo.Moves(deckSize);
		return memo.Total(deckSize);
	}
	else if(deckSize < 4)
	{
		return 0;
	}
	else
	{
		int total = 0;
		
		GameRound hitRound;
		std::list<GameRound> gameAfterHit;
		size_t deckAdjustment = 0;
		int hit = Simulate(true, deckSize, deck, &deckAdjustment, hitRound);
		int best = BestScore(memo, deckSize - deckAdjustment, deck + deckAdjustment, gameAfterHit);
		int futureTotalHit = hit + best;
		gameAfterHit.push_front(hitRound);
		
		GameRound noHitRound;
		std::list<GameRound> gameAfterNoHit;
		int noHit = Simulate(false, deckSize, deck, &deckAdjustment, noHitRound);
		best = BestScore(memo, deckSize - deckAdjustment, deck + deckAdjustment, gameAfterNoHit);
		int futureTotalNoHit = noHit + best;
		gameAfterNoHit.push_front(noHitRound);
		
		if(futureTotalHit > futureTotalNoHit)
		{
			game = gameAfterHit;
			total = futureTotalHit;
		}
		else
		{
			game = gameAfterNoHit;
			total = futureTotalNoHit;
		}

		memo.Remember(deckSize, total, game);
		return total;
	}
}

void PlayBeatTheDealer()
{
	std::list<GameRound> game;
	HashMemoizer memo;
	unsigned int cards[] = {9, 5, 7, 3, 9, 2, 1, 8, 3, 2, 7, 1, 5, 6};
	printf("\nBest score is %u.\n", BestScore(memo, _countof(cards), cards, game));
	
	unsigned int roundNum = 0;
	for(const GameRound &r : game)
	{
		printf("\nRound #%u\n", roundNum);
		r.Print();
		roundNum++;
	}
}
