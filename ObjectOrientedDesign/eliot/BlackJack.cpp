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

class MismatchedPlayerException : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Player not associated with this game.";
	}
};

class BlackJack
{
private:
	class PlayerBase
	{
	public:
		PlayerBase(const PlayerID ID, BlackJack *blackJack = nullptr)
			: _ID(ID),
			  _hand(),
			  _blackJack(blackJack)
		{
		}
		
		const PlayerID ID() const { return _ID; }
		const std::vector<Card> Hand() const { return _hand; }
		
		virtual const unsigned int showingTotal() const = 0;
				
		const bool busted() const
		{
			return BlackJack::busted(AllCards());
		}
		
		const bool hasBlackJack() const
		{
			return BlackJack::hasBlackJack(AllCards());
		}
		
		//
		// hit() is the only permissible way to modify a PlayerBase
		// from its public interface.
		//
		void hit()
		{
			_blackJack->hit(*this);
		}
		
	protected:
		friend class BlackJack;
	
		const unsigned int total() const
		{
			return BlackJack::total(AllCards());
		}

		void hitMe(const Card &card)
		{
			_hand.push_back(card);
		}

		virtual const std::vector<Card> AllCards() const = 0;
		virtual void dealt(const Card &card1, const Card &card2) = 0;

		PlayerID _ID;
		std::vector<Card> _hand;
		BlackJack *_blackJack;
	};

public:	
	BlackJack(unsigned int numDecks)
		:_cards(numDecks),
		 _dealer(0, this),
		 _nextID(1)
	{
		_cards.shuffle();
	}
	
	class Dealer : public PlayerBase
	{
	public:
		Dealer(const PlayerID ID, BlackJack *blackJack)
			: PlayerBase(ID, blackJack),
			  _hole()
		{
		}
		
		const unsigned int showingTotal() const
		{
			return BlackJack::total(_hand);
		}
				
	private:
		
		friend class BlackJack;
		
		virtual const std::vector<Card> AllCards() const
		{
			std::vector<Card> cards = _hand;
			cards.push_back(_hole);
			return cards;
		}
		
		virtual void dealt(const Card &card1, const Card &card2)
		{
			_hole = card1;
			_hand.clear();
			_hand.push_back(card2);
		}
		
		Card _hole;
	};
	
	class Player : public PlayerBase
	{
	public:
		Player(const PlayerID ID = 0, BlackJack *blackJack = nullptr)
			: PlayerBase(ID, blackJack)
		{
		}
		
		Player& operator=(const BlackJack::Player &copyFrom)
		{
			_ID = copyFrom.ID();
			_hand = copyFrom.Hand();
			_blackJack = copyFrom._blackJack;
			
			return *this;
		}
		
		const unsigned int showingTotal() const
		{
			return total();
		}

	private:
		friend class BlackJack;
		
		virtual const std::vector<Card> AllCards() const
		{
			return Hand();
		}
				
		virtual void dealt(const Card &card1, const Card &card2)
		{
			_hand.clear();
			_hand.push_back(card1);
			_hand.push_back(card2);
		}
	};
	
	class Table
	{
	public:
		Table(const Dealer *d, const std::vector<Player *> &p)
			: _dealer(d),
			  _players(p)
		{
		}
		
		Table() : _dealer(nullptr), _players() {}
		
		Table(const Table &copy)
			: _dealer(copy._dealer),
			  _players(copy._players)
		{
		}
			
		const Dealer &dealer() const
		{
			return *_dealer;
		}
		
		const size_t countPlayers() const
		{
			return _players.size();
		}
		
		Player &player(size_t index) const
		{
			return *(_players[index]);
		}
		
	private:
		const Dealer *_dealer;
		const std::vector<Player *> _players;
	};
	
	class DealResult
	{
	public:
		DealResult(const Dealer *d, const std::vector<Player *> &p)
			: _ended(false),
			  _table(d, p)
		{
		}
		
		DealResult() : _ended(true), _table() {}
		
		DealResult(const DealResult &copy)
			: _ended(copy._ended),
			  _table(copy._table)
		{
		}
		
		bool playContinues(const Table **ppTable) const
		{
			if(_ended)
			{
				return false;
			}
			else
			{
				*ppTable = &_table;
				return true;
			}
		}
		
	private:
		bool _ended;
		Table _table;
	};
	
	void addPlayer()
	{
		//
		// IDs are 64-bit unsigned integers so we cannot end up in the
		// situation in which we run out of player IDs without first
		// running out of addressable memory.
		//

		Player newPlayer(_nextID++, this);
		_players[newPlayer.ID()] = newPlayer;
	}
	
	// Reference to player is invalid after removePlayer returns true
	void removePlayer(const Player &player)
	{
		_players.erase(player.ID());
	}
	
	DealResult deal()
	{
		std::vector<Player *> players;
		
		// first, dealer gets two cards, one face up
		_dealer.dealt(_cards.dealCard(), _cards.dealCard());

		// next, every player gets two cards, face up
		for(std::pair<const PlayerID, Player> &pairs : _players)
		{
			Player &ref = _players.at(pairs.first);
			ref.dealt(_cards.dealCard(), _cards.dealCard());
			players.push_back(&ref);
		}
		
		DealResult ended;
		// Check the dealer for blackjack before any of the players
		if(_dealer.hasBlackJack())
		{
			return ended;
		}
		else
		{
			// Now check every player for BlackJack
			for(const std::pair<const PlayerID, Player> &pair : _players)
			{
				if(pair.second.hasBlackJack())
				{
					return ended;
				}
			}
		}
		
		DealResult playContinues(&_dealer, players);
		return playContinues;
	}
	
