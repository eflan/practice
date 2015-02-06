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
	
	return 0;
}
