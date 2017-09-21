// Stocks.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

class Key
{
public:
    Key(const unsigned int K, const unsigned int start, const unsigned int end)
        : k(K), start(start), end(end)
    {
    }

    Key(const Key &rhs)
        : k(rhs.K()), start(rhs.Start()), end(rhs.End())
    {
    }

    Key()
        : k(0), start(0), end(0)
    {
    }

    Key &operator=(const Key &rhs)
    {
        k = rhs.K();
        start = rhs.Start();
        end = rhs.End();
        return *this;
    }

    bool operator <(const Key &rhs) const
    {
        if(start < rhs.start)
        {
            return true;
        }
        else if(Start() > rhs.Start())
        {
            return false;
        }
        else
        {
            if(K() < rhs.K())
            {
                return true;
            }
            else if(K() > rhs.K())
            {
                return false;
            }
            else
            {
                if(End() < rhs.End())
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }

    bool operator ==(const Key &rhs) const
    {
        return K() == rhs.K() && Start() == rhs.Start() && End() == rhs.End();
    }

    const unsigned int K() const { return k; }
    const unsigned int Start() const { return start; }
    const unsigned int End() const { return end; }

    class Hasher
    {
    public:
        size_t operator()(const Key &k) const
        {
            //
            // Simple polynomial hash algorithm described by
            // Paul Larson of Microsoft Research.
            // Pick a random number to start with and iteratively
            // hash each value.
            //

            size_t hash = k.K() + 747984492;
            hash += (hash * 101) + k.Start();
            hash += (hash * 101) + k.End();
            return hash;
        }
    };

private:
    unsigned int k;
    unsigned int start;
    unsigned int end;
};

struct Transaction
{
    Transaction(unsigned int buy,
                unsigned int dateBought,
                unsigned int sell,
                unsigned int dateSold)
        : buyPrice(buy),
          buyDate(dateBought),
          sellPrice(sell),
          sellDate(dateSold)
    {
    }

    Transaction(const Transaction &t)
        : buyPrice(t.buyPrice),
          buyDate(t.buyDate),
          sellPrice(t.sellPrice),
          sellDate(t.sellDate)
    {
    }

    bool operator==(const Transaction &t) const
    {
        return buyDate == t.buyDate && sellDate == t.sellDate;
    }

    bool operator<(const Transaction &t) const
    {
        return buyDate < t.buyDate || sellDate < t.sellDate;
    }

    const unsigned int profit() const
    {
        if(buyPrice > sellPrice)
        {
            throw "Selling for less than you bought is a losing strategy!";
        }
        else
        {
            return sellPrice - buyPrice;
        }
    }

    void print() const
    {
        printf("%u [%u] -> %u [%u]\n", buyPrice, buyDate, sellPrice, sellDate);
    }
    
    Transaction &operator=(const Transaction &rhs)
    {
		buyPrice = rhs.buyPrice;
		buyDate = rhs.buyDate;
		sellPrice = rhs.sellPrice;
		sellDate = rhs.sellDate;
		return *this;
    }

    unsigned int buyPrice;
    unsigned int buyDate;
    unsigned int sellPrice;
    unsigned int sellDate;
};

class TransactionList
{
public:
    static TransactionList empty()
    {
        static TransactionList emptyList;
        return emptyList;
    }

    TransactionList() : total(0), transactions() {}

	TransactionList(TransactionList &left, TransactionList &right)
	{
		total = left.total + right.total;
		transactions.splice(transactions.begin(), left.transactions);
		transactions.splice(transactions.begin(), right.transactions);
	}

    TransactionList(const TransactionList &rhs)
    {
        total = rhs.total;
        transactions = rhs.transactions;
    }

    TransactionList &operator=(const TransactionList &rhs)
    {
        total = rhs.total;
        transactions = rhs.transactions;
        return *this;
    }

    TransactionList(TransactionList &&moveMe)
    {
        std::list<Transaction> emptyList;

        total = moveMe.total;
        transactions = moveMe.transactions;

        moveMe.total = 0;
        moveMe.transactions = emptyList;
    }

    void addTransaction(const Transaction &t)
    {
        total += t.profit();
        transactions.push_front(t);
    }

    unsigned long long int profit() const
    {
        return total;
    }

    bool isEmpty() const
    {
        return transactions.empty();
    }

    void print() const
    {
        unsigned int count = 0;
        std::list<Transaction> inorder = transactions;
        inorder.reverse();

        for(Transaction t: inorder)
        {
            t.print();
            count++;
        }
        printf("%u transactions yielding a profit of %llu\n", count, profit());
    }
    
    //
    // After takeOwn() returns other is empty and this has the profit and transactions of other.
    //
    void takeOwn(TransactionList &other)
    {
		total = other.total;
		other.total = 0;
		transactions.clear();
		transactions.splice(transactions.begin(), other.transactions);
	}

private:
    unsigned long long int total;
    std::list<Transaction> transactions;
};

class Memoizer
{
public:
    Memoizer() : _hits(0), _misses(0), _table() {}

    void memoize(unsigned int K,
                 unsigned int start,
                 unsigned int end,
                 const TransactionList transactions)
    {
        Key key(K, start, end);
        _table[key] = transactions;
    }

    const TransactionList get(unsigned int K,
                              unsigned int start,
                              unsigned int end)
    {
        Key key(K, start, end);
        auto it = _table.find(key);
        
        if(it == _table.end())
        {
            ++_misses;
            return TransactionList::empty();
        }
        else
        {
            ++_hits;
            return it->second;
        }
    }

    unsigned long long int Hits() const
    {
        return _hits;
    }

    unsigned long long Misses() const
    {
        return _misses;
    }

private:
    unsigned long long int _hits;
    unsigned long long _misses;
    std::unordered_map<Key, TransactionList, Key::Hasher> _table;
};

TransactionList StocksBaseCase(Memoizer &memoizer,
                               unsigned int *prices,
                               unsigned int countPrices,
                               unsigned int start,
                               unsigned int end)
{
    // Finding a single, optimal transaction is simple --
    //     (1) For each possible buy date find the highest possible subsequent sell price.
    //     (2) Calculate profit.
    //     (3) Return the (buy, sell) pair with highest profit.

    unsigned int best[2] = {0, 0};
    unsigned long long int bestProfit = 0;

    TransactionList transactions = memoizer.get(1, start, end);
    if(!transactions.isEmpty())
    {
        return transactions;
    }

    for(unsigned int buyDate = start; buyDate < countPrices && buyDate < end; buyDate++)
    {
        for(unsigned int sellDate = buyDate + 1; sellDate <= end && sellDate < countPrices; sellDate++)
        {
            if(prices[buyDate] < prices[sellDate])
            {
                unsigned long long int profit = prices[sellDate] - prices[buyDate];
                if(profit > bestProfit)
                {
                    best[0] = buyDate;
                    best[1] = sellDate;
                    bestProfit = profit;
                }
            }
        }
    }

    TransactionList transactionList;
    if(bestProfit > 0)
    {
        Transaction t(prices[best[0]], best[0], prices[best[1]], best[1]);
        transactionList.addTransaction(t);
    }

    return transactionList;
}

TransactionList Stocks(Memoizer &memoizer,
                       unsigned int K,
                       unsigned int *prices,
                       unsigned int countPrices,
                       unsigned int start,
                       unsigned int end)
{
    TransactionList transactions = memoizer.get(K, start, end);
    if(!transactions.isEmpty())
    {
        return transactions;
    }
    else if(K == 1)
    {
        transactions = StocksBaseCase(memoizer, prices, countPrices, start, end);
        memoizer.memoize(1, start, end, transactions);
        return transactions;
    }
    else
    {
        TransactionList highest;

        for(unsigned int split = start + 1; split < countPrices; split++)
        {
            TransactionList left = StocksBaseCase(memoizer, prices, countPrices, start, split);
            memoizer.memoize(1, start, split, left);
            TransactionList right = Stocks(memoizer, K - 1, prices, countPrices, split, end);
            memoizer.memoize(K - 1, split, end, right);
            
            if(left.profit() + right.profit() > highest.profit())
            {
		TransactionList joined(left, right);
                highest.takeOwn(joined);
            }
        }

        return highest;
    }
}

int main(int argc, char *argv[])
{
    /*
    unsigned int prices100[100] = { 
         48,  30,  88,  73, 100,  17,  24,  79,  49,  37,
         21,  14,  11,  62,  66,  56,  40,  81,  57,  71,
         60,  13,  16,  59,   8,  70,  55,  31,   5,  29,
          9,  69,  77,  45,  34,  84,   7,  51,  95,  43,
         96,  50,  44,  75,  52,  83,  68,  67,  20,   2,
         23,  92,  76,  98,  64,  86,  80,  33,  61,  85,
         39,  63,  93,  53,  42,   4,   3,  58,  38,  65,
         19,  89,  47,  46,  41,  82,  26,  78,  32,  72,
         15,  12,  27,  99,  35,  97,   1,  22,  90,  91,
         36,   6,  87,  25,  54,  28,  18,  94,  10,  74
    };
    */

    unsigned int prices365[365] = {
        748, 103, 517, 115, 836, 788, 436, 843, 871, 249,
        991, 131, 943, 139, 678,  98, 429, 895, 611, 993,
        377, 994, 580, 874, 514,  28, 732, 562, 214, 609,
         65, 801, 542, 647, 362, 273,   9, 871, 397, 948,
         22, 328, 623, 912, 843, 771,  25, 422, 667, 930,
        999, 339, 853, 388, 609, 754, 523, 642, 601, 808,
         55, 874, 628, 262, 184, 190, 589, 295, 776, 404,
         88, 558, 498, 662, 970,  49, 370, 668,  30, 742,
          2, 310, 895, 284, 609, 726, 947, 153,  38, 608,
        750, 366, 269, 588, 892, 148, 700, 361, 728, 653,
        182, 730, 645, 262, 842, 441, 840, 191, 418, 560,
        383, 855, 612, 859, 805, 745, 979, 343, 652, 458,
        243, 435, 257, 168, 654, 721, 288, 909, 769, 821,
        349,   9, 940, 376, 559, 271, 664, 632, 442, 168,
        885, 756, 735, 477, 224, 895, 753, 410, 782, 147,
        169, 356, 708, 270, 410, 747, 473, 476, 607, 120,
        153, 931, 421, 596, 210, 374, 915, 235, 299, 533,
         80, 346, 901, 483, 572, 487, 699, 779, 943, 759,
        967, 877, 126, 622, 652, 993, 300, 669, 966, 812,
        798, 187,  24, 321,  15, 954, 334, 138, 267, 850,
        979, 518, 111, 248,  67, 822, 454, 224, 697, 922,
         47, 694, 533, 848, 152, 276, 804, 798, 943, 217,
         74, 245, 269, 486, 735, 892, 321, 579, 468, 618,
        152, 561, 511,  93, 779, 449, 765, 877, 555, 830,
        538,  75, 972,  43, 765, 104, 654, 176, 169, 739,
        149, 536, 195,  50, 775, 734, 248, 624, 513, 174,
        500, 874, 756,  78, 770, 486, 182, 439,  68, 703,
        510, 926, 243, 113, 678, 998, 273, 225, 721, 630,
        237, 145, 410, 729, 469, 616, 366, 461, 317,  45,
        200, 140, 364, 380, 329, 664, 319, 893, 273, 278,
        922, 214, 107, 465, 228, 222, 292,  81, 371, 777,
        461,  23, 903, 882, 937, 355, 463,  26, 609, 154,
        120, 720, 122,  23, 502, 369, 562, 862, 783,  14,
        679, 379, 952, 443, 708,  22,  38, 945, 500, 246,
        323, 284,  89, 540, 583, 177, 237, 481, 443, 974,
        200, 292, 283, 118, 158, 201,  92,  83, 963, 858,
        660, 429, 298, 357,  97
    };

    Memoizer memo;

    /**
     * Because you are not allowed to buy while already holding the stock,    
     * the maximum possible number of transactions is equal to the number
     * of days divided by two and plus one for good measure.
     */

    TransactionList optimal = Stocks(memo,
    /* max number of transactions */ (_countof(prices365) / 2) + 1,
               /* array of prices */ prices365,
                /* number of days */ _countof(prices365),
                         /* start */ 0,
                           /* end */ _countof(prices365) - 1);
    
    optimal.print();
    printf("\nMemomizer hits %llu. misses %llu.\n", memo.Hits(), memo.Misses());
}

