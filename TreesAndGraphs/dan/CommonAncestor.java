/**
 * Find common ancestor of two nodes in Binary Tree
 */

import java.util.Random;

public class CommonAncestor
{
  public static BNode findCommonAncestor(BTree btree, BNode nodeOne, BNode nodeTwo)
  {
    if (nodeOne == nodeTwo)
      return nodeOne;
    else
    {
      int stepsToRootOne = 0;
      int stepsToRootTwo = 0;

      BNode ancestorOne = nodeOne;
      BNode ancestorTwo = nodeTwo;

      while ((ancestorOne != btree.root().getLeft() || ancestorOne != btree.root().getRight()) &&
          (ancestorOne != null))
      {
        ancestorOne = ancestorOne.getParent();
        stepsToRootOne++;
      }

      while ((ancestorTwo != btree.root().getLeft() || ancestorTwo != btree.root().getRight()) &&
          (ancestorTwo != null))
      {
        ancestorTwo = ancestorTwo.getParent();
        stepsToRootTwo++;
      }

      if (stepsToRootOne < stepsToRootTwo)
      {
        if ((ancestorOne == btree.root().getLeft() && ancestorTwo == btree.root().getLeft()) ||
            (ancestorOne == btree.root().getRight() && ancestorTwo == btree.root().getRight()))
          return nodeOne;
        else
          return btree.root();
      }
      else if (stepsToRootOne > stepsToRootTwo)
      {
        if ((ancestorOne == btree.root().getLeft() && ancestorTwo == btree.root().getLeft()) ||
            (ancestorOne == btree.root().getRight() && ancestorTwo == btree.root().getRight()))
          return nodeTwo;
        else
          return btree.root();
      }
      else
      {
        if ((ancestorOne == btree.root().getLeft() && ancestorTwo == btree.root().getLeft()) ||
            (ancestorOne == btree.root().getRight() && ancestorTwo == btree.root().getRight()))
          return nodeOne.getParent();
        else
          return btree.root();
      }
    }
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

    System.out.println("Common ancestor for root and root's left child: " + findCommonAncestor(btree, btree.root(), btree.root().getLeft()));

    BNode leftSide = btree.root();
    BNode rightSide = btree.root();

    while (leftSide.hasLeft())
      leftSide = leftSide.getLeft();
    while (rightSide.hasRight())
      rightSide = rightSide.getRight();

    System.out.println("Common ancestor for nodes on left (" + leftSide + ") and right (" + rightSide + ") of root: " + findCommonAncestor(btree, rightSide, leftSide));
  }

}// end Common Ancestor
