/**
 * Create linked lists for all nodes in each level of a binary tree
 */

import java.util.LinkedList;
import java.util.Random;

public class BTreeLevels
{
  public static void levels(LinkedList<BNode> level, LinkedList<LinkedList<BNode>> levelsList)
  {
    if (level.size() < 1)
      return;
    else
    {
      levelsList.add((LinkedList<BNode>) level.clone());
      LinkedList<BNode> nextLevel = new LinkedList<BNode>();

      while (level.size() > 0)
        for (BNode child : level.remove().getChildren())
        {
          nextLevel.add(child);
        }
      levels(nextLevel, levelsList);
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

    System.out.println("InOrder");
    btree.inorderTraversal();
    LinkedList<BNode> root = new LinkedList<BNode>();
    root.add(btree.root());

    LinkedList<LinkedList<BNode>> levels = new LinkedList<LinkedList<BNode>>();
    levels(root, levels);

    int i = 0;

    for (LinkedList<BNode> level : levels)
    {
      System.out.print("Level: " + i + "\n\t");
      for (BNode node : level)
        System.out.print(node + " ");
      System.out.println();
      i++;
    }
  }
}// end BTreeLevels

