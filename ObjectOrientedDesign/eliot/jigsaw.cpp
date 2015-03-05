#include <stdio.h>
#include <vector>
#include <list>
#include <functional>

template<typename T>
class ImmutableList
{
public:
	ImmutableList() : _list() {}	
	
	ImmutableList(const T &copy)
		: _list()
	{
		_list.push_back(copy);
	}
	
	ImmutableList(const std::list<T> &elements) : _list(elements) {}
	
	ImmutableList(const ImmutableList<T> &copy)
		: _list(copy._list)
	{
	}
	
	ImmutableList<T> append(const T &element) const
	{
		std::list<T> copy(_list);
		copy.push_back(element);
		return ImmutableList<T>(copy);
	}
	
	ImmutableList<T> remove(const T &element) const
	{
		std::list<T> copy(_list);

		auto it = copy.begin();
		while(it != copy.end() && *it != element)
		{
			it++;
		}

		copy.erase(it);
		
		return ImmutableList<T>(copy);
	}
	
	ImmutableList<T> append(const ImmutableList<T> &other) const
	{
		std::list<T> copy(_list);
		copy.insert(copy.end(), other._list.begin(), other._list.end());
		return ImmutableList<T>(copy);
	}
	
	template<typename Predicate>
	std::pair<ImmutableList<T>, ImmutableList<T> > split(Predicate test) const
	{
		std::list<T> passed;
		std::list<T> failed;
		
		for(const T &value : _list)
		{
			if(test(value))
			{
				passed.push_back(value);
			}
			else
			{
				failed.push_back(value);
			}
		}
		
		return std::pair<ImmutableList<T>, ImmutableList<T> >(ImmutableList<T>(passed), ImmutableList<T>(failed));
	}
	
	template<typename Predicate>
	ImmutableList<T> filter(Predicate test) const
	{
		auto splitLists = split(test);
		return splitLists.first;
	}

	const T &head() const
	{
		return _list.front();
	}
	
	ImmutableList<T> tail() const
	{
		auto it = _list.begin();
		it++;
		std::list<T> copy(it, _list.end());
		return ImmutableList<T>(copy);
	}
	
	const T&back() const
	{
		return _list.back();
	}
	
	const bool isEmpty() const
	{
		return _list.empty();
	}
	
	const size_t length() const
	{
		return _list.size();
	}
	
	const T&operator[](size_t index) const
	{
		auto it = _list.begin();
		
		for(size_t i = 0; i < index; i++)
		{
			it++;
		}
		
		return *it;
	}
	
private:
	std::list<T> _list;
};

template<typename T>
const ImmutableList<T> operator+(const ImmutableList<T> &lhs, const ImmutableList<T> &rhs)
{
	return lhs.append(rhs);
}

template<typename T>
const ImmutableList<T> operator+(const ImmutableList<T> &lhs, const T &element)
{
	return lhs.append(element);
}

template<typename T>
const ImmutableList<T> operator-(const ImmutableList<T> &lhs, const T &element)
{
	return lhs.remove(element);
}

enum Side { Any = 0, Smooth, In, Out };
typedef unsigned char Shape;

class JigsawSide
{
public:
	JigsawSide(Side side, Shape shape = 0) : _side(side), _shape(shape) {}
	JigsawSide(const JigsawSide &other) : _side(other.side()), _shape(other.shape()) {}
	
	const Side side() const { return _side; }
	
	const Shape shape() const { return _shape; }
	
	const JigsawSide opposite() const
	{
		if(_side == Side::Smooth)
		{
			return JigsawSide(Side::Smooth, shape());
		}
		else if(_side == Side::In)
		{
			return JigsawSide(Side::Out, shape());
		}
		else if(_side == Side::Out)
		{
			return JigsawSide(Side::In, shape());
		}
		else
		{
			return JigsawSide(Side::Any, shape());
		}
	}
	
	bool fits(const JigsawSide &other) const
	{
		return *this == other.opposite();
	}
	
	bool operator==(const JigsawSide &other) const
	{
		return side() == other.side() && shape() == other.shape();
	}
	
	bool operator!=(const JigsawSide &other) const
	{
		return !(*this == other);
	}
	
	const char *const toString() const
	{
		if(side() == Side::Smooth)
		{
			return "Smooth";
		}
		else if(side() == Side::In)
		{
			return "In";
		}
		else if(side() == Side::Out)
		{
			return "Out";
		}
		else
		{
			return "Any";
		}
	}
	
private:
	Side _side;
	Shape _shape;
};

typedef unsigned char Label;
class JigsawPiece
{
public:
	JigsawPiece(const Label label,
	            const JigsawSide &left,
				const JigsawSide &right,
				const JigsawSide &top,
				const JigsawSide &bottom)
		: _label(label),
		  _left(left),
		  _right(right),
		  _top(top),
		  _bottom(bottom)
	{
	}
	
	JigsawPiece(const JigsawPiece &copy)
		: _label(copy.label()),
		  _left(copy.left()),
		  _right(copy.right()),
		  _top(copy.top()),
		  _bottom(copy.bottom())
	{
	}
	
