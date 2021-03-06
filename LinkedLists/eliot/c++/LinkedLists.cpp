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

Node *FindCycleHelper(Node *list, Node *canary)
{
	// If we reach the end of the list then there was no cycle.
	if(list == nullptr)
	{
		return nullptr;
	}
	else
	{
		//
		// If this node's next pointer is the canary value
		// then we have found a circular reference.
		//
		
		if(list->next == canary)
		{
			return list;
		}
		else
		{
			//
			// Recursive step is to unlink the current node
			// but cache its next pointer to fixup later.
			// Then repeat the process with the next node.
			// Finally, fixup to restore the list to its original state.
			//
			
			Node *fixup = list->next;
			list->next = canary;
			Node *cycle = FindCycleHelper(fixup, canary);
			list->next = fixup;
			return cycle;
		}
	}
}

Node *FindCycle(Node *list)
{
	Node canary(0, nullptr);
	return FindCycleHelper(list, &canary);
}

size_t Length(Node *list)
{
	size_t size = 0;
	while(list != nullptr)
	{
		list = list->next;
		size++;
	}
	
	return size;
}

Node *ElementAt(Node *list, size_t index)
{
	for(size_t i = 0; i < index; i++)
	{
		if(list == nullptr)
		{
			return nullptr;
		}
		else
		{
			list = list->next;
		}
	}
	
	return list;
}

// 0 -> 1 -> 0 is a palindrome
// 0 -> 1 -> 2 is not

bool IsPalindrome(Node *list)
{
	// Count the number of elements in the list.
	// Then walk a pointer from the start and match it against its counterpart.
	
	Node *head = list;
	size_t listLength = Length(list);
	
	// Treat empty lists as non-palindromes. This is open to debate.
	if(listLength == 0)
	{
		return false;
	}
	
	size_t offset = listLength - 1;
	for(size_t checkedCount = 0; checkedCount < listLength / 2; checkedCount++)
	{
		if(head->value != ElementAt(head, offset)->value)
		{
			return false;
		}
		
		head = head->next;
		offset -= 2;
	}
	
	return true;
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
	
	printf("\nLeast significant digit first --\n");
	Node *sevenOneSix = new Node(7, new Node(1, new Node(6, nullptr)));
	Node *fiveNineTwo = new Node(5, new Node(9, new Node(2, nullptr)));
	Node *sum = AddNumbersAsLists_LeastSignificantFirst(sevenOneSix, fiveNineTwo);
	PrintList(sevenOneSix);
	printf(" + ");
	PrintList(fiveNineTwo);
	printf(" = ");
	PrintList(sum);
	printf("\n");
	
	Node *fiveNineFive = new Node(5, new Node(9, new Node(5, nullptr)));
	sum = AddNumbersAsLists_LeastSignificantFirst(sevenOneSix, fiveNineFive);
	PrintList(sevenOneSix);
	printf(" + ");
	PrintList(fiveNineFive);
	printf(" = ");
	PrintList(sum);
	printf("\n");
	
	printf("\nMost significant digit first --\n");
	Node *sixOneSeven = new Node(6, new Node(1, new Node(7, nullptr)));
	Node *twoNineFive = new Node(2, new Node(9, new Node(5, nullptr)));
	sum = AddNumbersAsLists_MostSignificantFirst(sixOneSeven, twoNineFive);
	PrintList(sixOneSeven);
	printf(" + ");
	PrintList(twoNineFive);
	printf(" = ");
	PrintList(sum);
	printf("\n");
	
	Node *fourNineFive = new Node(4, new Node(9, new Node(5, nullptr)));
	sum = AddNumbersAsLists_MostSignificantFirst(sixOneSeven, fourNineFive);
	PrintList(sixOneSeven);
	printf(" + ");
	PrintList(fourNineFive);
	printf(" = ");
	PrintList(sum);
	printf("\n");
	
	// EXAMPLE Input: 0->1->2->3->4->2 (the same 2 as earlier). Output: 2.

	Node *circularList = new Node(0, new Node(1, new Node(2, new Node(3, new Node(4, nullptr)))));
	// make it circular
	circularList->next->next->next->next->next = circularList->next->next;
	Node *cycle = FindCycle(circularList);
	printf("\nCircular list ");
	
	unsigned int cycleCount = 0;
	for(Node *p = circularList; cycleCount != 2; p = p->next)
	{
		if(p == circularList)
		{
			printf("(");
		}
		
		if(p == cycle)
		{
			cycleCount++;
			if(cycleCount == 2)
			{
				printf(" %u)", cycle->value);
			}
			else
			{
				printf("%u -> ", p->value);	
			}
		}
		else
		{
			printf("%u -> ", p->value);	
		}
	}
	
	printf(" cycle start is %u (Node = %p).\n", cycle->value, cycle);
	
	Node *notCircular = new Node(0, new Node(1, new Node(2, new Node(3, new Node(4, nullptr)))));
	cycle = FindCycle(notCircular);
	printf("List ");
	PrintList(notCircular);
	
	if(cycle == nullptr)
	{
		printf(" is not circular.\n");
	}
	else
	{
		printf(" should not be found to be circular! [ERROR]\n");
	}
	
	Node *palindromeOdd = new Node(0, new Node(1, new Node(2, new Node(3, new Node(2, new Node(1, new Node(0, nullptr)))))));
	printf("\n");
	PrintList(palindromeOdd);
	printf(" is a palindrome? %s.\n", IsPalindrome(palindromeOdd) ? ("Yes") : ("No"));

	Node *palindromeEven = new Node(0, new Node(1, new Node(2, new Node(3, new Node(3, new Node(2, new Node(1, new Node(0, nullptr))))))));
	PrintList(palindromeEven);
	printf(" is a palindrome? %s.\n", IsPalindrome(palindromeEven) ? ("Yes") : ("No"));

	Node *notPalindromeOdd = new Node(0, new Node(1, new Node(2, new Node(2, new Node(0, nullptr)))));
	PrintList(notPalindromeOdd);
	printf(" is a palindrome? %s.\n", IsPalindrome(notPalindromeOdd) ? ("Yes") : ("No"));
	
	Node *notPalindromeEven = new Node(0, new Node(1, new Node(2, new Node(3, new Node(1, new Node(0, nullptr))))));
	PrintList(notPalindromeEven);
	printf(" is a palindrome? %s.\n", IsPalindrome(notPalindromeEven) ? ("Yes") : ("No"));
	
	Node *degeneratePalindrome = new Node(0, nullptr);
	PrintList(degeneratePalindrome);
	printf(" is a palindrome? %s.\n", IsPalindrome(degeneratePalindrome) ? ("Yes") : ("No"));
	printf("() is a palindrome? %s.\n", IsPalindrome(nullptr) ? ("Yes") : ("No"));
	
	return 0;
}
