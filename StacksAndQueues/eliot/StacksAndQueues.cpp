#include <string.h>
#include <list>
#include <unordered_map>
#include <memory>
#include <new>

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

int main(int argc, char *argv[])
{
	ThreeStacks threeStacks;

	printf("Stack1 passed? %s.\n", TestStack(threeStacks.Stack1()) ? ("Yes") : ("No"));
	printf("Stack2 passed? %s.\n", TestStack(threeStacks.Stack2()) ? ("Yes") : ("No"));
	printf("Stack3 passed? %s.\n", TestStack(threeStacks.Stack3()) ? ("Yes") : ("No"));

	return 0;
}