	const Label label() const { return _label; }
	
	const JigsawSide &left() const { return _left; }
	const JigsawSide &right() const { return _right; }
	const JigsawSide &top() const { return _top; }
	const JigsawSide &bottom() const { return _bottom; }
	
	bool operator==(const JigsawPiece &other) const
	{
		return label() == other.label() &&
		       left() == other.left() &&
		       right() == other.right() &&
		       top() == other.top() &&
		       bottom() == other.bottom();
	}
	
	bool operator!=(const JigsawPiece &other) const
	{
		return !(*this == other);
	}
	
private:
	Label _label;
	JigsawSide _left;
	JigsawSide _right;
	JigsawSide _top;
	JigsawSide _bottom;
};

typedef ImmutableList<JigsawPiece> JigsawList;

void printJigsawListElements(const JigsawList &list)
{
	if(!list.isEmpty())
	{
		JigsawList tail = list.tail();
		if(tail.isEmpty())
		{
			printf("%u", list.head().label());
		}
		else
		{
			printf("%u ", list.head().label());
			printJigsawListElements(tail);
		}
	}
}

void printJigsawList(const JigsawList &list)
{
	printf("(");
	printJigsawListElements(list);
	printf(")\n");
}

JigsawList makeTopRow(const JigsawPiece &left, const JigsawList rest)
{
	if(rest.isEmpty())
	{
		return rest;
	}
	else
	{
		auto matchesAndOthers = rest.split([left](const JigsawPiece &p){ return p.left().fits(left.right()); });
		if(matchesAndOthers.first.isEmpty())
		{
			return matchesAndOthers.first;
		}
		else
		{
			JigsawList matches = matchesAndOthers.first;
			
			while(!matches.isEmpty())
			{
				JigsawPiece possibility = matches.head();
				matches = matches.tail();
				JigsawList row = makeTopRow(possibility, rest - possibility);
				if(row.length() == rest.length() - 1)
				{
					return JigsawList(possibility) + row;
				}
			}
			
			return JigsawList();
		}
	}
}

class PieceConstraint
{
public:
	PieceConstraint(const JigsawSide &left, const JigsawSide &top)
		: _left(left),
		  _top(top)
	{
		printf("Constraint[%s, %s]\n", left.toString(), top.toString());
	}
	
	const bool operator()(const JigsawPiece &piece) const
	{
		return piece.left() == left() && piece.top() == top();
	}
	
	const JigsawSide &left() const { return _left; }
	const JigsawSide &top() const { return _top; }
	
private:
	JigsawSide _left;
	JigsawSide _top;
};

class JigsawPuzzle
{
public:
	JigsawPuzzle(const JigsawPiece &topLeft)
		: _rows(JigsawList(topLeft)),
		  _constraint(topLeft.right().opposite(), JigsawSide(Side::Smooth))
	{
	}

	JigsawPuzzle(const JigsawPuzzle &copy)
		: _rows(copy._rows),
		  _constraint(copy.constraint())
	{
	}
	
	JigsawPuzzle(const ImmutableList<JigsawList> &rows)
		: _rows(rows),
		  _constraint(constraintFromRows(rows))
	{
	}
	
	JigsawPuzzle()
		: _rows(),
		  _constraint(JigsawSide(Side::Smooth), JigsawSide(Side::Smooth))
	{
	}
	
	const PieceConstraint &constraint() const
	{
		return _constraint;
	}
	
	const JigsawPuzzle operator+(const JigsawPiece &piece) const
	{		
		return JigsawPuzzle(addPiece(_rows, piece));
	}
	
	void print() const
	{
		print(_rows);
	}
	
	const bool isEmpty() const
	{
		return _rows.isEmpty() || _rows.head().isEmpty();
	}
	
	static JigsawPuzzle solve(const JigsawPuzzle &progress, const JigsawList &piecesRemaining)
	{
		printf("Progress\n");
		progress.print();
		printf("Pieces remaining\n");
		printJigsawList(piecesRemaining);
		
		if(piecesRemaining.isEmpty())
		{
			return progress;
		}
		else
		{
			JigsawList possibilities = piecesRemaining.filter(progress.constraint());
			printf("Possibilities = ");
			printJigsawList(possibilities);
			
			const size_t possibilitiesLength = possibilities.length();
			for(size_t i = 0; i < possibilitiesLength; i++)
			{
				printf("Trying piece %u\n", possibilities[i].label());
				JigsawPuzzle solution = solve(progress + possibilities[i], piecesRemaining - possibilities[i]);
				if(!solution.isEmpty())
				{
					return solution;
				}
			}
			
			return JigsawPuzzle();
		}		
	}
	
private:
	static void print(const ImmutableList<JigsawList> rows)
	{
		const size_t numRows = rows.length();
		for(size_t i = 0; i < numRows; i++)
		{
			printJigsawList(rows[i]);
		}
	}
	
