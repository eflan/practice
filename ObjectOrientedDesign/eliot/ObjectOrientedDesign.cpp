#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <unordered_map>

class Card
{
public:
	enum Suit{ None = 0, Hearts, Clubs, Diamonds, Spades };
	enum Face { NoFace = 0, Jack, Queen, King, Ace };

	Card() : _value(0), _face(NoFace), _suit(None) {}
	Card(const unsigned int value, const Face face, const Suit suit) : _value(value), _face(face), _suit(suit) {}
	Card(const Card &copy) : _value(copy._value), _face(copy._face), _suit(copy._suit) {}
	
	const Card &operator=(const Card &copy)
	{
		_value = copy._value;
		_face = copy._face;
		_suit = copy._suit;
		return *this;
	}
	
	const Face GetFace() const { return _face; }
	const Suit GetSuit() const { return _suit; }
	const unsigned int GetValue() const { return _value; }
	
	void Print() const
	{
		if(GetFace() == NoFace)
		{
			printf("%u of %s", _value, SuitToString(_suit));
		}
		else
		{
			printf("%s of %s", FaceToString(_face), SuitToString(_suit));
		}
	}
	
private:
	static char const *SuitToString(const Suit suit)
	{
		switch(suit)
		{
			case Hearts:
				return "Hearts";
			case Diamonds:
				return "Diamonds";
			case Clubs:
				return "Clubs";
			case Spades:
				return "Spades";
			default:
				return "Uninitialized suit";
		}
	}
	
	static char const *FaceToString(const Face face)
	{
		switch(face)
		{
			case Jack:
				return "Jack";
			case Queen:
				return "Queen";
			case King:
				return "King";
			case Ace:
				return "Ace";
			default:
				return "Uninitialized card face";
		}
	}
	
	unsigned int _value;
	Face _face;
	Suit _suit;
};

class CardDeck
{
public:
	static const unsigned int NumCards = 52;
	
	CardDeck() : _top(0)
	{
		CreateSuit(Card::Hearts, (NumCards / 4), _cards);
		CreateSuit(Card::Clubs, (NumCards / 4), _cards + (NumCards / 4));
		CreateSuit(Card::Diamonds, (NumCards / 4), _cards + (2 * (NumCards / 4)));
		CreateSuit(Card::Spades, (NumCards / 4), _cards + (3 * (NumCards / 4)));	
	}

	CardDeck(const CardDeck &copyDeck)
	{
		copy(copyDeck);
	}
	
	const CardDeck &operator=(const CardDeck &copyDeck)
	{
		copy(copyDeck);
		return *this;
	}
	
	void copy(const CardDeck &copyDeck)
	{
		_top = copyDeck._top;
		
		for(unsigned int i = 0; i < NumCards; i++)
		{
			_cards[i] = copyDeck._cards[i];
		}
	}
	
	void shuffle()
	{
		_top = 0;
		srand(time(NULL));
		
		// Swap each card with another card at a random position.
		for(unsigned int i = 0; i < NumCards; i++)
		{
			unsigned int swapIndex = rand() % NumCards;
			Card swapped = _cards[i];
			_cards[i].Print();
			_cards[i] = _cards[swapIndex];
			_cards[swapIndex] = swapped;
		}
	}
	
	const Card swapcard(const unsigned int index, const Card &card)
	{
		Card swapped = _cards[index];
		_cards[index] = card;
		return swapped;
	}
	
	const Card &peek(const unsigned int index) const
	{
		return _cards[index];
	}
	
	const Card &deal()
	{
		return _cards[_top++];
	}
	
	const bool needsToShuffle() const
	{
		return _top + 1 == NumCards;
	}
	
	void print() const
	{
		for(const Card &card : _cards)
		{
			printf("    ");
			card.Print();		
			printf("\n");
		}
	}

private:
	static void CreateSuit(const Card::Suit suit, const size_t sizeCards, Card *cards)
	{
		if(sizeCards != (NumCards / 4) || (NumCards / 4) != 13)
		{
			throw "Invalid size for one quarter of the deck!";
		}
		
		for(size_t i = 2; i <= 10; i++)
		{
			Card card(i, Card::NoFace, suit);
			cards[i - 2] = card;
		}
		
		Card jack(10, Card::Jack, suit);
		Card queen(10, Card::Queen, suit);
		Card king(10, Card::King, suit);
		Card ace(10, Card::Ace, suit);
		
		cards[9] = jack; 
		cards[10] = queen;
		cards[11] = king;
		cards[12] = ace;
	}
	