	class GameEnd
	{
	public:
		GameEnd(const std::vector<PlayerBase *> &winners)
			: _winners(winners)
		{
		}
		
		GameEnd() : _winners() {}
		
		const std::vector<PlayerBase *> &winners() const
		{
			return _winners;
		}
		
		virtual void printEnding() const = 0;
		
	protected:
		void push_back(PlayerBase *player)
		{
			_winners.push_back(player);
		}
		
	private:
		std::vector<PlayerBase *> _winners;
	};
	
	class DealerWins : public GameEnd
	{
	public:
		DealerWins(Dealer *dealer)
		{
			push_back(dealer);
		}

		virtual void printEnding() const
		{
			const PlayerBase *dealer = winners()[0];
			if(dealer->hasBlackJack())
			{
				printf("Dealer wins with Black Jack.\n");
			}
			else
			{
				printf("Dealer wins with a total of %u.\n", dealer->total());
			}
		}
	};

	class PlayerWins : public GameEnd
	{
	public:
		PlayerWins(const std::vector<PlayerBase *> winners) : GameEnd(winners) {}

		virtual void printEnding() const
		{
			for(const PlayerBase *player : winners())
			{
				if(player->hasBlackJack())
				{
					printf("Player %lu wins with Black Jack.\n", player->ID());
				}
				else
				{
					printf("Player %lu wins with a total of %u.\n", player->ID(), player->total());
				}
			}
		}
	};
	
	const GameEnd *endGame(const DealResult &dealResult)
	{
		const Table *table = nullptr;
		if(dealResult.playContinues(&table))
		{
			const Dealer &theDealer = table->dealer();
			if(&theDealer != &_dealer)
			{
				throw MismatchedPlayerException();
			}
		}
		
		unsigned int dealerTotal = _dealer.total();
		
		printHand("\nDealer shows\n", _dealer.AllCards());
		printf("Total of %u.\n", dealerTotal);
		
		while(dealerTotal < 16)
		{
			printf("\nDealer hits.\n");
			
			_dealer.hit();
			dealerTotal = _dealer.total();
			
			if(_dealer.busted())
			{
				printHand("Dealer busted!\n", _dealer.AllCards());
			}
			else
			{
				printHand("Dealer is showing\n", _dealer.AllCards());
			}
			
			printf("Total of %u.\n", dealerTotal);
		}
	
		std::vector<PlayerBase *> winners;
		dealerTotal = _dealer.total();
		
		for(std::pair<const PlayerID, Player> &pair : _players)
		{
			Player *player = &pair.second;
			if((_dealer.busted() && !player->busted()) || (dealerTotal < player->total()))
			{
				winners.push_back(player);
			}
		}
	
		if(winners.empty())
		{
			return new DealerWins(&_dealer);
		}
		else
		{
			return new PlayerWins(winners);
		}
	}
	
	static void printHand(const char *const msg, const std::vector<Card> &hand)
	{
		printf("%s", msg);
		for(const Card &card : hand)
		{
			card.Print();
			printf("\n");
		}
	}
	
private:
	static const bool busted(const std::vector<Card> &cards)
	{
		return total(cards) > 21;
	}
			
	static bool hasBlackJack(const std::vector<Card> &cards)
	{
		return (cards.size() == 2) && total(cards) == 21;
	}
	
	void hit(PlayerBase &player)
	{
		if(player.ID() == _dealer.ID())
		{
			_dealer.hitMe(_cards.dealCard());
		}
		else
		{
			_players.at(player.ID()).hitMe(_cards.dealCard());
		}
	}
	
	static const unsigned int total(const std::vector<Card> &hand)
	{
		unsigned int aceCount = 0;
		unsigned int total = 0;
		
		for(const Card &card : hand)
		{	
			if(card.GetFace() == Card::Ace)
			{
				aceCount++;
				total += 11;
			}
			else
			{
				total += card.GetValue();
			}			
		}
		
		// turn aces into ones to avoid going bust
		while(total > 21 && aceCount > 0)
		{
			total -= 10;
			aceCount--;
		}
		
		return total;
	}

	MultiDeckCardGame _cards;
	Dealer _dealer;
	std::unordered_map<PlayerID, Player> _players;
	PlayerID _nextID;
};

void PlayBlackJack()
{
	BlackJack blackJack(10);
	blackJack.addPlayer();
	
	BlackJack::DealResult dealResult = blackJack.deal();
	const BlackJack::Table *table = nullptr;
	
	if(dealResult.playContinues(&table))
	{
		BlackJack::Player &playerOne = table->player(0);	
		const BlackJack::Dealer &dealer = table->dealer();

		BlackJack::printHand("Dealer is showing\n", dealer.Hand());
		
		while(!playerOne.busted())
		{	
			BlackJack::printHand("\nPlayer is showing\n", playerOne.Hand());	
			printf("\nPlayer %s with a total of %u which is %s\n", (playerOne.busted() ? "busted!" : "did not bust"),
																   playerOne.showingTotal(),
																   (playerOne.hasBlackJack() ? "BlackJack!" : "not BlackJack."));
			const unsigned int dealerTotal = dealer.showingTotal();
			const unsigned int playerTotal = playerOne.showingTotal();
			
			if((playerTotal < dealerTotal || dealerTotal == 10) && playerTotal < 17)
			{
				playerOne.hit();
				printf("Player hits!\n");
			}
			else if(playerOne.busted())
			{
				printf("Player busted :-(\n");
				break;
			}
			else
			{
				printf("Player stays.\n");
				break;
			}
		}
		
		if(playerOne.busted())
		{
			printf("Player busted :-(\n");
		}
	}
	
	const BlackJack::GameEnd *end = blackJack.endGame(dealResult);
	end->printEnding();
}