	static 	ImmutableList<JigsawList> addPiece(const ImmutableList<JigsawList> &rows, const JigsawPiece &piece)
	{
		if(rows.isEmpty())
		{
			return ImmutableList<JigsawList>(JigsawList(piece));
		}
		else if(rows.head().back().right().side() != Side::Smooth)
		{
			if(rows.head().back().right().fits(piece.left()))
			{
				return ImmutableList<JigsawList>(rows.head() + piece);
			}
			else
			{
				throw "Piece does not fit in this position!";
			}			
		}
		else
		{
			return ImmutableList<JigsawList>(rows.head()) + addPiece(rows.tail(), piece);
		}
	}
	
	static PieceConstraint constraintFromRows(const ImmutableList<JigsawList> &rows)
	{
		if(rows.length() == 0)
		{
			printf("Top left constraint\n");
			// empty puzzle solution implies we are looking for the top left corner piece
			return PieceConstraint(JigsawSide(Side::Smooth), JigsawSide(Side::Smooth));
		}
		else if(rows.back().back().right().side() == Side::Smooth)
		{
			// We are starting a new row
			return PieceConstraint(JigsawSide(Side::Smooth), topConstraint(rows, 0));
		}
		else
		{
			//
			// Constraints come from opposite of bottom row end right side and
			// either Smooth if there is only one row or
			// else opposite of the bottom side of the piece at the same index
			// as the bottom row end in the second to last row.
			//
			 
			JigsawSide left = rows.back().back().right().opposite();
			JigsawSide top = topConstraint(rows, rows.back().length());
			return PieceConstraint(left, top);
		}
	}
	
	static JigsawSide topConstraint(const ImmutableList<JigsawList> &rows,
	                                const size_t bottomRowIndex)
	{
		const size_t numRows = rows.length();
	
		if(numRows == 0)
		{
			return JigsawSide(Side::Smooth);
		}
		else if(rows.back().back().right().side() == Side::Smooth)
		{
			// We are starting a new row
			return rows.back().head().bottom().opposite();
		}
		else if(numRows == 1)
		{
			// Continuing the top row
			return JigsawSide(Side::Smooth);
		}
		else
		{
			// Continuing a row other than the top row
			return rows[numRows - 2][bottomRowIndex].bottom().opposite(); 
		}
	}
	
	ImmutableList<JigsawList> _rows;
	PieceConstraint _constraint;
};

void JigsawPuzzleTest()
{
	/**
	 * ---------------
	 * |   ||   ||   |
	 * | 0 >> 1 << 2 |
	 * |   ||   ||   |
	 * --^ ---v----^--
	 * --^----v----^--
	 * |   ||   ||   |   
	 * | 3 >> 4 >> 5 |
	 * |   ||   ||   |
	 * --v----v----^--
	 * --v----v----^--
	 * |   ||   ||   |
	 * | 6 << 7 << 8 |
	 * |   ||   ||   |
	 * ---------------
	 **/
	 
	std::list<JigsawPiece> piecesList;

	printf("Creating list of pieces\n");
	 
	piecesList.push_back(JigsawPiece(0, JigsawSide(Side::Smooth), JigsawSide(Side::Out), JigsawSide(Side::Smooth), JigsawSide(Side::In)));
	piecesList.push_back(JigsawPiece(1, JigsawSide(Side::In), JigsawSide(Side::In), JigsawSide(Side::Smooth), JigsawSide(Side::Out)));
	piecesList.push_back(JigsawPiece(2, JigsawSide(Side::Out), JigsawSide(Side::Smooth), JigsawSide(Side::Smooth), JigsawSide(Side::In)));
	piecesList.push_back(JigsawPiece(3, JigsawSide(Side::Smooth), JigsawSide(Side::Out), JigsawSide(Side::Out), JigsawSide(Side::Out)));
	piecesList.push_back(JigsawPiece(4, JigsawSide(Side::In), JigsawSide(Side::Out), JigsawSide(Side::In), JigsawSide(Side::Out)));
	piecesList.push_back(JigsawPiece(5, JigsawSide(Side::In), JigsawSide(Side::Smooth), JigsawSide(Side::Out), JigsawSide(Side::In)));
	piecesList.push_back(JigsawPiece(6, JigsawSide(Side::Smooth), JigsawSide(Side::In), JigsawSide(Side::In), JigsawSide(Side::Smooth)));
	piecesList.push_back(JigsawPiece(7, JigsawSide(Side::Out), JigsawSide(Side::In), JigsawSide(Side::In), JigsawSide(Side::Smooth)));
	piecesList.push_back(JigsawPiece(8, JigsawSide(Side::Out), JigsawSide(Side::Smooth), JigsawSide(Side::Out), JigsawSide(Side::Smooth)));
	
	printf("Creating JigsawList of pieces\n");
	JigsawList pieces(piecesList);
	
	printf("Constructing empty puzzle\n");
	JigsawPuzzle emptyPuzzle;
	
	printf("Solving puzzle\n");
	JigsawPuzzle solution = JigsawPuzzle::solve(emptyPuzzle, pieces);
	printf("Done.\n");
	solution.print();
}