	unsigned int _top;
	Card _cards[NumCards];
};

class MultiDeckCardGame
{
public:
	MultiDeckCardGame(unsigned int numberOfDecks) : _topDeck(0)
	{
		for(unsigned int i = 0; i < numberOfDecks; i++)
		{
			CardDeck deck;
			_decks.push_back(deck);
		}
	}
	
	void shuffle()
	{
		_topDeck = 0;
		srand(time(NULL));
	
		// iterate through every card, in every deck, and swap it with
		// another random card in another random deck
		for(CardDeck &deck : _decks)
		{
			for(unsigned int i = 0; i < CardDeck::NumCards; i++)
			{
				Card swapped = _decks[rand() % _decks.size()].swapcard((rand() % CardDeck::NumCards), deck.peek(i));
				deck.swapcard(i, swapped);
			}
		}
	}
	
	const Card &dealCard()
	{
		if(_decks[_topDeck].needsToShuffle())
		{
			++_topDeck;
		}
		
		return _decks[_topDeck].deal();
	}
	
	const bool needsToShuffle() const
	{
		return (_topDeck == _decks.size()) && _decks[_topDeck].needsToShuffle();
	}
	
	void print() const
	{
		printf("Decks in the game:\n");
		for(const CardDeck & deck : _decks)
		{
			printf("Deck\n");
			deck.print();
			printf("\n");
		}
	}

private:
	unsigned int _topDeck;
	std::vector<CardDeck> _decks;
};

typedef uint64_t PlayerID;
	
class BlackJack
{
public:
	BlackJack(unsigned int numDecks)
		:_cards(numDecks),
		 _dealer(*this),
		 _nextID(0)
	{
		_cards.shuffle();
	}
	
	class Dealer
	{
	public:
		Dealer(BlackJack &blackJack) : _blackJack(blackJack) {}
		
		std::vector<Card> hand;
		
		const unsigned int total() const
		{
			return _blackJack.total(*this);
		}
		
		const bool busted() const
		{
			return _blackJack.busted(*this);
		}
		
		void hit() const
		{
			_blackJack.hit(*this);
		}
		
	private:
		friend class BlackJack;
		Card hole;
		BlackJack &_blackJack;
	};
	
	class Player
	{
	public:
		Player(BlackJack *blackJack = nullptr)
			: _blackJack(blackJack)
		{
		}
		
		PlayerID ID;
		std::vector<Card> hand;
		
		const unsigned int total() const
		{
			return _blackJack->total(*this);
		}

		const bool busted() const
		{
			return _blackJack->busted(*this);
		}
		
		const bool hasBlackJack() const
		{
			return _blackJack->hasBlackJack(*this);
		}
		
		void hit() const
		{
			_blackJack->hit(*this);
		}
		
		Player& operator=(const BlackJack::Player &copyFrom)
		{
			ID = copyFrom.ID;
			hand = copyFrom.hand;
			
			if(_blackJack == nullptr)
			{
				_blackJack = copyFrom._blackJack;
			}
			else if(_blackJack != copyFrom._blackJack)
			{
				throw "Mixing players from different BlackJack games is unsupported!";
			}
			
			return *this;
		}

	private:
		BlackJack *_blackJack;
	};
	
	const Dealer &dealer() const
	{
		return _dealer;
	}
	
	const Player &addPlayer()
	{
		Player newPlayer(this);
		newPlayer.ID = _nextID;
		_nextID++;
		
		_players[newPlayer.ID] = newPlayer;
		return _players.at(newPlayer.ID);
	}
	
	// Reference to player is invalid after removePlayer returns true
	void removePlayer(const Player &player)
	{
		_players.erase(player.ID);
	}
	
	void deal()
	{
		// first, dealer gets two cards, one face up
		_dealer.hole = _cards.dealCard();
		_dealer.hand.clear();
		_dealer.hand.push_back(_cards.dealCard());
		
		// next, every player gets two cards, face up
		for(std::pair<const PlayerID, Player> &pairs : _players)
		{
			Player &ref = _players.at(pairs.first);
			ref.hand.clear();
			ref.hand.push_back(_cards.dealCard());
			ref.hand.push_back(_cards.dealCard());
		}
	}
	
