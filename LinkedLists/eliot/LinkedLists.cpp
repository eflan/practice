#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <unordered_map>
#include <memory>

struct Node
{
	unsigned int value;
	Node *next;
	
	Node(unsigned int v, Node *n) : value(v), next(n) {}
	Node() : value(0), next(nullptr) {}
};

class NodeHasher
{
public:
		size_t operator()(const unsigned int &value) const 
		{
			return value;
		}
};

void RemoveDuplicatesUnsortedLinkedList(Node *list)
{
	if(list == nullptr)
	{
		return;
	}
	
	std::unordered_map<unsigned int, Node *, NodeHasher> uniqueNodes;

	Node *prev = list;
	Node *p = list->next;
	
	uniqueNodes[list->value] = list;
	
	while(p != nullptr)
	{
		if(uniqueNodes.find(p->value) != uniqueNodes.end())
		{
			prev->next = p->next;
			Node *deleteMe = p;
			p = p->next;
			delete deleteMe;
		}
		else
		{
			uniqueNodes[p->value] = p;
			p = p->next;
			prev = prev->next;
		}
	}
}

bool FindInListUpTo(const unsigned int value, Node *list, Node *end)
{
	while(list != end)
	{
		if(list->value == value)
		{
			return true;
		}
		
		list = list->next;
	}
	
	return false;
}

void RemoveDuplicatesUnsortedLinkedListNoTempBuffer(Node *list)
{
	if(list == nullptr)
	{
		return;
	}
	
	Node *prev = list;
	Node *p = list->next;
		
	while(p != nullptr)
	{
		if(FindInListUpTo(p->value, list, p))
		{
			prev->next = p->next;
			Node *deleteMe = p;
			p = p->next;
			delete deleteMe;
		}
		else
		{
			p = p->next;
			prev = prev->next;
		}
	}
}

const Node *KthToLast(const size_t k, const Node *list)
{
	const Node *kth = list;
	const Node *p = list;
	
	for(size_t count = 0; count < k; count++)
	{
		if(p == nullptr)
		{
			// there is no kth to last node
			return nullptr;
		}
		else
		{
			p = p->next;
		}
	}
	
	while(p->next != nullptr)
	{
		kth = kth->next;
		p = p->next;
	}
	
	return kth;
}

void PrintList(Node *list)
{
	printf("(");
	for(Node *p = list; p != nullptr; p = p->next)
	{
		printf("%d", p->value);
		if(p->next != nullptr)
		{
			printf(", ");
		}
		else
		{
			printf(")");
		}
	}
}

void DeleteMiddleNode(Node *middle)
{
	middle->value = middle->next->value;
	Node *next = middle->next;
	middle->next = middle->next->next;
	delete next;
}

Node *PartitionAboutX(unsigned int x, Node *list)
{
	// walk the list until we find a node >= x
	Node *pivot = list;
	
	while(pivot != nullptr && pivot->value < x)
	{
		pivot = pivot->next;
	}
	
	// if every node in the list < x then it is already partitioned
	if(pivot != nullptr)
	{
		// then continue walking, placing nodes < x before this node
		Node *cur = pivot->next;
		Node *prev = pivot;
				
		while(cur != nullptr)
		{
			if(cur->value < x)
			{
				// move cur to head of the list
				Node *unlinked = cur;
				cur = cur->next;
				prev->next = cur;
				unlinked->next = list;
				list = unlinked;
			}
			else
			{
				// keep on moving
				prev = prev->next;
				cur = cur->next;
			}
		}
	}
	
	return list;
}

Node *Next(Node *n)
{
	if(n == nullptr)
	{
		return nullptr;
	}
	else
	{
		return n->next;
	}
}

unsigned int Value(Node *n)
{
	if(n != nullptr)
	{
		return n->value;
	}
	else
	{
		return 0;
	}
}

