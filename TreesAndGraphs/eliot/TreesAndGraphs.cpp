#include <stdio.h>

class BinaryTreeNode
{
public:
	BinaryTreeNode() : _value(0), _left(nullptr), _right(nullptr) {}
	
	BinaryTreeNode(unsigned int value) : _value(0), _left(nullptr), _right(nullptr) {}
	
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
	
private:
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
	
	return 0;
}