	const bool busted(const Player &player) const
	{
		return total(player) > 21;		
	}
	
	const bool busted(const Dealer &dealer) const
	{
		return total(dealer) > 21;
	}
	
	bool hasBlackJack(const Player &player) const
	{
		return (_players.at(player.ID).hand.size() == 2) && total(player) == 21;
	}
	
	const unsigned int total(const Dealer &dealer) const
	{
		// there is only one dealer so nothing to look up
		std::vector<Card> handPlusHole = _dealer.hand;
		handPlusHole.push_back(_dealer.hole);
		return total(handPlusHole);
	}
		
	const unsigned int total(const Player &player) const
	{
		return total(_players.at(player.ID).hand);
	}
	
	void hit(const Player &player)
	{
		_players.at(player.ID).hand.push_back(_cards.dealCard());
	}
	
	void hit(const Dealer &dealer)
	{
		// there's only one dealer so no need to look anything up
		_dealer.hand.push_back(_cards.dealCard());
	}
	
private:
	const unsigned int total(const std::vector<Card> &hand) const
	{
		unsigned int total = 0;
		
		for(const Card &card : hand)
		{
			unsigned int aceCount = 0;
			
			if(card.GetFace() != Card::Ace)
			{
				total += card.GetValue();
			}
			else
			{
				aceCount++;
				total += 11;
			}
			
			// turn aces into ones to avoid going bust
			while(total > 21 && aceCount > 0)
			{
				total -= 10;
				aceCount--;
			}
		}
		
		return total;
	}
		
private:
	MultiDeckCardGame _cards;
	Dealer _dealer;
	std::unordered_map<PlayerID, Player> _players;
	PlayerID _nextID;
};

int main(int argc, char *argv[])
{
	BlackJack blackJack(10);
	const BlackJack::Player &playerOne = blackJack.addPlayer();
	blackJack.deal();

	printf("ID = %" PRIu64 "\n", playerOne.ID);
	printf("Total is %u\n", playerOne.total());
	
	const BlackJack::Dealer &dealer = blackJack.dealer();
	
	while(!playerOne.busted())
	{
		printf("Dealer is showing\n");
		for(const Card &card : dealer.hand)
		{
			card.Print();
			printf("\n");
		}
		printf("\n");
		
		printf("\nPlayer is showing\n");
		for(const Card &card : playerOne.hand)
		{
			card.Print();
			printf("\n");
		}
		printf("for a total of %u.\n", playerOne.total());
		
		printf("\nPlayer %s with a total of %u which is %s\n", (playerOne.busted() ? "busted!" : "did not bust"),
															   playerOne.total(),
															   (playerOne.hasBlackJack() ? "BlackJack!" : "not BlackJack."));
		if(playerOne.total() < 17)
		{
			playerOne.hit();
			printf("Player hits!\n");
		}
		else if(playerOne.busted())
		{
			printf("Player busted!\n");
			break;
		}
		else
		{
			printf("Player stays.\n");
			break;
		}
	}
	
	if(!playerOne.busted())
	{	
		while(dealer.total() < 16)
		{
			dealer.hit();
			printf("Dealer hits!\n");
			
			printf("Dealer is showing\n");
			for(const Card &card : dealer.hand)
			{
				card.Print();
				printf("\n");
			}
			printf("\n");
		}
		
		if(dealer.busted())
		{
			printf("Dealer busted! Player wins!\n");
		}
		else
		{
			const unsigned int dealerTotal = dealer.total();
			const unsigned int playerTotal = playerOne.total();
			
			if(dealerTotal > playerTotal)
			{
				printf("Player total of %u loses to dealer total of %u.\n", playerTotal, dealerTotal);
			}
			else if(dealerTotal == playerTotal)
			{
				printf("Player and dealer push with equal totals of %u.\n", playerTotal);
			}
			else
			{
				printf("Player wins with a total of %u versus dealer's total of %u!\n", playerTotal, dealerTotal);
			}
		}		
	}
	else
	{
		printf("Player loses :-(\n");
	}
		
	return 0;
}
