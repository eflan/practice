#include <stdio.h>
#include <list>
#include <vector>
#include <map>

class BinaryTreeNode
{
public:
	BinaryTreeNode() : _value(0), _left(nullptr), _right(nullptr), _parent(nullptr) {}
	
	BinaryTreeNode(unsigned int value) : _value(value), _left(nullptr), _right(nullptr), _parent(nullptr) {}
	
	BinaryTreeNode(unsigned int value, BinaryTreeNode *left, BinaryTreeNode *right)
		: _value(value), _left(left), _right(right), _parent(nullptr)
	{
	}
	
	const BinaryTreeNode *Left() const
	{
		return _left;
	}

	const BinaryTreeNode *Right() const
	{
		return _right;
	}
	
	const BinaryTreeNode *CommonAncestor(const BinaryTreeNode *node1, const BinaryTreeNode *node2) const
	{
		return CommonAncestor(this, node1, node2);
	}
	
	void SetupParents()
	{
		if(_left != nullptr)
		{
			_left->_parent = this;
			_left->SetupParents();
		}
		
		if(_right != nullptr)
		{
			_right->_parent = this;
			_right->SetupParents();
		}
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

	std::vector<std::list<const BinaryTreeNode *> *> *CollectLevels() const
	{
		std::vector<std::list<const BinaryTreeNode *> *> *levels = new std::vector<std::list<const BinaryTreeNode *> *>();
		levels->push_back(new std::list<const BinaryTreeNode *>());
		CollectLevels(this, 0, *levels);
		return levels;
	}
	
	bool IsSearchTree() const
	{
		bool leftVerified = true;
		bool rightVerified = true;
		
		if(_left != nullptr)
		{
			leftVerified = IsSearchTree(true, _left, true, Value(), Value());
		}
		
		if(_right != nullptr)
		{
			rightVerified = IsSearchTree(false, _right, false, Value(), Value());
		}
		
		return leftVerified && rightVerified;
	}

	/**
	 *    3
	 *  1   
	 * 0 2
	 */
	const BinaryTreeNode *InOrderSuccessor() const
	{
		if(_right != nullptr)
		{
			return _right->Leftmost();
		}
		else
		{
			if(_parent == nullptr)
			{
				// if the root has no right subtree then it has no successor
				return nullptr;
			}
			else if(_parent->_left == this)
			{
				return _parent;
			}
		}
		
		return _parent->InOrderSuccessorHelper();
	}
	
	bool IsRightOfRoot() const
	{
		// The root is not right of itself
		if(_parent == nullptr)
		{
			return false;
		}
		else
		{
			if(_parent->_parent == nullptr)
			{
				// We found the root so are we to the right of it?
				return _parent->_right == this;
			}
		}
		
		// Search for the root
		return _parent->IsRightOfRoot();
	}
	
	const BinaryTreeNode *Leftmost() const
	{
		if(_left == nullptr)
		{
			return this;
		}
		
		return _left->Leftmost();
	}
	
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

	static void CollectLevels(const BinaryTreeNode *node, unsigned int level, std::vector<std::list<const BinaryTreeNode *> *> &nodesAtLevel)
	{
		if(node != nullptr)
		{
			// add this node to current level
			nodesAtLevel[level]->push_back(node);
		
			if(node->_left != nullptr || node->_right != nullptr)
			{
				if(nodesAtLevel.size() < level + 2)
				{
					// create the list for the next level
					nodesAtLevel.push_back(new std::list<const BinaryTreeNode *>());
				}
				
				// collect the nodes at that level (and below)
				CollectLevels(node->_left, level + 1, nodesAtLevel);
				CollectLevels(node->_right, level + 1, nodesAtLevel);
			}
		}
	}

	static bool IsSearchTree(const bool isLeftOfRoot, const BinaryTreeNode *node, const bool isLeftOfParent, const unsigned int rootValue, const unsigned int parentValue)
	{
		if(node == nullptr)
		{
			return true;
		}

		if(isLeftOfRoot)
		{
			if(node->Value() >= rootValue)
			{
				return false;
			}
		}
		else
		{
			if(node->Value() < rootValue)
			{
				return false;
			}
		}
		
		if(isLeftOfParent)
		{
			if(node->Value() >= parentValue)
			{
				return false;
			}
		}
		else
		{
			if(node->Value() < parentValue)
			{
				return false;
			}
		}
		
		return IsSearchTree(isLeftOfRoot, node->_left, true, rootValue, node->Value()) &&
		       IsSearchTree(isLeftOfRoot, node->_right, false, rootValue, node->Value());
	}

	BinaryTreeNode *InOrderSuccessorHelper() const
	{
		if(_parent == nullptr)
		{
			return nullptr;
		}
		else if(_parent->_left == this)
		{
			return _parent;
		}
		
		return _parent->InOrderSuccessorHelper();
	}
	
	static const BinaryTreeNode *CommonAncestor(const BinaryTreeNode *root, const BinaryTreeNode *node1, const BinaryTreeNode *node2)
	{
		// If either node is the root then they cannot share a common ancestor
		if(node1 == root || node2 == root)
		{
			return nullptr;
		}
		else if(node1 == node2)
		{
			return FindParent(root, node1);
		}
		else
		{
			if(Contains(root->_left, node1))
			{
				if(Contains(root->_right, node2))
				{
					return root;
				}
				else
				{
					// both nodes are on the left subtree
					return CommonAncestor(root->_left, node1, node2);
				}
			}
			else
			{
				if(Contains(root->_left, node2))
				{
					return root;
				}
				else
				{
					// both nodes are on the right subtree
					return CommonAncestor(root->_right, node1, node2);					
				}
			}
		}
	}
	
	static bool Contains(const BinaryTreeNode *root, const BinaryTreeNode *target)
	{
		if(root == nullptr)
		{
			return false;
		}
		else if(root == target)
		{
			return true;
		}
		
		return Contains(root->_left, target) || Contains(root->_right, target);
	}
	
	static const BinaryTreeNode *FindParent(const BinaryTreeNode *curNode, const BinaryTreeNode *target)
	{
		if(curNode->_left == target || curNode->_right == target)
		{
			return curNode;
		}
		else
		{
			if(curNode->_left != nullptr)
			{
				const BinaryTreeNode *possible = FindParent(curNode->_left, target);
				if(possible != nullptr)
				{
					return possible;
				}
			}
			
			if(curNode->_right != nullptr)
			{
				const BinaryTreeNode *possible = FindParent(curNode->_right, target);
				if(possible != nullptr)
				{
					return possible;
				}
			}
		}
		
		return nullptr;
	}

	unsigned int _value;
	BinaryTreeNode *_left;
	BinaryTreeNode *_right;
	BinaryTreeNode *_parent;
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
	printf("unbalanced1 is balanced? %s.\n", unbalanced1->IsBalanced() ? ("Yes") : ("No"));
	printf("unbalanced2 is balanced? %s.\n", unbalanced2->IsBalanced() ? ("Yes") : ("No"));
	
	printf("balanced1->IsSearchTree()? %s (Should be No).\n", balanced1->IsSearchTree() ? ("Yes") : ("No"));
	printf("balanced2->IsSearchTree()? %s (Should be No).\n", balanced2->IsSearchTree() ? ("Yes") : ("No"));
	printf("balanced3->IsSearchTree()? %s (Should be No).\n", balanced3->IsSearchTree() ? ("Yes") : ("No"));
	printf("balanced4->IsSearchTree()? %s (Should be No).\n", balanced4->IsSearchTree() ? ("Yes") : ("No"));
	printf("unbalanced1->IsSearchTree()? %s (Should be No).\n", unbalanced1->IsSearchTree() ? ("Yes") : ("No"));
	printf("unbalanced2->IsSearchTree()? %s (Should be No).\n", unbalanced2->IsSearchTree() ? ("Yes") : ("No"));
	
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
	printf("\nHeight of search tree constructed for {0, 1, 2, 3, 4, 5, 6, 7, 8, 9} is %zu. (Should be 4)\n\n", searchTree->TreeHeight());
	searchTree->Print();
	printf("\n\n");
	printf("searchTree->IsSearchTree()? %s (Should be Yes).\n\n", searchTree->IsSearchTree() ? ("Yes") : ("No"));

	searchTree = SortedArrayToBinarySearchTree(5, array);
	printf("Height of search tree constructed for {0, 1, 2, 3, 4} is %zu. (Should be 3)\n\n", searchTree->TreeHeight());
	searchTree->Print();
	printf("\n\n");
	printf("searchTree->IsSearchTree()? %s (Should be Yes).\n\n", searchTree->IsSearchTree() ? ("Yes") : ("No"));

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
	printf("Height of search tree constructed for {0, 1, 2, 3, ... 99} is %zu. (Should be 7)\n\n", searchTree->TreeHeight());
	searchTree->Print();
	printf("\n\n");
	printf("searchTree->IsSearchTree()? %s (Should be Yes).\n\n", searchTree->IsSearchTree() ? ("Yes") : ("No"));

	printf("\nLevels of unbalanced2 tree --\n");
	std::vector<std::list<const BinaryTreeNode *> *> *levels = unbalanced2->CollectLevels();
	for(const std::list<const BinaryTreeNode *> *level : *levels)
	{
		printf("(");
		for(const BinaryTreeNode *node : *level)
		{
			printf("%u ", node->Value());
		}
		printf(")\n");
	}
	
	BinaryTreeNode *basicInorderTest = new BinaryTreeNode(1, new BinaryTreeNode(0), new BinaryTreeNode(2));
	basicInorderTest->SetupParents();
	
	const BinaryTreeNode *leftmost = basicInorderTest->Leftmost();
	printf("\nBasic InOrder: ");
	BinaryTreeNode *next = const_cast<BinaryTreeNode *>(leftmost);
	do
	{
		printf("%u, ", next->Value());
		next = const_cast<BinaryTreeNode *>(next->InOrderSuccessor());
	}while(next != nullptr);
	printf("\n");
	
	/**
	 *         7
	 *    3         11
	 *  1   5    9     13
	 * 0 2 4 6  8 10  12 14
	 */
	BinaryTreeNode *inorderTest = new BinaryTreeNode(7,
	                                                 new BinaryTreeNode(3,
	                                                                    new BinaryTreeNode(1,
	                                                                                       new BinaryTreeNode(0),
	                                                                                       new BinaryTreeNode(2)),
	                                                                    new BinaryTreeNode(5,
	                                                                                       new BinaryTreeNode(4),
	                                                                                       new BinaryTreeNode(6))),
	                                                 new BinaryTreeNode(11,
	                                                                    new BinaryTreeNode(9,
	                                                                                       new BinaryTreeNode(8),
	                                                                                       new BinaryTreeNode(10)),
	                                                                    new BinaryTreeNode(13,
	                                                                                       new BinaryTreeNode(12),
	                                                                                       new BinaryTreeNode(14))));
	inorderTest->SetupParents();
	
	const BinaryTreeNode *leftmost2 = inorderTest->Leftmost();
	printf("InOrder: ");
	next = const_cast<BinaryTreeNode *>(leftmost2);
	do
	{
		printf("%u, ", next->Value());
		next = const_cast<BinaryTreeNode *>(next->InOrderSuccessor());
	}while(next != nullptr);
	printf("\n");
	
	printf("\nCommon ancestor of 14 and 10 is %u (Should be 11).\n",
	       inorderTest->CommonAncestor(inorderTest->Right()->Right()->Right(),
									   inorderTest->Right()->Left()->Right())->Value());

	printf("Common ancestor of 13 and 10 is %u (Should be 11).\n",
	       inorderTest->CommonAncestor(inorderTest->Right()->Right(),
									   inorderTest->Right()->Left()->Right())->Value());

	printf("Common ancestor of 13 and 9 is %u (Should be 11).\n",
	       inorderTest->CommonAncestor(inorderTest->Right()->Right(),
									   inorderTest->Right()->Left())->Value());

	printf("Common ancestor of 13 and 2 is %u (Should be 7).\n",
	       inorderTest->CommonAncestor(inorderTest->Right()->Right(),
									   inorderTest->Left()->Left()->Left())->Value());

	printf("Common ancestor of 5 and 2 is %u (Should be 3).\n",
	       inorderTest->CommonAncestor(inorderTest->Left()->Right(),
									   inorderTest->Left()->Left()->Left())->Value());
	return 0;
}
