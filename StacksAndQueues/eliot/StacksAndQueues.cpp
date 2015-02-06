#include <string.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <memory>

class Stack
{
public:
	virtual unsigned int pop() = 0;
	virtual void push(const unsigned int &value) = 0;
	virtual const unsigned int &peek() const = 0;
	virtual bool isEmpty() const = 0;
};

class ThreeStacks
{
public:
	ThreeStacks()
		: _stack1(_array, 100),
		  _stack2(_array + 100, 100),
		  _stack3(_array + 200, 100)
	{
	}

	Stack *Stack1()
	{
		return &_stack1;
	}
	
	Stack *Stack2()
	{
		return &_stack2;
	}
	
	Stack *Stack3()
	{
		return &_stack3;
	}

private:
	class ArrayStack : public Stack
	{
	public:
		ArrayStack(unsigned int *array, size_t size)
			: _array(array),
			  _size(size),
			  _count(0)
		{
		}
		
		virtual unsigned int pop()
		{
			if(isEmpty())
			{
				throw "Cannot pop from an empty stack!";
			}
			else
			{
				unsigned int value = peek();
				_count--;
				return value;
			}
		}
		
		virtual void push(const unsigned int &value)
		{
			if(_count == _size)
			{
				throw "Cannot push onto a full stack!";
			}
			else
			{
				_array[_count] = value;
				_count++;
			}
		}
		
		virtual const unsigned int& peek() const
		{
			if(isEmpty())
			{
				throw "Cannot peek at an empty stack!";
			}
			else
			{
				return _array[_count - 1];
			}
		}
		
		virtual bool isEmpty() const
		{
			return _count == 0;
		}
		
	private:
		unsigned *_array;
		const size_t _size;
		size_t _count;
	};
	
	ArrayStack _stack1;
	ArrayStack _stack2;
	ArrayStack _stack3;
	
	unsigned int _array[300];
};

bool TestStack(Stack *stack)
{
	if(!stack->isEmpty())
	{
		printf("Stack is not empty at beginning of test!\n");
		return false;
	}
	
	for(unsigned int i = 0; i < 100; i++)
	{
		stack->push(i);
	}
	
	if(stack->peek() != 99)
	{
		printf("peek() = %u but expected %u!\n", stack->peek(), 99);
		return false;
	}
	
	for(unsigned int i = 0; i < 100; i++)
	{
		unsigned int popped = stack->pop();
		if(99 - i != popped)
		{
			printf("Popped %u but expected %u!\n", popped, 100 - i);
			return false;
		}
	}
	
	if(!stack->isEmpty())
	{
		printf("Stack is not empty after popping all elements!\n");
		return false;
	}
	
	return true;
}

class MinStack : public Stack
{
public:
	virtual unsigned int pop()
	{
		unsigned int top = _stack.front();
		_stack.pop_front();
		_minimums.pop_front();
		return top;
	}
	
	virtual void push(const unsigned int &value)
	{
		unsigned int lower = value;
		if(!_minimums.empty() && lower > _minimums.front())
		{
			lower = _minimums.front();
		}
		
		_stack.push_front(value);
		_minimums.push_front(lower);
	}
	
	virtual const unsigned int &peek() const
	{
		return _stack.front();
	}
	
	virtual bool isEmpty() const
	{
		return _stack.empty();
	}
	
	unsigned int min() const
	{
		return _minimums.front();
	}
	
private:
	std::list<unsigned int> _stack;
	std::list<unsigned int> _minimums;
};

bool TestMinStack(MinStack *minStack)
{
	unsigned int values[25]   = {100, 99, 15, 77, 32, 38, 20, 18, 58, 30,
                                  92, 10, 83, 13, 21, 70, 28, 96, 44, 52,
                                  55, 79, 39, 94, 54 };

	unsigned int minimums[25] = {100, 99, 15, 15, 15, 15, 15, 15, 15, 15,
		                          15, 10, 10, 10, 10, 10, 10, 10, 10, 10,
		                          10, 10, 10, 10, 10};
	
	for(size_t i = 0; i < 25; i++)
	{
		minStack->push(values[i]);
	
		if(minStack->min() != minimums[i])
		{
			printf("After pushing element $%zu = %u stack reported minimum of %u but we expected %u!\n", i, values[i], minStack->min(), minimums[i]);
			return false;
		}
	}
	
	for(size_t i = 0; i < 25; i++)
	{
		if(minStack->min() != minimums[24 - i])
		{
			printf("Before popping element #%zu minimum reported was %u but expected %u!\n", i, minStack->min(), minimums[24 - i]);
			return false;
		}

		unsigned int popped = minStack->pop();
		if(popped != values[24 - i])
		{
			printf("Popped element #%zu = %u but expected %u!\n", i, popped, values[24 - i]);
			return false;
		}		
	}
	
	return true;
}

int main(int argc, char *argv[])
{
	ThreeStacks threeStacks;

	printf("Stack1 passed? %s.\n", TestStack(threeStacks.Stack1()) ? ("Yes") : ("No"));
	printf("Stack2 passed? %s.\n", TestStack(threeStacks.Stack2()) ? ("Yes") : ("No"));
	printf("Stack3 passed? %s.\n", TestStack(threeStacks.Stack3()) ? ("Yes") : ("No"));

	MinStack minStack;
	printf("MinStack passed? %s.\n", TestMinStack(&minStack) ? ("Yes") : ("No"));
	
	return 0;
}
