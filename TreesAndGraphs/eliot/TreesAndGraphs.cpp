#include <stdio.h>
#include <list>
#include <map>

class BinaryTreeNode
{
public:
	BinaryTreeNode() : _value(0), _left(nullptr), _right(nullptr) {}
	
	BinaryTreeNode(unsigned int value) : _value(value), _left(nullptr), _right(nullptr) {}
	
	BinaryTreeNode(unsigned int value, BinaryTreeNode *left, BinaryTreeNode *right)
		: _value(value), _left(left), _right(right)
	{
	}
	
	const bool IsBalanced() const
	{
		const size_t leftHeight = Height(_left);
		const size_t rightHeight = Height(_right);
		if(leftHeight == rightHeight)
		{
			return true;
		}
		else if(leftHeight > rightHeight)
		{
			return leftHeight - rightHeight == 1;
		}
		else
		{
			return rightHeight - leftHeight == 1;
		}
	}	
	
	void InsertForSearch(BinaryTreeNode *node)
	{
		if(node->_value < _value)
		{
			if(_left == nullptr)
			{
				_left = node;
			}
			else
			{
				_left->InsertForSearch(node);
			}
		}
		else
		{
			if(_right == nullptr)
			{
				_right = node;
			}
			else
			{
				_right->InsertForSearch(node);
			}
		}
	}
	
	void Print()
	{
		PrintTree(this);
	}
	
	const unsigned int Value() const { return _value; }
	const size_t TreeHeight() const { return Height(this); }
	
private:
	static void PrintTree(BinaryTreeNode *node)
	{
		if(node != nullptr)
		{
			printf("(");
			PrintTree(node->_left);
			printf(")");
			printf("%.3u ", node->_value);
			printf("(");
			PrintTree(node->_right);
			printf(")");
		}		
	}
	
	static const size_t Height(const BinaryTreeNode *node)
	{
		if(node == nullptr)
		{
			return 0;
		}
		else
		{
			const size_t leftHeight = Height(node->_left);
			const size_t rightHeight = Height(node->_right);
			const size_t maxHeight = (leftHeight > rightHeight) ? (leftHeight) : (rightHeight);
			return 1 + maxHeight;
		}
	}
	
	unsigned int _value;
	BinaryTreeNode *_left;
	BinaryTreeNode *_right;
};

class DirectedNode
{
public:
	DirectedNode(unsigned int label = 0) : _label(label), _edges() {}
	
	void ConnectTo(const DirectedNode *target)
	{
		_edges.push_back(target);
	}
	
	const unsigned int Label() const { return _label; }
	
	std::list<const DirectedNode *>::const_iterator Edges() const { return _edges.begin(); }
	std::list<const DirectedNode *>::const_iterator End() const { return _edges.end(); }
	
private:
	unsigned int _label;
	std::list<const DirectedNode *> _edges;
};

bool PathFromToHelper(const DirectedNode *from, const DirectedNode *to, std::map<unsigned int, const DirectedNode *> &visited)
{
	std::list<const DirectedNode *>::const_iterator edges = from->Edges();
	
	while(edges != from->End())
	{
		if(visited.end() == visited.find((*edges)->Label()))
		{
			visited.insert(std::make_pair((*edges)->Label(), *edges));
			
			if(*edges == to || PathFromToHelper(*edges, to, visited))
			{
				return true;
			}
		}
		
		edges++;
	}
	
	return false;
}

bool PathFromTo(const DirectedNode *from, const DirectedNode *to)
{
	std::map<unsigned int, const DirectedNode *> visited;
	
	visited.insert(std::make_pair(from->Label(), from));
	
	return PathFromToHelper(from, to, visited);
}

void SortedArrayToBinarySearchTreeHelper(BinaryTreeNode *root, size_t count, unsigned int *array)
{
	if(count == 0)
	{
		return;
	}
	else
	{
		size_t pivot = (count / 2);
		if((count % 2) == 0)
		{
			pivot--;
		}

		BinaryTreeNode *node = new BinaryTreeNode(array[pivot]);
		root->InsertForSearch(node);
		
		SortedArrayToBinarySearchTreeHelper(root, pivot, array);
		SortedArrayToBinarySearchTreeHelper(root, (count - pivot) - 1, array + pivot + 1);
	}
}

