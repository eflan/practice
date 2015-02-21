/**
 * Test if a binary tree is balanced
 */

import java.util.Random;

public class Balanced
{

  //BTree has function height, this is rewritten for practice
  //of making external static function and from scratch review of BTree concepts
  public static int height(BNode bnode)
  {
    if (!bnode.hasLeft() && !bnode.hasRight())
      return 0;
    else if (bnode.hasLeft() && bnode.hasRight())
      return 1 + Math.max(height(bnode.getLeft()), height(bnode.getRight()));
    else if (bnode.hasLeft())
      return 1 + height(bnode.getLeft());
    else
      return 1 + height(bnode.getRight());
  }

  public static int checkHeight(BNode bnode)
  {
    if (bnode == null)
      return 0;

    int leftHeight = checkHeight(bnode.getLeft());
    if (leftHeight == -1)
      return -1;

    int rightHeight = checkHeight(bnode.getRight());
    if (rightHeight == -1)
      return -1;

    if (Math.abs(leftHeight - rightHeight) > 1)
      return -1;
    else
      return 1 + Math.max(leftHeight, rightHeight);
  }

  public static boolean isBalancedTwo(BNode bnode)
  {
    int height = checkHeight(bnode);
    if (height == -1)
      return false;
    else
      return true;
  }

  public static boolean isBalanced(BNode bnode)
  {
    if (bnode.hasLeft() && bnode.hasRight())
    {
        if (Math.abs(height(bnode.getLeft()) - height(bnode.getRight())) > 1)
        {
          return false;
      }
    }
    else if (bnode.hasLeft())
    {
        if (height(bnode.getLeft()) > 1)
          return false;
    }
    else if (bnode.hasRight())
    {
      if (height(bnode.getRight()) > 1)
          return false;
    }
    return true;
  }

  public static void main(String[] args)
  {
    BTree btree = new BTree();
    btree.inorderTraversal();
    Random random = new Random();
    for (int i = 0; i < 10; i++)
    {
      int data = random.nextInt() % 100;
      System.out.println("Adding: " + data);
      btree.addNode(new BNode(data));
    }
    System.out.println("Root is " + btree.root());
    System.out.println("Height of root is " + height(btree.root()));
    System.out.println("Is the tree balanced? " + isBalanced(btree.root()));
    btree.inorderTraversalPlusHeight();
    System.out.println("Is the tree balanced (other algoritm)? " + isBalancedTwo(btree.root()));

  }


}
