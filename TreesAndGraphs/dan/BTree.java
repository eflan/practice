/**
 * Straightforward Binary Tree, left child is less than root, right child is greater than root
 */

import java.util.Random;
import java.util.ArrayList;
import java.util.Iterator;

public class BTree {
  protected int size;
  protected BNode root;

  public BTree()
  {
    size = 0;
    root = null;
  }

  public BTree(BNode root)
  {
    size = 1;
    this.root = root;
  }
  // return number of nodes
  public int size()
  {
    return size;
  }

  // determine is tree is empty
  public boolean isEmpty()
  {
    return size == 0;
  }

  // determine is a node is internal
  public boolean isInternal(BNode v)
  {
    return (v.hasLeft() || v.hasRight());
  }

  // check if node is root
  public boolean isRoot(BNode v)
  {
    return v == root;
  }

  // return the root
  public BNode root()
  {
    return root;
  }

  // determine if a given node has left child
  public boolean hasLeft(BNode v)
  {
    return v.getLeft() != null;
  }

  // determine if a given node has left child
  public boolean hasRight(BNode v)
  {
    return v.getRight() != null;
  }

  // remove a node, adjust tree
  public BNode remove(BNode v)
  {
    if (isRoot(v))
    {
      if (v.hasLeft())
      {
        root = v.getLeft();
        add(v.getRight(), false);
      }
      else
        root = v.getRight();
    }
    else if (!isInternal(v))
    {
      v.getParent().remove(v);
      v.setParent(null);
    }
    else
    {
      v.getParent().remove(v);
      if (v.hasLeft())
        add(v.getLeft(), false);
      if (v.hasRight())
        add(v.getRight(), false);
    }
    size--;
    return v;
  } // end remove

  // add a node
  public void add(BNode v, boolean increment)
  {
    if (isEmpty())
    {
      root = v;
    }
    else
    {
      BNode adoptiveParent = binarySearch(v);
      v.setParent(adoptiveParent);
      if (v.compareTo(adoptiveParent) < 0)
        adoptiveParent.setLeft(v);
      else if (v.compareTo(adoptiveParent) > 0)
        adoptiveParent.setRight(v);
      else
        System.out.println("Node already in tree");
    }
    if (increment)
      size++;
  }

  public void addNode(BNode v)
  {
    add(v, true);
  }

  // overkill on iteration, but general format for a n-ary tree
  public int height(BNode v)
  {
    if (!isInternal(v))
      return 0;
    else
    {
      int maxHeight = 0;
      Iterator<BNode> children = v.children();
      ArrayList<Integer> heights = new ArrayList<Integer>();
      while (children.hasNext())
      {
        heights.add(height((BNode) children.next()));
      }
      int max = Integer.MIN_VALUE;
      for (int i = 0; i < heights.size(); i++)
        max = Math.max(max, heights.get(i));
      return 1 + max;
    }
  }
  // find the node or next closest node, start search with root
  public BNode binarySearch(BNode search)
  {
    return binarySearchHelper(root, search);
  }

  public BNode binarySearchHelper(BNode current, BNode search)
  {
    if (isEmpty())
      return null;
    if (search.compareTo(current) == 0)
      return current;
    else if (!isInternal(current))
      return current;
    else
    {
      if (search.compareTo(current) < 0 && current.hasLeft())
        return binarySearchHelper(current.getLeft(), search);
      else if (search.compareTo(current) > 0 && current.hasRight())
        return binarySearchHelper(current.getRight(), search);
      else
        return current;
    }
  }

  public void preorderTraversal()
  {
    preorderTraversalHelper(root);
  }

  private void preorderTraversalHelper(BNode v)
  {
    if (!isEmpty())
    {
      System.out.println(v);
      if (v.hasLeft())
        preorderTraversalHelper(v.getLeft());
      if (v.hasRight())
        preorderTraversalHelper(v.getRight());
    }
    else
      System.out.println("Tree is empty");
  }

  public void inorderTraversal()
  {
    inorderTraversalHelper(root, false);
  }

  public void inorderTraversalPlusHeight()
  {
    inorderTraversalHelper(root, true);
  }

  private void inorderTraversalHelper(BNode v, boolean tellHeight)
  {
    if (!isEmpty())
    {
      if (v.hasLeft())
        inorderTraversalHelper(v.getLeft(), tellHeight);
      if (tellHeight)
        System.out.println(v + ", height = " + height(v));
      else
        System.out.println(v);
      if (v.hasRight())
        inorderTraversalHelper(v.getRight(), tellHeight);
    }
    else
      System.out.println("Tree is empty");
  }

  public void postorderTraversal()
  {
    postOrderTraversalHelper(root);
  }

  private void postOrderTraversalHelper(BNode v)
  {
    if (!isEmpty())
    {
      if (v.hasLeft())
        postOrderTraversalHelper(v.getLeft());
      if (v.hasRight())
        postOrderTraversalHelper(v.getRight());
      System.out.println(v);
    }
    else
      System.out.println("Tree is empty");
  }



  // in class testing
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
    System.out.println("PreOrder");
    btree.preorderTraversal();
    System.out.println("PostOrder");
    btree.postorderTraversal();
    System.out.println("Remove the root");
    btree.remove(btree.root());
    System.out.println("InOrder");
    btree.inorderTraversal();
    System.out.println("Root: " + btree.root());
    btree.inorderTraversal();
    System.out.println("Size: " + btree.size());
    String direction = "";
    System.out.println("Root: " + btree.root());
    if (btree.root().hasLeft())
    {
      direction = "Left"; btree.remove(btree.root().getLeft());
    }
    else
    {
      direction = "Right"; btree.remove(btree.root().getRight());
    }
    System.out.println("Removed the Root's " + direction + " Child");
    btree.inorderTraversal();
    System.out.println("Size: " + btree.size());
    System.out.println("Height of root (" + btree.root() + "): " + btree.height(btree.root()));
  }
} // end BinaryTree
