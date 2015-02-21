/**
 * Find Next Node in Binary Search Tree
 */

import java.util.Random;

public class FindNext
{
  public static BNode findNext(BNode node)
  {
    if (node.getParent() == null)
      return getLeftMostNode(node.getRight());
    else if (node.getData() <= node.getParent().getData())
      return node.getParent();
    else if (node.getData() > node.getParent().getData() && !node.hasRight())
      return null; // last node
    else
      if (node.hasRight())
        return getLeftMostNode(node.getRight());
      else
        return null;
  }

  private static BNode getLeftMostNode(BNode node)
  {
    while (node.hasLeft())
      node = node.getLeft();
    return node;
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

    System.out.println("InOrder");
    btree.inorderTraversal();
    System.out.println("Next node following root: " + findNext(btree.root()));
    if (btree.root().hasRight())
      System.out.println("Next node following root's right child: " + findNext(btree.root().getRight()));
    if (btree.root().hasLeft()) 
      System.out.println("Next node following root's left child: " + findNext(btree.root().getLeft()));
  }

}// end FindNext
