/**
 *
 * Determine if a Binary Tree is a BST
 */

import java.util.Random;

public class IsBST
{
  public static boolean isBST(BNode node)
  {
    boolean is_a_BST = false;
    if (!node.hasChildren())
      return (is_a_BST = true);
    if (node.isInOrder())
    {
      if (node.hasLeft() && node.hasRight())
        is_a_BST = isBST(node.getLeft()) && isBST(node.getRight());
      else if (node.hasLeft())
        is_a_BST = isBST(node.getLeft());
      else if (node.hasRight())
        is_a_BST = isBST(node.getRight());
    }
    return is_a_BST;
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
    btree.inorderTraversal();

    System.out.println("Is this a BST? " + (isBST(btree.root()) ? "Yes" : "No"));

    System.out.println("Set Root to a value greater than greatest value in tree");
    btree.root().setData(10000000);
    System.out.println("Is this still a BST? " + (isBST(btree.root()) ? "Yes" : "No"));

  }
}//end IsBST