/**
 * 0 1 2 3 4 5 6 7 8 9
 * 
 *    4
 *  2   7
 * 1 3 6 8
 *0       9
 *
 *    4
 *  1
 * 0 2
 */
BinaryTreeNode *SortedArrayToBinarySearchTree(size_t count, unsigned int *array)
{
	if(count == 0)
	{
		return nullptr;
	}
	else
	{
		size_t pivot = (count / 2);
		if((count % 2) == 0)
		{
			pivot--;
		}

		BinaryTreeNode *root = new BinaryTreeNode(array[pivot]);

		SortedArrayToBinarySearchTreeHelper(root, pivot, array);
		SortedArrayToBinarySearchTreeHelper(root, (count - pivot) - 1, array + pivot + 1);
		
		return root;
	}
}

int main(int argc, char *argv[])
{
	BinaryTreeNode *balanced1 = new BinaryTreeNode(0,
	                                               new BinaryTreeNode(1),
	                                               nullptr);
	                          
	BinaryTreeNode *balanced2 = new BinaryTreeNode(0,
                        	                       new BinaryTreeNode(1),
	                                               new BinaryTreeNode(1));
	                          
	BinaryTreeNode *balanced3 = new BinaryTreeNode(0,
	                                               new BinaryTreeNode(1),
	                                               new BinaryTreeNode(1,
	                                                                  new BinaryTreeNode(2),
	                                                                  nullptr));
	                                             
	BinaryTreeNode *balanced4 = new BinaryTreeNode(0,
	                                               new BinaryTreeNode(1),
	                                               new BinaryTreeNode(1,
	                                                                  new BinaryTreeNode(2),
	                                                                  new BinaryTreeNode(2)));
	
	BinaryTreeNode *unbalanced1 = new BinaryTreeNode(0,
								                     new BinaryTreeNode(1),
								                     new BinaryTreeNode(1,
												                        new BinaryTreeNode(2,
																                           new BinaryTreeNode(3),
																                           nullptr),
												                        new BinaryTreeNode(2)));
	
	BinaryTreeNode *unbalanced2 = new BinaryTreeNode(0,
								                     new BinaryTreeNode(1,
								                                        new BinaryTreeNode(2,
								                                                           new BinaryTreeNode(3,
								                                                                              new BinaryTreeNode(4,
								                                                                                                 new BinaryTreeNode(5),
								                                                                                                 new BinaryTreeNode(5)),
                     								                                                         nullptr),
					                     			                                       nullptr),
								                                        nullptr),
								                     new BinaryTreeNode(1,
												                        new BinaryTreeNode(2,
																                           new BinaryTreeNode(3),
																                           nullptr),
												                        new BinaryTreeNode(2)));
									   
	printf("balanced1 is balanced? %s.\n", balanced1->IsBalanced() ? ("Yes") : ("No"));
	printf("balanced2 is balanced? %s.\n", balanced2->IsBalanced() ? ("Yes") : ("No"));
	printf("balanced3 is balanced? %s.\n", balanced3->IsBalanced() ? ("Yes") : ("No"));
	printf("balanced4 is balanced? %s.\n", balanced4->IsBalanced() ? ("Yes") : ("No"));
	printf("unalanced1 is balanced? %s.\n", unbalanced1->IsBalanced() ? ("Yes") : ("No"));
	printf("unalanced2 is balanced? %s.\n", unbalanced2->IsBalanced() ? ("Yes") : ("No"));
	
	// graph looks like so
	//
	// 0 -> 1 <-+
	// |    ^   |
	// V    |   |
	// 2 -> 3<->4
	
	DirectedNode node0(0);
	DirectedNode node1(1);
	DirectedNode node2(2);
	DirectedNode node3(3);
	DirectedNode node4(4);
	
	node0.ConnectTo(&node1);
	node0.ConnectTo(&node2);
	node2.ConnectTo(&node3);
	node3.ConnectTo(&node1);
	node3.ConnectTo(&node4);
	node4.ConnectTo(&node3);
	node4.ConnectTo(&node1);
	
	printf("\n0 connects 1? %s. (Should be Yes)\n", PathFromTo(&node0, &node1) ? ("Yes") : ("No"));
	printf("0 connects 2? %s. (Should be Yes)\n", PathFromTo(&node0, &node2) ? ("Yes") : ("No"));
	printf("0 connects 3? %s. (Should be Yes)\n", PathFromTo(&node0, &node3) ? ("Yes") : ("No"));
	printf("0 connects 4? %s. (Should be Yes)\n", PathFromTo(&node0, &node4) ? ("Yes") : ("No"));

	printf("1 connects 0? %s. (Should be No)\n", PathFromTo(&node1, &node0) ? ("Yes") : ("No"));
	printf("1 connects 2? %s. (Should be No)\n", PathFromTo(&node1, &node2) ? ("Yes") : ("No"));
	printf("1 connects 3? %s. (Should be No)\n", PathFromTo(&node1, &node3) ? ("Yes") : ("No"));
	printf("1 connects 4? %s. (Should be No)\n", PathFromTo(&node1, &node4) ? ("Yes") : ("No"));

	printf("2 connects 0? %s. (Should be No)\n", PathFromTo(&node2, &node0) ? ("Yes") : ("No"));
	printf("2 connects 1? %s. (Should be Yes)\n", PathFromTo(&node2, &node1) ? ("Yes") : ("No"));
	printf("2 connects 3? %s. (Should be Yes)\n", PathFromTo(&node2, &node3) ? ("Yes") : ("No"));
	printf("2 connects 4? %s. (Should be Yes)\n", PathFromTo(&node2, &node4) ? ("Yes") : ("No"));

	printf("3 connects 0? %s. (Should be No)\n", PathFromTo(&node3, &node0) ? ("Yes") : ("No"));
	printf("3 connects 1? %s. (Should be Yes)\n", PathFromTo(&node3, &node1) ? ("Yes") : ("No"));
	printf("3 connects 2? %s. (Should be No)\n", PathFromTo(&node3, &node2) ? ("Yes") : ("No"));
	printf("3 connects 4? %s. (Should be Yes)\n", PathFromTo(&node3, &node4) ? ("Yes") : ("No"));

	printf("4 connects 0? %s. (Should be No)\n", PathFromTo(&node4, &node0) ? ("Yes") : ("No"));
	printf("4 connects 1? %s. (Should be Yes)\n", PathFromTo(&node4, &node1) ? ("Yes") : ("No"));
	printf("4 connects 2? %s. (Should be No)\n", PathFromTo(&node4, &node2) ? ("Yes") : ("No"));
	printf("4 connects 3? %s. (Should be Yes)\n", PathFromTo(&node4, &node3) ? ("Yes") : ("No"));

	unsigned int array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	BinaryTreeNode *searchTree = SortedArrayToBinarySearchTree(10, array);
	printf("\nHeight of search tree constructed for {0, 1, 2, 3, 4, 5, 6, 7, 8, 9} is %zu. (Should be 4)\n", searchTree->TreeHeight());
	searchTree->Print();
	printf("\n");

	searchTree = SortedArrayToBinarySearchTree(5, array);
	printf("Height of search tree constructed for {0, 1, 2, 3, 4} is %zu. (Should be 3)\n", searchTree->TreeHeight());
	searchTree->Print();
	printf("\n");

	unsigned int array100[100] = {  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,
		                           10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
		                           20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
		                           30,  31,  32,  33,  34,  35,  36,  37,  38,  39,
		                           40,  41,  42,  43,  44,  45,  46,  47,  48,  49,
		                           50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
		                           60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
		                           70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
		                           80,  81,  82,  83,  84,  85,  86,  87,  88,  89,
		                           90,  91,  92,  93,  94,  95,  96,  97,  98,  99 };
		                        
	searchTree = SortedArrayToBinarySearchTree(100, array100);
	printf("Height of search tree constructed for {0, 1, 2, 3, ... 99} is %zu. (Should be 7)\n", searchTree->TreeHeight());
	searchTree->Print();
	printf("\n");
	
	return 0;
}
