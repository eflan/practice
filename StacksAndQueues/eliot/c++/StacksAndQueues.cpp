#include <string.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <memory>

class Stack
{
public:
	virtual ~Stack(){}
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
	virtual ~MinStack() {}

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
	
	size_t count() const
	{
		return _stack.size();
	}

	void print()
	{
		printf("[");
		for(unsigned int n : _stack)
		{
			printf(" %u ", n);
		}
		printf("]");
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

class SetOfStacks : public Stack
{
public:
	SetOfStacks(const size_t capacity) : _cap(capacity), _fullStacks()
	{
		_stack = new MinStack;
	}
	
	~SetOfStacks()
	{
		while(!_fullStacks.empty())
		{
			MinStack *front = _fullStacks.front();
			_fullStacks.pop_front();
			delete front;
		}
		
		delete _stack;
	}
	
	virtual unsigned int pop()
	{
		if(!_stack->isEmpty())
		{
			return _stack->pop();
		}
		else if(!_fullStacks.empty())
		{
			MinStack *front = _fullStacks.front();
			unsigned int value = front->pop();
			if(front->isEmpty())
			{
				_fullStacks.pop_front();
				delete front;
			}
			
			return value;
		}
		else
		{
			throw "Cannot pop from an empty stack!.\n";
		}
	}
	
	virtual void push(const unsigned int &value)
	{
		_stack->push(value);
		
		if(_stack->count() == _cap)
		{
			_fullStacks.push_front(_stack);
			_stack = new MinStack;
		}		
	}
	
	virtual const unsigned int &peek() const
	{
		if(!_stack->isEmpty())
		{
			return _stack->peek();
		}
		else if(!_fullStacks.empty())
		{
			return _fullStacks.front()->peek();
		}

		throw "Cannot peek at the top of an empty stack!.\n";
	}
	
	virtual bool isEmpty() const
	{
		for(const MinStack *stack : _fullStacks)
		{
			if(!stack->isEmpty())
			{
				return false;
			}
		}
		
		return _stack->isEmpty();
	}
	
private:
	const size_t _cap;
	MinStack *_stack;
	std::list<MinStack *> _fullStacks;
};

void TowersOfHanoi(unsigned int count, MinStack *from, MinStack *to, MinStack *temp)
{
	if(from->isEmpty())
	{		
		return;
	}
	else if(count == 1)
	{
		if(from->peek() > to->peek())
		{
			throw "You crushed a disk!";
		}
		
		to->push(from->pop());
	}
	else
	{
		// move count - 1 disks to temp
		TowersOfHanoi(count - 1, from, temp, to);
		
		// then move one disk from from to to
		TowersOfHanoi(1, from, to, temp);
		
		// then move N - 1 disks from temp to to
		TowersOfHanoi(count - 1, temp, to, from);
	}
}

class Queue
{
public:
	virtual ~Queue() {}
	virtual unsigned int dequeue() = 0;
	virtual void enqueue(const unsigned int &n) = 0;
	virtual const unsigned int &peek() const = 0;
	virtual bool isEmpty() const = 0;
};

class MyQueue : public Queue
{
public:
	MyQueue()
	{
	}
	
	virtual ~MyQueue()
	{
	}
	
	virtual unsigned int dequeue()
	{
		readyQueue();
		return _popStack.pop();
	}
	
	virtual void enqueue(const unsigned int &value)
	{
		_pushStack.push(value);
	}
	
	virtual const unsigned int &peek() const
	{
		readyQueue();
		return _popStack.peek();
	}
	
	virtual bool isEmpty() const
	{
		return _pushStack.isEmpty() && _popStack.isEmpty();
	}
	
private:
	void readyQueue() const
	{
		if(_popStack.isEmpty())
		{
			MinStack *pushStack = const_cast<MinStack *>(&_pushStack);
			MinStack *popStack = const_cast<MinStack *>(&_popStack);
			
			while(!pushStack->isEmpty())
			{
				popStack->push(pushStack->pop());
			}
		}
	}
	
	MinStack _pushStack;
	MinStack _popStack;
};

bool TestQueue(Queue &q)
{
	if(!q.isEmpty())
	{
		printf("Queue is not empty at the start of the test!\n");
		return false;
	}
	
	for(unsigned int i = 0; i < 100; i++)
	{
		q.enqueue(i);
	}

	if(q.peek() != 0)
	{
		printf("Queue peek after inserting %u elements was %u not element %u!\n", 100, q.peek(), 0);
		return false;
	}
	
	for(unsigned int i = 0; i < 100; i++)
	{
		unsigned int value = q.dequeue();
		if(value != i)
		{
			printf("Queue dequeue operation #%u = %u but expected %u!\n", i, value, i);
			return false;
		}
	}
	
	if(!q.isEmpty())
	{
		printf("Queue is not empty at the end of the test!\n");
		return false;
	}
	
	return true;
}

void SortAscendingUsingOnlyAnotherStack(Stack *stack)
{
	MinStack temp;

	// sort stack in descending order onto temp
	while(!stack->isEmpty())
	{
		unsigned int n = stack->pop();
		size_t count = 0;
		while(n > temp.peek())
		{
			stack->push(temp.pop());
			count++;
		}
		
		temp.push(n);
		
		for(size_t i = 0; i < count; i++)
		{
			temp.push(stack->pop());
		}
	}
	
	// pop everything off of temp onto stack
	while(!temp.isEmpty())
	{
		stack->push(temp.pop());
	}
}

enum SpeciesType {Feline, Canine};

class Animal
{
public:
	Animal(const char *const myName) : _name(myName) {}
	virtual ~Animal() {}
	
	virtual const SpeciesType species() const = 0;
	
	const char *const name() const { return _name; }
	
private:
    const char *const _name;
};

class Cat : public Animal
{
public:
	Cat(const char *const myName) : Animal(myName) {}
	
	virtual ~Cat() {}
	virtual const SpeciesType species() const { return Feline; }
};

class Dog : public Animal
{
public:
	Dog(const char *const myName) : Animal(myName) {}
	
	virtual ~Dog() {}
	virtual const SpeciesType species() const { return Canine; }
};

class AnimalShelter
{
public:
	void enqueue(Animal *animal)
	{
		_animals.push_back(animal);
		if(animal->species() == Feline)
		{
			_cats.push_back(std::prev(_animals.end()));
		}
		else
		{
			_dogs.push_back(std::prev(_animals.end()));
		}
	}
		
	Animal *dequeueAny()
	{
		Animal *animal = _animals.front();
		_animals.pop_front();
		if(animal->species() == Feline)
		{
			_cats.pop_front();
		}
		else
		{
			_dogs.pop_front();
		}
			
		return animal;
	}
	
	Cat *dequeueCat()
	{
		auto it = _cats.front();
		Cat *cat = static_cast<Cat *>(*it);
		_cats.pop_front();
		_animals.erase(it);
		return cat;
	}
	
	Dog *dequeueDog()
	{
		auto it = _dogs.front();
		Dog *dog = static_cast<Dog *>(*it);
		_dogs.pop_front();
		_animals.erase(it);
		return dog;
	}

private:	
	std::list<Animal *> _animals;
	std::list<std::list<Animal *>::iterator > _dogs;
	std::list<std::list<Animal *>::iterator > _cats;
};

bool TestAnimalShelter(AnimalShelter &shelter)
{
		const char *const names[] = {"Sophie", "Ralph", "Felix", "Sylvester", "Rex", "Regina", "Pepper", "Max", "Fraggle", "Buddy"};	
	
	shelter.enqueue(new Cat(names[0]));
	shelter.enqueue(new Dog(names[1]));
	shelter.enqueue(new Cat(names[2]));
	shelter.enqueue(new Cat(names[3]));
	shelter.enqueue(new Dog(names[4]));
	shelter.enqueue(new Dog(names[5]));
	shelter.enqueue(new Cat(names[6]));
	shelter.enqueue(new Dog(names[7]));
	shelter.enqueue(new Cat(names[8]));
	shelter.enqueue(new Dog(names[9]));
	
	// names is sorted by decreasing age so we just match up animals as they come out
	
	for(size_t i = 0; i < 3; i++)
	{
		Animal *animal = shelter.dequeueAny();
		if(0 != strcmp(animal->name(), names[i]))
		{
			printf("Dequeued %s but expected %s!\n", animal->name(), names[i]);
			return false;
		}
		delete animal;
	}
	
	// Queue is now "Sylvester" (cat), "Rex" (dog), "Regina" (dog), "Pepper" (cat), "Max" (dog), "Fraggle" (cat), "Buddy" (dog)
	Dog *dog = shelter.dequeueDog();
	if(0 != strcmp(dog->name(), "Rex"))
	{
		printf("Dequeued %s instead of %s!\n", dog->name(), "Rex");
		return false;
	}
	delete dog;
	
	// Queue is now "Sylvester" (cat), "Regina" (dog), "Pepper" (cat), "Max" (dog), "Fraggle" (cat), "Buddy" (dog)	
	Cat *cat = shelter.dequeueCat();
	if(0 != strcmp(cat->name(), "Sylvester"))
	{
		printf("Dequeued %s instead of %s!\n", cat->name(), "Sylvester");
		return false;
	}
	delete cat;
	
	// Queue is now "Regina" (dog), "Pepper" (cat), "Max" (dog), "Fraggle" (cat), "Buddy" (dog)	
	cat = shelter.dequeueCat();
	if(0 != strcmp(cat->name(), "Pepper"))
	{
		printf("Dequeued %s instead of %s!\n", cat->name(), "Pepper");
		return false;
	}
	delete cat;
	
	// Queue is now "Regina" (dog), "Max" (dog), "Fraggle" (cat), "Buddy" (dog)	
	dog = shelter.dequeueDog();
	if(0 != strcmp(dog->name(), "Regina"))
	{
		printf("Dequeued %s instead of %s!\n", cat->name(), "Pepper");
		return false;
	}
	delete dog;
	
	// Queue is now "Max" (dog), "Fraggle" (cat), "Buddy" (dog)	
	dog = shelter.dequeueDog();
	if(0 != strcmp(dog->name(), "Max"))
	{
		printf("Dequeued %s instead of %s!\n", dog->name(), "Max");
		return false;
	}
	delete dog;
	
	// Queue is now "Fraggle" (cat), "Buddy" (dog)	
	dog = shelter.dequeueDog();
	if(0 != strcmp(dog->name(), "Buddy"))
	{
		printf("Dequeued %s instead of %s!\n", dog->name(), "Max");
		return false;
	}
	delete dog;
	
	// Queue is now "Fraggle" (cat)
	cat = shelter.dequeueCat();
	if(0 != strcmp(cat->name(), "Fraggle"))
	{
		printf("Dequeued %s instead of %s!\n", cat->name(), "Fraggle");
		return false;
	}
	delete cat;
	return true;
}

int main(int argc, char *argv[])
{
	ThreeStacks threeStacks;

	printf("Stack1 passed? %s.\n", TestStack(threeStacks.Stack1()) ? ("Yes") : ("No"));
	printf("Stack2 passed? %s.\n", TestStack(threeStacks.Stack2()) ? ("Yes") : ("No"));
	printf("Stack3 passed? %s.\n", TestStack(threeStacks.Stack3()) ? ("Yes") : ("No"));

	MinStack minStack;
	printf("\nMinStack passed? %s.\n", TestMinStack(&minStack) ? ("Yes") : ("No"));
	
	SetOfStacks stackSet(10);
	printf("\nSetOfStacks passed? %s.\n", TestStack(&stackSet) ? ("Yes") : ("No"));
	
	MinStack from;
	MinStack to;
	MinStack temp;
	
	for(unsigned int i = 0; i < 10; i++)
	{
		from.push(10 - i);
	}
	
	TowersOfHanoi(10, &from, &to, &temp);

	printf("\nTo peg: ");
	to.print();
	printf("\n");
	
	MyQueue q;
	printf("\nMyQueue passed? %s.\n", TestQueue(q) ? ("Yes") : ("No"));
	
	MinStack unsorted;
	unsigned int numbers[30] = { 9, 34, 19, 21, 45, 65, 93, 88, 52,  6,
		                        38, 82, 36, 10, 28, 30, 20, 99, 62, 17,
		                        22,  8, 69, 46, 89, 14, 51,100, 44, 87};
	for(size_t i = 0; i < 30; i++)
	{
		unsorted.push(numbers[i]);
	}
	
	printf("\n");
	unsorted.print();
	SortAscendingUsingOnlyAnotherStack(&unsorted);
	printf("\nafter sorting is\n");
	unsorted.print();
	printf("\n");

	AnimalShelter shelter;
	printf("\nAnimalShelter passed? %s.\n", TestAnimalShelter(shelter) ? ("Yes") : ("No"));
	
	return 0;
}