Node *AddNumbersAsLists_LeastSignificantFirstHelper(Node *num1, Node *num2, unsigned int carry)
{
	if(num1 == nullptr && num2 == nullptr)
	{
		if(carry != 0)
		{
			return new Node(carry, nullptr);
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		unsigned int n = Value(num1) + Value(num2) + carry;
		return new Node(n % 10, AddNumbersAsLists_LeastSignificantFirstHelper(Next(num1), Next(num2), n / 10));
	}
}

Node *AddNumbersAsLists_LeastSignificantFirst(Node *num1, Node *num2)
{
	return AddNumbersAsLists_LeastSignificantFirstHelper(num1, num2, 0);
}

Node *AddNumbersAsLists_MostSignificantFirstHelper(Node *num1, Node *num2, unsigned int *carry)
{
	if(num1 == nullptr && num2 == nullptr)
	{
		return nullptr;
	}
	else
	{
		unsigned int localCarry = 0;
		Node *rest = AddNumbersAsLists_MostSignificantFirstHelper(Next(num1), Next(num2), &localCarry);
		unsigned int n = Value(num1) + Value(num2) + localCarry;
		*carry = n / 10;
		return new Node(n % 10, rest);
	}
}

Node *AddNumbersAsLists_MostSignificantFirst(Node *num1, Node *num2)
{
	unsigned int carry = 0;
	Node *rest = AddNumbersAsLists_MostSignificantFirstHelper(num1, num2, &carry);
	if(carry != 0)
	{
		return new Node(carry, rest);
	}
	else
	{
		return rest;
	}
}

int main(int argc, char *argv[])
{
	Node *listOf5 = new Node(4, new Node(1, new Node(1, new Node(3, new Node(4, nullptr)))));
	Node *listOf7 = new Node(0, new Node(0, new Node(5, new Node(3, new Node(4, new Node(5, new Node(6, nullptr)))))));
	
	printf("\nRemoveDuplicatesUnsortedLinkedList() of ");
	PrintList(listOf5);
	RemoveDuplicatesUnsortedLinkedList(listOf5);
	printf(" is ");
	PrintList(listOf5);
	printf("\n");
	
	printf("RemoveDuplicatesUnsortedLinkedList() of ");
	PrintList(listOf7);
	RemoveDuplicatesUnsortedLinkedList(listOf7);
	printf(" is ");
	PrintList(listOf7);
	printf("\n");
	
	Node *anotherListOf5 = new Node(400, new Node(100, new Node(100, new Node(300, new Node(400, nullptr)))));
	Node *anotherListOf7 = new Node(0, new Node(0, new Node(500, new Node(300, new Node(400, new Node(500, new Node(600, nullptr)))))));
	
	printf("\nRemoveDuplicatesUnsortedLinkedListNoTempBuffer() of ");
	PrintList(anotherListOf5);
	RemoveDuplicatesUnsortedLinkedListNoTempBuffer(anotherListOf5);
	printf(" is ");
	PrintList(anotherListOf5);
	printf("\n");
	
	printf("RemoveDuplicatesUnsortedLinkedListNoTempBuffer() of ");
	PrintList(anotherListOf7);
	RemoveDuplicatesUnsortedLinkedListNoTempBuffer(anotherListOf7);
	printf(" is ");
	PrintList(anotherListOf7);
	printf("\n");
	
	Node *listOf10 = new Node(0, new Node(1, new Node(2, new Node(3, new Node(4, new Node(5, new Node(6, new Node(7, new Node(8, new Node(9, nullptr))))))))));
	const Node *kth = KthToLast(3, listOf10);
	printf("\n%u is the %u element from the end of the list ", kth->value, 3);
	PrintList(listOf10);
	printf("\n");
	kth = KthToLast(1, listOf10);
	printf("%u is the %u element from the end of the list ", kth->value, 1);
	PrintList(listOf10);
	printf("\n");
	
	printf("\nDeleting %u, middle node of ", listOf10->next->next->next->next->value);
	PrintList(listOf10); 
	DeleteMiddleNode(listOf10->next->next->next->next);
	printf(" results in the list ");
	PrintList(listOf10);
	printf("\n");

    Node *listOf50 = new Node(44, new Node(56, new Node(79, new Node(94, new Node(83, new Node(78, new Node(1, new Node(92, new Node(39, new Node(91,
                      new Node(86, new Node(10, new Node(11, new Node(22, new Node(12, new Node(36, new Node(32, new Node(61, new Node(5, new Node(34,
                       new Node(28, new Node(46, new Node(71, new Node(13, new Node(25, new Node(30, new Node(73, new Node(88, new Node(20, new Node(95,
                        new Node(74, new Node(60, new Node(35, new Node(50, new Node(18, new Node(67, new Node(70, new Node(29, new Node(41, new Node(84,
                         new Node(66, new Node(47, new Node(72, new Node(57, new Node(26, new Node(81, new Node(21, new Node(48, new Node(98, new Node(100, nullptr))))))))))))))))))))))))))))))))))))))))))))))))));
	

	printf("\nList \n");
	PrintList(listOf50);
	printf("\nPartitioned around %u is\n", 42);
	Node *partitioned = PartitionAboutX(42, listOf50);
	PrintList(partitioned);
	printf("\n");
	
	printf("\nList \n");
	PrintList(partitioned);
	printf("\nPartitioned around %u is\n", 90);
	partitioned = PartitionAboutX(90, partitioned);
	PrintList(partitioned);
	printf("\n");
	
	printf("\nList \n");
	PrintList(partitioned);
	printf("\nPartitioned around %u is\n", 100);
	partitioned = PartitionAboutX(100, partitioned);
	PrintList(partitioned);
	printf("\n");
	
	Node *sevenOneSix = new Node(7, new Node(1, new Node(6, nullptr)));
	Node *fiveNineTwo = new Node(5, new Node(9, new Node(2, nullptr)));
	Node *sum = AddNumbersAsLists_LeastSignificantFirst(sevenOneSix, fiveNineTwo);
	printf("\n");
	PrintList(sevenOneSix);
	printf(" + ");
	PrintList(fiveNineTwo);
	printf(" = ");
	PrintList(sum);
	printf("\n");
	
	Node *sixOneSeven = new Node(6, new Node(1, new Node(7, nullptr)));
	Node *twoNineFive = new Node(2, new Node(9, new Node(5, nullptr)));
	sum = AddNumbersAsLists_MostSignificantFirst(sixOneSeven, twoNineFive);
	PrintList(sixOneSeven);
	printf(" + ");
	PrintList(twoNineFive);
	printf(" = ");
	PrintList(sum);
	printf("\n");
	
	return 0;
}
